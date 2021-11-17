#include "utility.hpp"
#include "print.hpp"

#include <cerrno>
#include <cstdlib>
#include <cstring>

int test_linking() {
   int result = system("LD_LIBRARY_PATH=. ./linking_test_cpp");
   if (-1 == result) {
      cerr << "Call of system(\"LD_LIBRARY_PATH=. ./linking_test_cpp\") failed. Error: " << strerror(errno) << '\n';
   }
   else if (127 == result) {
      cerr << "Call of system(\"LD_LIBRARY_PATH=. ./linking_test_cpp\") failed. Shell could not be executed in the child process. \
      Error: " << strerror(errno) << '\n';
   }
   else if (0 == result) {
      cerr << "Call of system(\"LD_LIBRARY_PATH=. ./linking_test_cpp\") succeed. Return value is the termination status of the child \n \
      shell used to execute command. The termination status of a shell is the termination status of the last command it executes.\n \
      result: " << result << '\n';
   }
   else {
      cerr << " result: " << result << '\n';
   }
   return result;
}

int main() {
   cerr << "\n C++ " << __cplusplus << " function = " << __func__ << '\n';;
   int result = test_linking ();
   //if (result == 0)
   //   makefile();
   return result;
}
