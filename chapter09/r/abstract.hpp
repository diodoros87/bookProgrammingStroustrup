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
   
   double x = 3;
   
public:
   static const string class_name;
   
   virtual double pv_Y() const = 0;
   virtual void pv_print_Y() const = 0;
   
   static void validate(const double, const string &);
   
   virtual double virt_get_X() const final { return x; };
   virtual void virt_set_X (const double x) final;
   virtual void virt_print_X() const final;
   
   virtual double virt_area() const;
   virtual void virt_print_area() const;

   int number() const { return NR; };
   void print_number() const;
   
   int number() { return NR; };
   
   
   //int fin_abstract_number() const final { return NR; };
   //void fin_abstract_print() const final;
   
   int pv_number() const override { return NR; };
   void pv_print_number() const override final;
   
   ~Abstract();
protected:
   Abstract(const double);
};

}


#endif
