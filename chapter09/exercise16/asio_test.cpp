
#include "floatrates_test.hpp"

using namespace std;

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <system_error>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
void view_document(const File_format & format, const string & CURRENCY, const string & JSON_DOC) {
   using Downloader = std::conditional_t<FLAG, Float_rates_json, Float_rates_xml>;
#ifdef __clang__
   Downloader  float_rates = {"", false };
#elif defined(__GNUG__)
   Downloader  float_rates = {""};
#endif
   float_rates.set_document(JSON_DOC);
   float_rates.set_rates_from_doc();
   map <string, float_rates_info> rates = float_rates.float_rates();
   for (const pair<string, float_rates_info> &p : rates) 
      cout << " 1 " << CURRENCY << " = " << p.second.rate << " " << p.second.code << " and "
         << " 1 " << p.second.code << " = " << p.second.inverse_rate << " " << CURRENCY << endl;
}
*/

template <typename... T>
int execute(const char * file, T &&... args) {
   if (! file)
      throw std::invalid_argument("filename is null");
   pid_t  pid;
   int    status;
   pid = fork();
   if (pid < 0) 
      throw system_error(make_error_code(errc::no_child_process), "forking child process failed ");
   else if (pid == 0) {
      cerr << "Child process pid = " << getpid() << ", parent process pid = " << getppid() << '\n';
      if (execlp(file, std::forward<T>(args)..., 0 ) < 0)  
         throw std::system_error(error_code(static_cast<int>(errno), std::generic_category()),
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
int test_linking(const bool valgrind) {
   static const string ld_path = "LD_LIBRARY_PATH=.";
   static const string exec = "./float_rates_test";
   int result = OK;
   if (OK == result && valgrind) {
      static const string valgrind_str = "valgrind --leak-check=full --show-leak-kinds=all --exit-on-first-error=yes --error-exitcode=1";
      result = system_utility::call_system(ld_path + " " + valgrind_str + " " + exec);
   }
   return result;
}
*/
int main() {
   char cwd[PATH_MAX];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
       return 1;
   }
   //pid_t pid;
   string pathvar = getenv("PATH");
   string newpath = pathvar + ":" + cwd;

   //pathvar = getenv("PATH");
   //strcpy(newpath, pathvar);
   //strcat(newpath, ":");
   //strcat(newpath, cwd);
   cerr << "PATH = " << newpath << '\n';
   int result = setenv("PATH", newpath.c_str(), 1);
   cerr << "PATH = " << getenv("PATH") << '\n';
   if (result) {
      cerr << std::strerror(errno) << '\n';
      cerr << " result of setenv = " << result << '\n';
      return result;
   }
   
   string executable = string(cwd) + "/floatrates_test";
   cerr << "executable = " << executable << '\n';
   return execute("valgrind", "valgrind", "--leak-check=full", "--show-leak-kinds=all", 
         //"--exit-on-first-error=yes", "--error-exitcode=1",
         executable.c_str());
}
