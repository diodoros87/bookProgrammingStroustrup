#ifndef ABSTRACT_H
#define ABSTRACT_H

#include "interface.h"

struct Abstract_t;

typedef struct Abstract_t Abstract_t;

#ifdef __cplusplus
extern "C" {
#endif

void Abstract_destroy(Abstract_t ** const);

Result_codes X(const Abstract_t * const, double * const);

Result_codes virt_set_X(const Abstract_t * const, const double);

Result_codes pv_Y(const Abstract_t * const, double * const);

Result_codes validate(const double, const char * const);

Result_codes virt_area(const Abstract_t * const, double * const);

int number(const Abstract_t * const);

#ifdef  __cplusplus
}
#endif

extern const int ABSTRACT;
extern const char * const class_name;

class Abstract {
private:
   double x {ABSTRACT};
public:
   static const int ABSTRACT = 0;
   static const string class_name;
   //static string get_class_name() { return class_name; }
   
   virtual double pv_Y() const = 0;
   virtual void pv_print_Y() const = 0;
   
   static void validate(const double, const string &);
   
   virtual double X() const final { return x; };
   virtual void virt_set_X (const double x);
   virtual void virt_print_X() const final;
   
   virtual double virt_area() const;
   //virtual void print_virt_area() const ;
   virtual void print_virt_area() const final;

   int number() const { return ABSTRACT; };
   void print_number() const;
   /*
   int number() { return ABSTRACT; };
   void print_number();*/
   
   int pv_number() const override { return ABSTRACT; };
   void pv_print_number() const override final;
   
   virtual ~Abstract() ;
   Abstract(Abstract &&) noexcept;
   Abstract& operator=(Abstract &&) noexcept;
protected:
   Abstract(const double);
   Abstract() { cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__) << '\n'; }
   Abstract(Abstract const &);
   Abstract& operator=(Abstract const &);
   
};


#endif
