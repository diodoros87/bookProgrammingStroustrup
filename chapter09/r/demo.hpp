#ifndef DEMO_HPP
#define DEMO_HPP

#include <string>
#include <regex>

using std::string;
using std::regex;

namespace demo {

class Demo {
   string name;
   
   static const regex & set_regex();
public:
   Demo(const char *name);
   char * get_name() const;
   void set_name(const char * name);
   ~Demo();
      
   static const regex NAME_REGEX;
};

}

#endif
