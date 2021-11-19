/*
#define MANUAL_DLL_LOAD
*/
#include "print.h"
#include "connector.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
#endif

char * read_line(FILE *file) {
   int ch;  /*  int (not char) due to value of EOF is a negative integer constant. The precise value is implementation-defined.  */
   int index = 0;
   size_t buff_len = 1;
   char * buffer = calloc(buff_len, sizeof (char));
   char * tmp_buff;
   if (! buffer) {
      LOG_FUNC(__FUNCTION__);
      LOG("%s\n", "Out of memory");
      return NULL;
   }
   while ((ch = fgetc(file)) != '\n' && ch != EOF) {
      if (ferror (file))
         perror("Error reading file\n");
      tmp_buff= realloc(buffer, ++buff_len);
      if (! tmp_buff) {
         free(buffer);
         LOG_FUNC(__FUNCTION__);
         LOG("%s\n", "Out of memory");
         return NULL;
      }
      buffer = tmp_buff;
      buffer[index++] = (char) ch;
   }
   buffer[index] = '\0';
   if (ch == EOF && (index == 0 || ferror(file))) {
      free(buffer);
      return NULL;
   }
   
   return buffer;
}

#ifdef MANUAL_DLL_LOAD
void delete_manual_dll_load(FILE * file, FILE * edited_file) {
   const char comment[] = "#";
   const char * manual_dll_load_string = "-DMANUAL_DLL_LOAD"; 
   char * line;
   char * word;
   char * manual_dll_load_line;
   char * comment_line;
   /* 1st for loop  */
   for (line = read_line(file); line != NULL; free(line), line = read_line(file), fprintf (edited_file, "%c", '\n')) {
      manual_dll_load_line = strstr (line, manual_dll_load_string);
      if (manual_dll_load_line) {
         comment_line = strstr (line, comment);
         if (! comment_line || (comment_line && comment_line > manual_dll_load_line)) {  /* interested line (flags_line before comment)  */
            for (word = strtok (line, " "); word; word = strtok (NULL, " ")) {   /*  2nd for loop  */
               if (0 != strcmp(word, manual_dll_load_string))  /* not insert manual_dll_load_string  */
                  fprintf (edited_file, "%s ", word);
            }
            continue;  /*  after process on interested line (flags_line before comment) continuing to 1st for loop  */
         } 
      }
      fputs (line, edited_file);  /*  insert not interested line without changes */
   }
}
#else
enum Insert_flag { NOT_YET, FLAG, DONE };   /* FLAG = "CPPFLAGS" or "CFLAGS" to distinct with
                                                 "CPPFLAGS=" or "CFLAGS="   */
void insert_manual_dll_load(FILE * file, FILE * edited_file) {
   const char cppflags[] = "CPPFLAGS";
   const char cflags[] = "CFLAGS";
   const size_t cflags_size = strlen(cflags);
   const size_t cppflags_size = strlen(cppflags);
   const char comment[] = "#";
   char * line;
   char * word;
   char * flags_line; 
   char * comment_line;
   enum Insert_flag inserting = NOT_YET;
   const char * manual_dll_load_string = " -DMANUAL_DLL_LOAD";       
   /* 1st for loop  */
   for (line = read_line(file); line != NULL; free(line), line = read_line(file), fprintf (edited_file, "%c", '\n')) {
      flags_line = strstr (line, cppflags);
      if (! flags_line)
         flags_line = strstr (line, cflags);
      if (flags_line) {
         comment_line = strstr (line, comment);
         if (! comment_line || (comment_line && comment_line > flags_line)) {   /* interested line (flags_line before comment)  */
            for (word = strtok (line, " "); word; word = strtok (NULL, " ")) {   /*  2nd for loop  */
               fputs (word, edited_file);
               if (FLAG == inserting) {
                  fputs(manual_dll_load_string, edited_file);
                  inserting = DONE;   /* change inserting to DONE protect before next inserting manual_dll_load_string */
               }
               else if (NOT_YET == inserting) {
                  if (0 == strcmp(word, cflags) || 0 == strcmp(word, cppflags))  /* word is "CPPFLAGS" or "CFLAGS"  */
                     inserting = FLAG;       /* signal to insert manual_dll_load_string in next iteration due to "=" is separated from "CPPFLAGS" or "CFLAGS" */
                  else if (0 == strncmp(word, cflags, cflags_size) || 0 == strncmp(word, cppflags, cppflags_size)) {  
                     fputs(manual_dll_load_string, edited_file);  /* "=" is in "CPPFLAGS=" or "CFLAG="  */
                     inserting = DONE;
                  }
               }
               fputs(" ", edited_file);
            }
            inserting = NOT_YET;  /* set to NOT_YET before read next line  */
            continue;  /*  after process on interested line (flags_line before comment) continuing to 1st for loop  */
         }
      }
      fputs (line, edited_file);  /*  insert not interested line without changes  */
   }
}
#endif

