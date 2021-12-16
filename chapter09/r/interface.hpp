#ifndef INTERFACE_HPP
#define INTERFACE_HPP

namespace Hierarchy {

class Interface {
public:
   virtual int pv_number() const = 0;
   virtual char pv_char() const = 0;
   virtual void pv_print_number() const = 0;
   virtual void pv_print_char() const = 0;
   virtual ~Interface();
};

}


#endif
