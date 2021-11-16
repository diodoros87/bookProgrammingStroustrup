#include "demo.hpp"
#include "print.hpp"
#include "null.hpp"

using std::cerr;

namespace demo {
   
const regex Demo::NAME_REGEX       = Demo::set_regex() ;

static inline void validate(const char * name, const string & function, const string & message) {
   if (OK != check_pointer(name, function, message)) 
      throw std::invalid_argument(function + " argument of name: '" + name + "' is nullptr");
   if (! regex_match(name, Demo::NAME_REGEX)) 
      throw std::invalid_argument(function + " argument of name: '" + name + "' is not matches by regex");
}

Demo::Demo(const char * name) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, name) << '\n';
   validate(name, __func__, " Error name");
   this->name = name;
}

void Demo::set_name(const char *name) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, name) << '\n';
   validate(name, __func__, " Error name");
   this->name = name;
   //throw std::invalid_argument {__func__ };
}

char * Demo::get_name() const {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__) << '\n';
   char * result = const_cast<char *>(name.data());
   return result;
}

Demo::~Demo() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->name) << '\n';
}

const regex & Demo::set_regex() {
   try {
      static regex NAME_REGEX       = regex { R"(^[[:upper:]][[:lower:]]*( [[:upper:]][[:lower:]]*)?$)", std::regex::extended } ;
      return NAME_REGEX;
   } catch (const std::regex_error & e) {
      cerr << "regex_error caught: " << e.what() << '\n';
      throw e;
   }
}

}
