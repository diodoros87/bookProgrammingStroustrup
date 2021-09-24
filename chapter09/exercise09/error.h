#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <stdexcept>

using std::string;

class Invalid : public std::exception { 
   string msg {"!!!!! Error: "};
public:
   Invalid() {}
   Invalid(const string& message) { msg += message; }
   const char* what() {
      return msg.c_str();
   }
};

inline void error(const std::string& errormessage) {
   throw std::runtime_error("!!!!! Error: " + errormessage);
}

inline void error(const std::string& s, const std::string& s2) {
	error(s + s2);
}

#endif
