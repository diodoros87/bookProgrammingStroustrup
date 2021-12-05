#ifndef DEMO_HPP
#define DEMO_HPP

#include <string>
#include <regex>

using std::string;
using std::regex;

namespace demo {

class Demo_derived : public Demo {
   unsigned int birth_year;

public:
   Demo_derived(const char *name, const unsigned int year);
   unsigned int  get_birth_year() const;
   void set_birth_year(const unsigned int year);
   ~Demo_derived();
};

}

#endif
