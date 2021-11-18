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

char * readline(FILE *fp, char *buffer) {
    int ch;
    int i = 0;
    size_t buff_len = 0;

    buffer = malloc(buff_len + 1);
    if (!buffer) 
       return NULL;  // Out of memory

    while ((ch = fgetc(fp)) != '\n' && ch != EOF)
    {
        buff_len++;
        void *tmp = realloc(buffer, buff_len + 1);
        if (tmp == NULL)
        {
            free(buffer);
            return NULL; // Out of memory
        }
        buffer = tmp;

        buffer[i] = (char) ch;
        i++;
    }
    buffer[i] = '\0';

    // Detect end
    if (ch == EOF && (i == 0 || ferror(fp)))
    {
        free(buffer);
        return NULL;
    }
    return buffer;
}

void lineByline(FILE * file, FILE * edited_file) {
   char *line;
   const char cppflags[] = "CPPFLAGS";
   const char cflags[] = "CFLAGS";
   const size_t cflags_size = strlen(cflags);
   const size_t cppflags_size = strlen(cppflags);
   const char comment[] = "#";
   int CFLAGS_position;
   int comment_position;
   int skip_whole_line = 0;
   char * pch;
   char * edited_line;
   char * line_copy;
   char * flags_line; 
   char * comment_line;
   /*    */
   const char * manual_dll_load_string = " -DMANUAL_DLL_LOAD";
   for (line = readline(file, NULL); line != NULL; free(line), line = readline(file, NULL), skip_whole_line = 0, fprintf (edited_file, "%c", '\n')) {
      flags_line = strstr (line, cppflags);
      if (! flags_line)
         flags_line = strstr (line, cflags);
      if (flags_line) {
         comment_line = strstr (line, comment);
         if (! comment_line || (comment_line && comment_line > flags_line)) {
            for (pch = strtok (line, " \t"); pch; pch = strtok (NULL, " \t")) {
               fputs (pch, edited_file);
               if (0 == strncmp(pch, cflags, cflags_size - 1) || 0 == strncmp(pch, cppflags, cppflags_size - 1))
                  fputs(manual_dll_load_string, edited_file);
               fputs(" ", edited_file);
            } 
            continue;
         }
      }
      fputs (line, edited_file);
      
         /*
      }
      line_copy = calloc(strlen(line) + 1, sizeof (char));
      strcpy(line_copy, line);
      LOG ("\n 2 readline = %s \n", line);
      LOG ("\n 1 strlen(line) = %d \n", strlen(line));
      if (flags_line) {
         for (pch = strtok (line_copy," \t"); skip_whole_line == 0 && pch != NULL && 0 != strcmp(pch, comment); pch = strtok (NULL, " \t")) {
            LOG ("\n 2 strlen(line_copy) = %d \n", strlen(line_copy));
            if (0 == strncmp(pch, cflags, cflags_size - 1)) {   
               if (cflags_size - 1 == strlen(pch)) {   
                  pch = strtok (NULL, " \t");
                  LOG ("\n 1 pch = \'%s\' \n", pch);
                  if (pch == NULL)
                     break;
                  if (0 != strcmp(pch, "=") && 0 != strncmp(pch, "=-", 2))
                     break;
               }
               else if (0 != strncmp(pch, cflags, cflags_size))  
                  skip_whole_line = 1;
               LOG ("\n 2 pch = \'%s\' \n", pch);
               LOG ("\n 3 strlen(line) = %d \n", strlen(line));
               LOG ("\n strlen(manual_dll_load_string) = %d \n", strlen(manual_dll_load_string));
               cflags_line_found = 1;
               edited_line = calloc(strlen(line) + strlen(manual_dll_load_string) + 4, sizeof (char));
               strcat(edited_line, cflags);
               strcat(edited_line, manual_dll_load_string);
               LOG ("\n 1 edited_line = %s \n", edited_line);
               while (pch = strtok (NULL, " \t")) {
                  strcat(edited_line, " ");
                  strcat(edited_line, pch);
               } 
               free(line);
               LOG ("\n 2 edited_line = %s \n", edited_line);
               fputs (edited_line, edited_file);
               fprintf (edited_file, "%c", '\n');
               LOG ("\n 3 edited_line = %s \n", edited_line);
               free(edited_line);
            } 
         }
      }
      else {
         fputs (line, edited_file);
         fprintf (edited_file, "%c", '\n');
         free(line);
      } */
      
   }
   /*
   if (cflags_line_found == 0) {
      edited_line = calloc(strlen(edited_line) + strlen(manual_dll_load_string) + 1, sizeof char);
   } */
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
   FILE* file = open_file("Make2", "r+");
   FILE * edited_file = open_file("make2.tmp", "w");
   if (file) {
      lineByline(file, edited_file);
      fclose(file);
      fclose(edited_file);
      rename("make2.tmp", "Make2");
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
