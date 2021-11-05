#include "demo.hpp"
#include "print.hpp"

using std::cerr;

namespace demo {

Demo::Demo(const char * NAME) : name(NAME) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, name) << '\n';
}

void Demo::set_name(const char *name) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, name) << '\n';
   this->name = name;
   throw std::invalid_argument {__func__ };
}

char * Demo::get_name() const {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__) << '\n';
   char * result = const_cast<char *>(name.data());
   return result;
}

Demo::~Demo() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->name) << '\n';
}

}
