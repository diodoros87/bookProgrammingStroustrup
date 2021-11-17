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
   const char cflags[] = "CFLAGS";
   const char comment[] = "#";
   int CFLAGS_position;
   int comment_position;
   char * pch;
   int cflags_line = 0;
   char * edited_line;
   const char * manual_dll_load_string = " -DMANUAL_DLL_LOAD";
   while ((line = readline(file, NULL)) != NULL) {
      
      for (pch = strtok (line," \t"); pch != NULL && 0 != strcmp(pch, comment); pch = strtok (NULL, " \t")) {
         if (0 == strcmp(pch, cflags)) {
            pch = strtok (NULL, " \t");
            if (pch == NULL)
               break;
            if (0 != strcmp(pch, "="))
               break;
            
            edited_line = calloc(strlen(line) + strlen(manual_dll_load_string) + 1, 1);
            strcat(edited_line, cflags);
            strcat(edited_line, " ");
            strcat(edited_line, "=");
            strcat(edited_line, manual_dll_load_string);
            LOG ("\n edited_line = %s \n", edited_line);
            while (pch = strtok (NULL, " \t")) {
               strcat(edited_line, " ");
               strcat(edited_line, pch);
            }
            free(line);
            LOG ("\n edited_line = %s \n", edited_line);
            line = edited_line;
            LOG ("\n line        = %s \n", line);
         } /*
         printf ("%s ",pch);*/
      }
      puts("");
      /*
      CFLAGS_position = strcspn(cflags, line);
      comment_position = strcspn(comment, line);
      LOG("CFLAGS_position = %d   comment_position = %d\n", CFLAGS_position, comment_position);
      if (comment_position > CFLAGS_position) {
         puts(line);
         puts("-DMANUAL_DLL_LOAD");
      }
      
      puts(line);*/
      fputs (line, edited_file);
      free(line);/*
      printf("\n");*/
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
   int result = test_linking ();
   if (result == OK)
      makefile();
   return result;
}
