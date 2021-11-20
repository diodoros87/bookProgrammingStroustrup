#include "utility.hpp"
#include "print.hpp"

#include <string>
#include <fstream>
#include <sstream>

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include <sys/types.h>
#include <sys/wait.h>

#include <dirent.h>
#include <unistd.h>

using std::fstream;
using std::ifstream;
using std::ofstream;
using std::runtime_error;
using std::size_t;
using std::iostream;
using std::ios_base;
using std::to_string;
using std::stringstream;
/*
ifstream& open_file(const string & filename) {
   ifstream file;
   file.exceptions ( file.exceptions() | ifstream::failbit | ifstream::badbit );
   try {
      file.open (filename);
      return file;
   }
   catch (ifstream::failure e) {
      throw std::runtime_error("Exception opening file\n" + e.what()); 
   }
}
*/
struct Io_guard {
   fstream& stream;
   const ios_base::iostate old_e;
   Io_guard(fstream& s, ios_base::iostate e) :stream(s), old_e(stream.exceptions()) { 
      stream.exceptions(stream.exceptions() | e); 
   }
   ~Io_guard() { stream.exceptions(old_e); }
   
   void open_file(const string & filename, ios_base::openmode mode = ios_base::in | ios_base::out) {
      try {
         stream.open(filename, mode);
      }
      catch (const ios_base::failure & e) {
         throw ios_base::failure("Exception opening file. Caught an ios_base::failure.\n" + string(e.what()) +
            "Error code: " + string(e.code().category().name()) + " : " + to_string((e.code().value())) + "\n");
      }
   }
};

