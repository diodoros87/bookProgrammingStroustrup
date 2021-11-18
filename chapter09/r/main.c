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

char * readline(FILE *file) {
   int ch;
   int index = 0;
   size_t buff_len = 1;
   char * buffer = calloc(buff_len, sizeof (char));
   if (! buffer) {
      LOG_FUNC(__FUNCTION__);
      LOG("%s\n", "Out of memory");
      return NULL;
   }
   while ((ch = fgetc(file)) != '\n' && ch != EOF) {
      void *tmp = realloc(buffer, ++buff_len);
      if (tmp == NULL) {
         free(buffer);
         LOG_FUNC(__FUNCTION__);
         LOG("%s\n", "Out of memory");
         return NULL;
      }
      buffer = tmp;
      buffer[index++] = (char) ch;
   }
   if (ferror (file))
      perror("Error reading file\n");
   buffer[index] = '\0';
   if (ch == EOF && (index == 0 || ferror(file))) {
      free(buffer);
      return NULL;
   }
   
   return buffer;
}

enum Insert_flag { NOT_YET, FLAG, DONE };   /* FLAG = "CPPFLAGS" or "CFLAGS" to distinct with
                                                 "CPPFLAGS=" or "CFLAGS="   */
void lineByline(FILE * file, FILE * edited_file) {
   char *line;
   const char cppflags[] = "CPPFLAGS";
   const char cflags[] = "CFLAGS";
   const size_t cflags_size = strlen(cflags);
   const size_t cppflags_size = strlen(cppflags);
   const char comment[] = "#";
   char * pch;
   char * flags_line; 
   char * comment_line;
   enum Insert_flag inserting = NOT_YET;
   const char * manual_dll_load_string = " -DMANUAL_DLL_LOAD";
   for (line = readline(file); line != NULL; free(line), line = readline(file), fprintf (edited_file, "%c", '\n')) {
      flags_line = strstr (line, cppflags);
      if (! flags_line)
         flags_line = strstr (line, cflags);
      if (flags_line) {
         comment_line = strstr (line, comment);
         if (! comment_line || (comment_line && comment_line > flags_line)) {
            for (pch = strtok (line, " "); pch; pch = strtok (NULL, " ")) {
               fputs (pch, edited_file);
               if (FLAG == inserting) {
                  fputs(manual_dll_load_string, edited_file);
                  inserting = DONE;
               }
               else if (NOT_YET == inserting) {
                  if (0 == strcmp(pch, cflags) || 0 == strcmp(pch, cppflags))
                     inserting = FLAG;
                  else if (0 == strncmp(pch, cflags, cflags_size) || 0 == strncmp(pch, cppflags, cppflags_size)) {
                     fputs(manual_dll_load_string, edited_file);
                     inserting = DONE;
                  }
               }
               fputs(" ", edited_file);
            }
            inserting = NOT_YET;
            continue;
         }
      }
      fputs (line, edited_file);
   }
}

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
#ifdef MANUAL_DLL_LOAD
   int manual_dll_load = 0;
#else
   int manual_dll_load = 1;
   FILE* file = open_file("Makefile", "r+");
   FILE * edited_file = open_file("Makefile.tmp", "w");
   if (file) {
      lineByline(file, edited_file);
      fclose(file);
      fclose(edited_file);
      rename("Makefile.tmp", "Makefile");
   }
#endif
   
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
   /*
   char *exec_args[] = { "make", "clean", NULL };
   execute(exec_args);
   */
   edit_makefile();
   /*
   exec_args[1] = NULL;
   execute(exec_args);
   LOG("Parent process: pid = %d\nGoodbye!\n", getpid());
   */
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
