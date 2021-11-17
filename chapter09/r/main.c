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
   char *execArgs[] = { "make", "clean", NULL };
   execute(execArgs);
   execArgs[1] = NULL;
   execute(execArgs);
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
   int result = test_linking ();
   if (result == OK)
      makefile();
   return result;
}
