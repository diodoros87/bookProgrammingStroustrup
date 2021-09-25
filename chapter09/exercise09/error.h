#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <stdexcept>

using std::string;

#ifndef NDEBUG
#   define Unforeseen_Behavior(Msg) \
    __Unforeseen_Behavior(__FILE__, __LINE__, Msg)
#else
#   define Unforeseen_Behavior();
#endif

inline void __Unforeseen_Behavior(const char* file, const int line, const string& msg = "") {
   std::cerr << "!!!!! FATAL ERROR !!! Program should not be executed this instruction: \n"
      << msg << "\n" << "Source:\t\t" << file << ", line " << line << "\n";
   exit(EXIT_FAILURE);
}

class Invalid : public std::exception { 
   string msg {"!!!!! Error: "};
public:
   Invalid() {}
   Invalid(const string& message) { msg += message; }
   const char* what() {
      return msg.c_str();
   }
};
/*
class Unforeseen_Behavior : public std::runtime_error { 
   string msg {"!!!!! FATAL ERROR !!! Program should not be executed this instruction: "};
public:
   Unforeseen_Behavior() {}
   Unforeseen_Behavior(const char* file, const int line) { 
      msg += message; 
   }
   const char* what() {
      return msg.c_str();
   }
};
*/
inline void error(const std::string& errormessage) {
   throw std::runtime_error("!!!!! Error: " + errormessage);
}

inline void error(const std::string& s, const std::string& s2) {
	error(s + s2);
}

#endif
