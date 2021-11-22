#include "utility.hpp"
#include "print.hpp"
#include "file_edit.hpp"

#include <string>
   
#include <cerrno>
#include <cstdlib>
#include <cstring>

#include <sys/types.h>
#include <sys/wait.h>

#include <dirent.h>
#include <unistd.h>

using std::exception;
using std::system_error;
using std::invalid_argument;
using std::make_error_code;
using std::errc;
using std::error_code;

template <typename... T>
int execute(const char * file, T &&... args) {
   if (! file)
      throw invalid_argument("filename is null");
   pid_t  pid;
   int    status;
   pid = fork();
   if (pid < 0) 
      throw system_error(make_error_code(errc::no_child_process), "forking child process failed ");
   else if (pid == 0) {
      cerr << "Child process pid = " << getpid() << ", parent process pid = " << getppid() << '\n';
      if (execlp(file, std::forward<T>(args)..., 0 ) < 0)  
         throw system_error(error_code(static_cast<int>(errno), std::generic_category()),
                            "Call of execvp failed. Error: " + string(strerror(errno)));
   }
   else {
      cerr << "Parent process: pid = " << getpid() << '\n';
      cerr << "Parent process: waiting for completion of child process with pid = " << pid << '\n';
      pid_t result;
      do {
         result = wait(&status);
      } while (result != pid && result != -1);

      cerr << "status of child process: " << status << '\n';
      cerr << "wait(&status) on success returns the process ID of the terminated child - on failure, -1 is returned.\n \
      Result of waiting for child process: " << result << '\n';
      cerr << "result of waiting for child process " << (result == static_cast<int>(pid) ? "==" : "!=") << " child pid\n";
      if (result == -1) 
         throw system_error(error_code(static_cast<int>(errno), std::generic_category()),
                                       "Call of wait(&status) failed. Error: " +  string(strerror(errno)));
      return result;
   }
}

/*
void edit_file(const string & filename, const string & erasing, const string & replacing) {
   ostringstream text;
   ifstream in_file(filename);

   text << in_file().rdbuf();
   string str = text.str();
   string str_erasing = erasing;
   string str_replacing = replacing;
   size_t pos = str.find(str_erasing);
   str.replace(pos, string(str_erasing).length(), str_replacing);
   in_file().close();

   ofstream out_file(filename);
   out_file << str;
}
*/
void makefile() {
   execute("make", "make", "clean");
   File_edit file_edit("Makefile");
#ifdef MANUAL_DLL_LOAD
   file_edit.delete_manual_dll_load();
#else
   file_edit.insert_manual_dll_load();
#endif
   execute("make", "make");
   cerr << "Parent process: pid = " << getpid() << '\n';
}

int call_system(const string & command) {
   int result = system(command.c_str());
   if (-1 == result) {
      cerr << "Call of system(\"" << command << "\") failed. Error: " << strerror(errno) << '\n';
   }
   else if (127 == result) {
      cerr << "Call of system(\"" << command << "\") failed. Shell could not be executed in the child process. \
      Error: " << strerror(errno) << '\n';
   }
   else if (0 == result) {
      cerr << "Call of system(\"" << command << "\") succeed. Return value is the termination status of the child \n \
      shell used to execute command. The termination status of a shell is the termination status of the last command it executes.\n \
      result: " << result << '\n';
   }
   else {
      cerr << " result: " << result << '\n';
   }
   return result;
}

int test_linking() {
   const string command = "LD_LIBRARY_PATH=. ./linking_test_cpp";
   int result = call_system(command);
   return result;
}

int main() {
   try {
      cerr << "\n C++ " << __cplusplus << " function = " << __func__ << '\n';
      int result = test_linking ();
      if (result == 0) {
         makefile();
         result = test_linking();
      }
      assert_many(result == 0, "result == ", result);
      return result;
   } 
   catch (const system_error& e) {
      cerr << "Caught system_error with code " << e.code() << " meaning: " << e.what() << '\n';
      return e.code().value();
   }
   catch (const invalid_argument & e) {
      cerr << "Exception: " << e.what() << '\n';
   }
   catch (const exception & e) {
      cerr << "Exception: " << e.what() << '\n';
   }
   catch (...) {
      cerr << "Unrecognized Exception: " << '\n';
   }
   return EXIT_FAILURE;
}
