#ifndef ABSTRACT_HPP
#define ABSTRACT_HPP

#include "interface.hpp"

#include <string>

using std::string;
using std::cerr;

namespace Hierarchy {

class Abstract : public Interface {
private:
   static const int ABSTRACT = 0;
   
   double x = ABSTRACT;
   
public:
   static const string class_name;
   
   virtual double pv_Y() const = 0;
   virtual void pv_print_Y() const = 0;
   
   static void validate(const double, const string &);
   
   double X() const { return x; };
   virtual void virt_set_X (const double x);
   virtual void virt_print_X() const final;
   
   virtual double virt_area() const;
   void print_virt_area() const;
   //void print_virt_area() const final;

   int number() const { return ABSTRACT; };
   void print_number() const;
   /*
   int number() { return ABSTRACT; };
   void print_number();*/
   
   int pv_number() const override { return ABSTRACT; };
   void pv_print_number() const override final;
   
   virtual ~Abstract();
protected:
   Abstract(const double);
};

}


#endif
