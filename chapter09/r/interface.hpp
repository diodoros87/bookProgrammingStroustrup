#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <iostream>
#include "print.hpp"

namespace Hierarchy {

class Interface {
public:
   static const int NB;
   
   virtual int pv_number() const = 0;
   virtual char pv_char() const = 0;
   virtual void pv_print_number() const = 0;
   virtual void pv_print_char() const = 0;
   virtual ~Interface() { 
      std::cerr << '\n' << TIE("C++", unmove(__cplusplus), "Interface", __func__, Interface::NB) << '\n';
   }
   Interface(Interface const &) = delete;
   Interface& operator=(Interface const &) = delete;
protected:
   virtual bool pv_valid(const double) const = 0;
   Interface() { }
};

}


#endif
