#ifndef DERIVED_TEST_HPP
#define DERIVED_TEST_HPP

#include "result_codes.h"

namespace tests {
   
using Hierarchy::Derived;
   
#ifdef MANUAL_DLL_LOAD
typedef struct Manual_DLL_derived {
   Derived* (*create_derived) (const double, const double, const double);
   void (*destroy) (Derived * & );
   mutable void * handle;
   Manual_DLL_interface_derived(const char * shared_library, const char * create, const char *  destroy) {
      handle   = get_handle(const_cast<char *> (shared_library), RTLD_LAZY);
      this->create_derived   = reinterpret_cast<Base * (*)(const double, const double, const double)> (get_symbol(handle, const_cast<char *> (create)));
      this->destroy  = reinterpret_cast<void (*)(Base * & )> (get_symbol(handle, const_cast<char *> (destroy)));
   }
} Manual_DLL_derived;
#endif

typedef struct Derived_expected : public Base_expected {
   void set(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
      Interface_expected::set(pv_n, pv_c);
      X.second = x;
      pv_Y.second = pv_y;
      virt_area.second = area;
      number.second = n;
   }
   pair<string, double> X = {"x", 0};
   pair<string, double> pv_Y = {"y", 0};
   pair<string, double> virt_area = {"virt_area", 0};
   pair<string, int> number = {"number", 0};
} Derived_expected;
   
struct Derived_test : public Base_expected {
private:
#ifdef MANUAL_DLL_LOAD
   static const Manual_DLL_derived manual_interface;
   static Base * base;
#else
   static Base base;
#endif
   static Interface_expected interface_expected;
   static Abstract_expected abstract_expected;
   static Base_expected base_expected;
   static Interface_real interface_real;
   static Abstract_real abstract_real;
   static Base_real base_real;
public:
   static Result_codes test_base_linking(); 
private:
#ifdef MANUAL_DLL_LOAD
   static Result_codes load_base();
#endif
   static void test_interface(const Interface_expected & expected, const Interface_real & real);
   static void test_interface(int n, char c);
   static void test_abstract(const Abstract_expected & expected, const Abstract_real & real);
   static void test_abstract(int pv_n, char pv_c, double x, double pv_y, double area, int n);
   static void test_base(const Base_expected & expected, const Base_real & real);
   static void test_base(int pv_n, char pv_c, double x, double pv_y, double area, int n);
};

Result_codes test_derived();

}

#endif