#ifdef MANUAL_DLL_LOAD
void delete_manual_dll_load(const string & filename) {
   /* ostringstream text;
   ifstream& in_file = open_file(filename);
   fstream file(filename, std::fstream::in | std::fstream::out);*/
   fstream in_file;/*
   stringstream ss;
   ss << in_file.rdbuf();
    string str = ss.str();*/
   Io_guard in_guard(in_file, ios_base::failbit | ios_base::badbit);
   in_guard.open_file(filename, ios_base::in);
   stringstream text;
   text << in_guard.stream.rdbuf();
   in_guard.stream.close();
   
   fstream out_file;
   Io_guard out_guard(out_file, ios_base::failbit | ios_base::badbit);
   out_guard.open_file(filename, ios_base::out);

   const string comment = "#";
   const string manual_dll = "-DMANUAL_DLL_LOAD"; 
   const size_t manual_dll_size = manual_dll.size(); 
   size_t manual_dll_pos;
   size_t comment_pos;

   string line;
   //  1st while loop  
   for (; getline(text, line); out_guard.stream << '\n') {
      manual_dll_pos = line.find(manual_dll);
      if (manual_dll_pos != string::npos) {
         comment_pos = line.find(comment);
         if (comment_pos == string::npos || (comment_pos != string::npos && comment_pos > manual_dll_pos)) {  // interested line (flags_line before comment)
            stringstream line_stream(line);
            string word;
            bool not_comment = true; // code
            while (getline(line_stream, word, ' ')) { //  2nd while loop  
               if (not_comment) { // modify word only in code, not in comments
                  comment_pos = word.find(comment);
                  if (comment_pos != string::npos)
                     not_comment = false;
                  manual_dll_pos = word.find(manual_dll);
                  if (manual_dll_pos != string::npos) {
                     if (comment_pos == string::npos || (comment_pos != string::npos && comment_pos > manual_dll_pos)) {
                        cerr << " erasing word = " << word << '\n';
                        word.erase(manual_dll_pos, manual_dll_pos + manual_dll_size);  // erase manual_dll in word
                     }
                  }
               }
               out_guard.stream << word << ' '; // save to file modified or unmodified word  
            }
            continue;    //  after process on interested line (flags_line before comment) continuing to 1st while loop
         }
      }
      out_guard.stream << line;    //  insert not interested line without changes 
   }
   /*
   string str = text.str();
   size_t previous_line_position = 0;
   size_t next_line_position = str.find('\n', previous_line_position);
   for (; next_line_position != string::npos; previous_line_position = line_position + 1, next_line_position = str.find('\n', previous_line_position)) {
      string line = str.substr (previous_line_position, next_line_position);
      manual_dll_pos = line.find(manual_dll);
      if (manual_dll_pos != string::npos) {
         comment_pos = line.find(comment);
         if (comment_pos != string::npos && comment_pos > manual_dll_pos) {  // interested line (flags_line before comment)  
            line.replace(manual_dll_pos, manual_dll_size, "");
            for (word = strtok (line, " "); word; word = strtok (NULL, " ")) {   //  2nd for loop  
               if (0 != strcmp(word, manual_dll))  // not insert manual_dll  
                  fprintf (edited_file, "%s ", word);
            }
            continue;  //  after process on interested line (flags_line before comment) continuing to 1st for loop 
         } 
      }
      fputs (line, edited_file);  
   }
   while (getline( myfile, line )) 
   
   for ( ; getline( guard.stream, line ); fprintf (edited_file, "%c", '\n')) {
      manual_dll_pos = line.find(manual_dll);
      if (manual_dll_pos != std::string::npos) {
         comment_pos = line.find(comment);
         if (comment_pos != std::string::npos && comment_pos > manual_dll_pos) {  
            line.replace(pos, string(str_erasing).length(), str_replacing);
            for (word = strtok (line, " "); word; word = strtok (NULL, " ")) {   
               if (0 != strcmp(word, manual_dll))  
                  fprintf (edited_file, "%s ", word);
            }
            continue;  
         } 
      }
      fputs (line, edited_file);  
   }
   */
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
                     fprintf (edited_file, "%s ", word);  /* "=" is in "CPPFLAGS=" or "CFLAG="  */
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

int execute(char ** const argv) {
   pid_t  pid;
   int    status;
   if (! argv || ! *argv) { 
      throw runtime_error("Array of pointers is null / filename is null");
   }
   if ((pid = fork()) < 0) { 
      throw runtime_error("forking child process failed ");
   }
   else if (pid == 0) {
      cerr << "Child process pid = " << getpid() << ", parent process pid = " << getppid() << '\n';
      if (execvp(*argv, argv) < 0) {     
         throw runtime_error("Call of execvp failed. Error: " + string(strerror(errno)));
      }
   }
   else {
      cerr << "Parent process: pid = " << getpid() << '\n';
      cerr << "Parent process: waiting for completion of child process with pid = " << pid << '\n';
      pid_t result;
      do {
         result = wait(&status);
      } while (result != pid && result != -1);
/*
      LOG("status of child process: %d\n", status);
      LOG("wait(&status) on success returns the process ID of the terminated child - on failure, -1 is returned.\n \
      Result of waiting for child process: %d\n", result);
      LOG("result of waiting for child process %s child pid\n", result == pid ? "==" : "!=");*/
      if (result == -1) {
         throw runtime_error("Call of wait(&status) failed. Error: " +  string(strerror(errno)));
      }
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
int makefile() {
   char *exec_args[] = { "make", "clean", NULL };
   int result = execute(exec_args);
   if (result != -1) {
#ifdef MANUAL_DLL_LOAD
   delete_manual_dll_load("Make2");
   //edit_file(file, "-DMANUAL_DLL_LOAD", "");
#else
   //edit_file(file, "-DMANUAL_DLL_LOAD", "");
#endif
   }
      //result = edit_makefile();
   if (result != -1) {
      exec_args[1] = NULL;
      if (execute(exec_args) != -1)
         result = 0;
   }
   cerr << "Parent process: pid = " << getpid() << '\n';
   return result;
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
   cerr << "\n C++ " << __cplusplus << " function = " << __func__ << '\n';
   int result = test_linking ();
   if (result == 0)
      makefile();
   return result;
}
