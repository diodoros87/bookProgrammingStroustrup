#ifndef ABSTRACT_HPP
#define ABSTRACT_HPP

#include "print.hpp"
#include <string>

using std::string;
using std::cerr;

namespace Hierarchy {

class Abstract : public Interface {
private:
   static int NR = 0;
   
   const double x1 = 3;
   
public:
   virtual void pv_abstract_print() const = 0;
   
   static void validate(const double x);
   
   virtual void virt_abstract_print() const;
   virtual double virt_area() const;
   double area() const;
   void fin_abstract_print() const final;
   
   int pv_number() const override { return NR; };
   void pv_print() const override { cerr << TIE( "C++", unmove(__cplusplus), __func__, " Abstract ", pv_number()) << '\n'; };
   void print() const { cerr << TIE( "C++", unmove(__cplusplus), __func__, " Abstract ", '\n'); }
   
   ~Abstract();
protected:
   Abstract(const double);
};

}


#endif