FILE* open_file( const char ** filename, const char * mode ) {
   FILE * file = fopen (filename, mode);
   if (file != NULL) {
      LOG("open file of %s\n", filename);
   }
   else
      LOG("file of %s is not exist\n", filename);
   return file;
}  

void edit_makefile() {
   FILE* file = open_file("Makefile", "r");
   FILE * edited_file = open_file("Makefile.tmp", "w");
   if (file && edited_file) {
#ifdef MANUAL_DLL_LOAD
      delete_manual_dll_load(file, edited_file);
#else
      insert_manual_dll_load(file, edited_file);
#endif
      fclose(file);
      fclose(edited_file);
      rename("Makefile.tmp", "Makefile");
   }
}

void execute(const char ** const argv) {
   pid_t  pid;
   int    status;
   if (! argv || ! *argv) { 
      LOG_EXIT(__FUNCTION__, "Array of pointers is null / filename is null", EXIT_FAILURE);
   }
   if ((pid = fork()) < 0) { 
      LOG_EXIT(__FUNCTION__, "forking child process failed ", EXIT_FAILURE);
   }
   else if (pid == 0) {
      LOG("Child process pid = %d, parent process pid = %d\n", getpid(), getppid());
      if (execvp(*argv, argv) < 0) {     
         LOG("Call of execvp failed. Error: %s\n", strerror(errno));
         exit(EXIT_FAILURE);
      }
   }
   else {
      LOG("Parent process: pid = %d\n", getpid());
      LOG("Parent process: waiting for completion of child process with pid = %d\n", pid);
      pid_t result;
      do {
         result = wait(&status);
      } while (result != pid && result != -1);

      LOG("status of child process: %d\n", status);
      LOG("wait(&status) on success returns the process ID of the terminated child - on failure, -1 is returned.\n \
      Result of waiting for child process: %d\n", result);
      LOG("result of waiting for child process %s child pid\n", result == pid ? "==" : "!=");
      if (result == -1) {
         LOG("Call of wait(&status) failed. Error: %s\n", strerror(errno));
      }
   }
}

void makefile() {
   
   char *exec_args[] = { "make", "clean", NULL };
   execute(exec_args);
   
   edit_makefile();
   
   exec_args[1] = NULL;
   execute(exec_args);
   LOG("Parent process: pid = %d\nGoodbye!\n", getpid());
   
}

int test_linking() {
   FUNCTION_INFO(__FUNCTION__);
   int result = system("LD_LIBRARY_PATH=. ./c_linking_test");
   if (-1 == result) {
      LOG("Call of system(\"LD_LIBRARY_PATH=. ./c_linking_test\") failed. Error: %s\n", strerror(errno));
   }
   else if (127 == result) {
      LOG("Call of system(\"LD_LIBRARY_PATH=. ./c_linking_test\") failed. Shell could not be executed in the child process. \
      Error: %s\n", strerror(errno));
   }
   else if (OK == result) {
      LOG("Call of system(\"LD_LIBRARY_PATH=. ./c_linking_test\") succeed. Return value is the termination status of the child \n \
      shell used to execute command. The termination status of a shell is the termination status of the last command it executes.\n \
      result: %d\n", result);
   }
   else {
      LOG(" result: %d\n", result);
   }
   return result;
}

int main(void) {
   FUNCTION_INFO(__FUNCTION__);
   system("cat Make.bak > Make2");
   int result = test_linking ();
   if (result == OK)
      makefile();
   return result;
}
