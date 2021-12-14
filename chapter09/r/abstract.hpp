#ifndef ABSTRACT_HPP
#define ABSTRACT_HPP

#include "interface.hpp"
#include "print.hpp"

#include <string>

using std::string;
using std::cerr;

namespace Hierarchy {

class Abstract : public Interface {
private:
   static const int NR = 0;
   
   const double x1 = 3;
   
public:
   static const string class_name = typeid(person).name();
   
   virtual void pv_abstract_print() const = 0;
   virtual double pv_abstract_Y() const = 0;
   
   static void validate(const double x);
   
   virtual int virt_get_X() const { return x1; };
   virtual int virt_set_X(const double x) const { return x1; };
   virtual int virt_print_X() const;
   
   void print_area() const;
   double area() const;
   
   int fin_abstract_number() const final { return NR; };
   void fin_abstract_print() const final;
   
   int pv_number() const override { return NR; };
   void pv_print() const override { cerr << TIE( "C++", unmove(__cplusplus), __func__, " Abstract ", pv_number()) << '\n'; };
   
   ~Abstract();
protected:
   Abstract(const double);
};

}


#endif
