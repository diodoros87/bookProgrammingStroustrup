#ifndef BASE_HPP
#define BASE_HPP

#include "abstract.hpp"

namespace Hierarchy {

class Base : public Abstract {
private:
   static const int BASE = 1;
   static const char BASE_CHAR = 'B';
   
   double y = BASE;
   
public:
   static const string class_name;
   
   virtual double pv_Y() const override final { return y; };
   virtual void virt_set_Y (const double );
   virtual void pv_print_Y() const override;
   
   virtual void virt_print_number() const;
   
   //virtual double virt_area() const;
   virtual double virt_area() const override;

   int number() const { return BASE; };
   void print_number() const;
   
   int pv_number() const override { return BASE; };
   
   virtual char pv_char() const { return BASE_CHAR; };
   virtual void pv_print_char() const;
   
   ~Base();
   Base(const double, const double);
protected:
   
};

}


#endif
