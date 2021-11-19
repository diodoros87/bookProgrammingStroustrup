#include "utility.hpp"
#include "print.hpp"

#include <string>
#include <fstream>
#include <sstream>

#include <cerrno>
#include <cstdlib>
#include <cstring>

void edit_file(const string & filename, const string & erasing, const string & replacing) {
   ostringstream text;
   ifstream in_file(filename);

   text << in_file().rdbuf();
   string str = text.str();
   string str_search = search;
   string str_replace = replace;
   size_t pos = str.find(str_search);
   str.replace(pos, string(str_search).length(), str_replace);
   in_file().close();

   ofstream out_file(filename);
   out_file << str;
    
}

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
