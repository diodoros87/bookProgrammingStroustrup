#ifndef DERIVED_HPP
#define DERIVED_HPP

#include "base.hpp"

namespace Hierarchy {

class Derived : public Base {
private:
   static const int DERIVED = 1;
   static const char DERIVED_CHAR = 'B';
   
   double z = DERIVED;
public:
   static const string class_name;
   static void validate(const double, const string &);
   
   virtual void virt_set_Z (const double z);
   virtual double Z() const final { return z; };
   virtual void virt_print_Z() const final;
   
   virtual void virt_set_X (const double x) override final;
   virtual void virt_set_Y (const double ) override ;
   
   virtual void pv_print_Y() const override;
   
   //virtual double virt_Z() const final { return z; };
   //virtual void virt_set_Z (const double ) override final;
   //virtual void pv_print_Y() const override final;
   
   virtual void virt_print_number() const final;
   
   //virtual double virt_area() const;
   virtual double virt_area() const override;

   int number() const { return DERIVED; };
   void print_number() const;
   
   int pv_number() const override { return DERIVED; };
   
   virtual char pv_char() const { return DERIVED_CHAR; };
   virtual void pv_print_char() const;
   
   ~Derived();
   Derived(const double, const double, const double);
};

}


#endif
