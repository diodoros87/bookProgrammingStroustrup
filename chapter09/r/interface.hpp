#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <iostream>
#include "print.hpp"

namespace Hierarchy {

class Interface {
public:
   static const int nb;
   virtual int pv_number() const = 0;
   virtual char pv_char() const = 0;
   virtual void pv_print_number() const = 0;
   virtual void pv_print_char() const = 0;
   virtual ~Interface() { 
      std::cerr << '\n' << TIE("C++", unmove(__cplusplus), "Interface", __func__, Interface::nb) << '\n';
   }
};

}


#endif
