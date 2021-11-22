/*
#define MANUAL_DLL_LOAD
*/
#include "print.h"
#include "connector.h"
#include "file_modify.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <dirent.h>
#include <unistd.h>

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

int makefile(void) {
   char *exec_args[] = { "make", "clean", NULL };
   int result = execute(exec_args);
   if (result != SYSTEM_ERROR) {
      struct File_modify_t * modifier = File_modify_malloc();
      if (! modifier)
         return BAD_ALLOC;
      result = edit_makefile(modifier);
   }
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

int test_linking(void) {
   FUNCTION_INFO(__FUNCTION__);
   const char * const command = "LD_LIBRARY_PATH=. ./c_linking_test";
   int result = call_system(command);
   return result;
}

int main(void) {
   FUNCTION_INFO(__FUNCTION__);
   const char * const command = "LD_LIBRARY_PATH=. ./c_linking_test";
   int result = test_linking ();
   assert_many(result != SYSTEM_ERROR, "assert failed: ", "s d", "result == ", result);
   if (result == OK)
      result = makefile();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   if (result == OK)
      result = test_linking();
   assert_many(result != SYSTEM_ERROR, "assert failed: ", "s d", "result == ", result);
   return result;
}
