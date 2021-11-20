#include "utility.hpp"
#include "print.hpp"

#include <string>
#include <fstream>
#include <sstream>

#include <cerrno>
#include <cstdlib>
#include <cstring>

using std::ifstream;
using std::runtime_error;
using std::size_t;
using std::string::npos;
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
   iostream& stream;
   const ios_base::iostate old_e;
   Io_guard(iostream& s, ios_base::iostate e) :stream(s), old_e(stream.exceptions()) { 
      stream.exceptions(stream.exceptions | e); 
   }
   ~Io_guard() { stream.exceptions(old_e); }
   
   void open_file(const string & filename) {
      try {
         stream.open(filename);
      }
      catch (const ios_base::failure & e) {
         throw ios_base::failure("Exception opening file. Caught an ios_base::failure.\n" + e.what() +
                           "Error code: " + e.code() + "\n");
      }
   }
};

#ifdef MANUAL_DLL_LOAD
void delete_manual_dll_load(const string & filename, const string & erasing, const string & replacing) {
   /* ostringstream text;
   ifstream& in_file = open_file(filename);
   fstream file(filename, std::fstream::in | std::fstream::out);
   ifstream in_file(filename);*/
   Io_guard guard(ifstream {}, ios_base::failbit | ios_base::badbit);
   guard.open_file(filename);
   ostringstream text;
   text << in_file().rdbuf();
   string str = text.str();
   const string comment = "#";
   const string manual_dll_load_string = "-DMANUAL_DLL_LOAD"; 
   string line;
   string word;
   size_t manual_dll_load_position;
   size_t comment_position;
   size_t previous_line_position = 0;
   size_t next_line_position = str.find('\n', previous_line_position);
   for (; next_line_position != string::npos; previous_line_position = line_position + 1, next_line_position = str.find('\n', previous_line_position)) {
      manual_dll_load_position = line.find(manual_dll_load_string);
      if (manual_dll_load_position != std::string::npos) {
         comment_position = line.find(comment);
         if (comment_position != std::string::npos && comment_position > manual_dll_load_position) {  /* interested line (flags_line before comment)  */
            line.replace(pos, string(str_erasing).length(), str_replacing);
            for (word = strtok (line, " "); word; word = strtok (NULL, " ")) {   /*  2nd for loop  */
               if (0 != strcmp(word, manual_dll_load_string))  /* not insert manual_dll_load_string  */
                  fprintf (edited_file, "%s ", word);
            }
            continue;  /*  after process on interested line (flags_line before comment) continuing to 1st for loop  */
         } 
      }
      fputs (line, edited_file);  /*  insert not interested line without changes */
   }
   while (getline( myfile, line )) 
   /* 1st for loop  */
   for ( ; getline( guard.stream, line ); fprintf (edited_file, "%c", '\n')) {
      manual_dll_load_position = line.find(manual_dll_load_string);
      if (manual_dll_load_position != std::string::npos) {
         comment_position = line.find(comment);
         if (comment_position != std::string::npos && comment_position > manual_dll_load_position) {  /* interested line (flags_line before comment)  */
            line.replace(pos, string(str_erasing).length(), str_replacing);
            for (word = strtok (line, " "); word; word = strtok (NULL, " ")) {   /*  2nd for loop  */
               if (0 != strcmp(word, manual_dll_load_string))  /* not insert manual_dll_load_string  */
                  fprintf (edited_file, "%s ", word);
            }
            continue;  /*  after process on interested line (flags_line before comment) continuing to 1st for loop  */
         } 
      }
      fputs (line, edited_file);  /*  insert not interested line without changes */
   }
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
   const char * manual_dll_load_string = "-DMANUAL_DLL_LOAD";       
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
                  fprintf (edited_file, "%s ", manual_dll_load_string);
                  inserting = DONE;   /* change inserting to DONE protect before next inserting manual_dll_load_string */
               }
               else if (NOT_YET == inserting) {
                  if (0 == strcmp(word, cflags) || 0 == strcmp(word, cppflags))  /* word is "CPPFLAGS" or "CFLAGS"  */
                     inserting = FLAG;       /* signal to insert manual_dll_load_string in next iteration due to "=" is separated from "CPPFLAGS" or "CFLAGS" */
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

int makefile() {
   char *exec_args[] = { "make", "clean", NULL };
   int result = execute(exec_args);
   if (result != SYSTEM_ERROR) {
#ifdef MANUAL_DLL_LOAD
   edit_file(file, "-DMANUAL_DLL_LOAD", "");
#else
   edit_file(file, "-DMANUAL_DLL_LOAD", "");
#endif
   }
      result = edit_makefile();
   if (result == OK) {
      exec_args[1] = NULL;
      if (execute(exec_args) != SYSTEM_ERROR)
         result = OK;
   }
   LOG("Parent process: pid = %d\nGoodbye!\n", getpid());
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
   cerr << "\n C++ " << __cplusplus << " function = " << __func__ << '\n';;
   int result = test_linking ();
   if (result == 0)
      makefile();
   return result;
}
