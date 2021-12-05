#include "demo.hpp"
#include "print.hpp"
#include "utility.hpp"
#include "null.hpp"

using std::cerr;

namespace demo {
   
const regex Demo::NAME_REGEX       = Demo::set_regex() ;

static inline void validate(const unsigned int year, const string & function) {
   if (! regex_match(name, Demo::NAME_REGEX)) 
      throw std::invalid_argument(function + " argument of name: '" + name + "' is not matches by regex");
}

Demo_derived::Demo_derived(const string & name, const unsigned int year) : Demo(name) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, year) << '\n';
   validate(year, __func__);
   this->birth_year = year;
}

void Demo_derived::set_birth_year(const unsigned int year) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, year) << '\n';
   validate(year, __func__);
   this->birth_year = year;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->birth_year) << '\n';
}

unsigned int Demo_derived::get_birth_year() const {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__) << '\n';
   return birth_year;
}

Demo_derived::~Demo_derived() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->birth_year) << '\n';
}

}
