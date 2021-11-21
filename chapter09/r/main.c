/*
#define MANUAL_DLL_LOAD
*/
#include "print.h"
#include "connector.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <dirent.h>
#include <unistd.h>

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
#endif

char * read_line(FILE *file) {
   if (! file) { 
      LOG_EXIT(__FUNCTION__, "file is null", EXIT_FAILURE);
   }
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

typedef int bool_t;

#ifdef MANUAL_DLL_LOAD
void delete_manual_dll_load(FILE * file, FILE * edited_file) {
   if (! file || ! edited_file) { 
      LOG_EXIT(__FUNCTION__, "file is null / edited_file is null", EXIT_FAILURE);
   }
   const char comment[] = "#";
   const char * manual_dll = "-DMANUAL_DLL_LOAD"; 
   const size_t manual_dll_size = strlen(manual_dll); 
   char * line;
   char * word;
   char * manual_dll_line;
   char * comment_line;
   bool_t not_comment = 1;
   bool_t insert_word = 0;
   /* 1st for loop  */
   for (line = read_line(file); line != NULL; free(line), line = read_line(file), fprintf (edited_file, "%c", '\n')) {
      manual_dll_line = strstr (line, manual_dll);
      if (manual_dll_line) {
         comment_line = strstr (line, comment);
         if (! comment_line || (comment_line && comment_line > manual_dll_line)) {  /* interested line (flags_line before comment)  */
            not_comment = 1;   /* code, not comment */
            for (word = strtok (line, " "); word; word = strtok (NULL, " ")) {   /*  2nd for loop  */
               if (not_comment) { /* modify word only in code, not in comments */
                  comment_line = strstr (word, comment);
                  if (comment_line)
                     not_comment = 0;
                  manual_dll_line = strstr (word, manual_dll);
                  if (manual_dll_line) {
                     if (! comment_line || (comment_line && comment_line > manual_dll_line)) {
                        insert_word = 1;
                        size_t word_length = strlen(word);
                        for (size_t i = 0; i < word_length; ) {   /*  3rd for loop  */
                           if (word + i == manual_dll_line) {
                              for (size_t j = 0; j < manual_dll_size; j++)  /*  skip  manual_dll */
                                 i++;
                              continue;  /*  continue to 3rd for loop to check condition: i < word_length  */
                           }
                           fputc (word[i] , edited_file );
                           i++;
                        } 
                        fputc (' ' , edited_file );
                     }
                  }
               }
               if (0 == insert_word)
                  fprintf (edited_file, "%s ", word);
               else
                  insert_word = 0;
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
   if (! file || ! edited_file) { 
      LOG_EXIT(__FUNCTION__, "file is null / edited_file is null", EXIT_FAILURE);
   }
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
   const char * manual_dll = "-DMANUAL_DLL_LOAD";       
   /* 1st for loop  */
   for (line = read_line(file); line != NULL; free(line), line = read_line(file), fprintf (edited_file, "%c", '\n')) {
      flags_line = strstr (line, cppflags);
      if (! flags_line)
         flags_line = strstr (line, cflags);
      if (flags_line) {
         comment_line = strstr (line, comment);
         if (! comment_line || (comment_line && comment_line > flags_line)) {   /* interested line (flags_line before comment)  */
            for (word = strtok (line, " "); word; word = strtok (NULL, " ")) {   /*  2nd for loop  */
               fprintf (edited_file, "%s ", word);
               if (FLAG == inserting) {
                  fprintf (edited_file, "%s ", manual_dll);
                  inserting = DONE;   /* change inserting to DONE protect before next inserting manual_dll */
               }
               else if (NOT_YET == inserting) {
                  if (0 == strcmp(word, cflags) || 0 == strcmp(word, cppflags))  /* word is "CPPFLAGS" or "CFLAGS"  */
                     inserting = FLAG;       /* signal to insert manual_dll in next iteration due to "=" is separated from "CPPFLAGS" or "CFLAGS" */
                  else if (0 == strncmp(word, cflags, cflags_size) || 0 == strncmp(word, cppflags, cppflags_size)) {  
                     fprintf (edited_file, "%s ", manual_dll);  /* "=" is in "CPPFLAGS=" or "CFLAG="  */
                     inserting = DONE;
                  }
               }
            }
            inserting = NOT_YET;  /* set to NOT_YET before read next line  */
            continue;  /*  after process on interested line (flags_line before comment) continuing to 1st for loop  */
         }
      }
      fputs (line, edited_file);  /*  insert not interested line without changes  */
   }
}
#endif

FILE* open_file( const char * filename, const char * mode ) {
   if (! filename || ! mode) { 
      LOG_EXIT(__FUNCTION__, "file is null / file access mode is null", EXIT_FAILURE);
   }
   FILE * file = fopen (filename, mode);
   if (file != NULL) {
      LOG("open file of %s\n", filename);
   }
   else {
      LOG("file of %s is not exist\n", filename);
      LOG("Call of fopen failed. Error: %s\n", strerror(errno));
   }
   return file;
}  

int edit_makefile() {
   FILE* file = open_file("Makefile", "r");
   FILE * edited_file = open_file("Makefile.tmp", "w");
   if (! file || ! edited_file)
      return OPEN_FILE_ERROR;
#ifdef MANUAL_DLL_LOAD
   delete_manual_dll_load(file, edited_file);
#else
   insert_manual_dll_load(file, edited_file);
#endif
   if (0 != fclose(file) || 0 != fclose(edited_file)) {
      LOG("Call of fclose failed. Error: %s\n", strerror(errno));
      return FILE_CLOSE_ERROR;
   }
   if (0 != rename("Makefile.tmp", "Makefile")) {
      LOG("Call of rename failed. Error: %s\n", strerror(errno));
      return RENAME_FILE_ERROR;
   }
   return OK;
}

int execute(const char ** const argv) {
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
      return result;
   }
}

int makefile() {
   char *exec_args[] = { "make", "clean", NULL };
   int result = execute(exec_args);
   if (result != SYSTEM_ERROR)
      result = edit_makefile();
   if (result == OK) {
      exec_args[1] = NULL;
      if (execute(exec_args) != SYSTEM_ERROR)
         result = OK;
   }
   LOG("Parent process: pid = %d\nGoodbye!\n", getpid());
   return result;
}

int call_system(const char * const command) {
   FUNCTION_INFO(__FUNCTION__);
   if (! command) { 
      LOG_EXIT(__FUNCTION__, "command is null", EXIT_FAILURE);
   }
   LOG("Parent process: pid = %d\n", getpid());
   int result = system(command);
   if (-1 == result) {
      LOG("Call of system(\"%s\") failed. Error: %s\n", command, strerror(errno));
   }
   else if (127 == result) {
      LOG("Call of system(\"%s\") failed. Shell could not be executed in the child process. \
      Error: %s\n", command, strerror(errno));
   }
   else if (OK == result) {
      LOG("Call of system(\"%s\") succeed. Return value is the termination status of the child \n \
      shell used to execute command. The termination status of a shell is the termination status of the last command it executes.\n \
      result: %d\n", command, result);
      LOG("Parent process: pid = %d\n", getpid());
   }
   else {
      LOG(" result: %d\n", result);
   }
   return result;
}

int test_linking() {
   FUNCTION_INFO(__FUNCTION__);
   const char * const command = "LD_LIBRARY_PATH=. ./c_linking_test";
   int result = call_system(command);
   return result;
}

int main(int argc, char *argv[]) {
   FUNCTION_INFO(__FUNCTION__);
   const char * const command = "LD_LIBRARY_PATH=. ./c_linking_test";
   int result = test_linking (command);
   assert_many(result != SYSTEM_ERROR, "assert failed: ", "s d", "result == ", result);
   if (result == OK)
      result = makefile();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   if (result == OK)
      result = test_linking(command);
   assert_many(result != SYSTEM_ERROR, "assert failed: ", "s d", "result == ", result);
   return result;
}
