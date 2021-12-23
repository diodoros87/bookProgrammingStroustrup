#ifndef BASE_TEST_HPP
#define BASE_TEST_HPP

#include "result_codes.h"
#include "base.hpp"

#include <string>

using std::pair;
using std::string;

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
#endif

namespace tests {
   
using Hierarchy::Base;
   
#ifdef MANUAL_DLL_LOAD
typedef struct Manual_DLL_interface {
   Base* (*create) (const double, const double);
   void (*destroy) (Base * & );
   mutable void * handle;
   Manual_DLL_interface(const char * shared_library, const char * create, const char *  destroy) {
      handle   = get_handle(const_cast<char *> (shared_library), RTLD_LAZY);
      this->create   = reinterpret_cast<Base * (*)(const double, const double)> (get_symbol(handle, const_cast<char *> (create)));
      this->destroy  = reinterpret_cast<void (*)(Base * & )> (get_symbol(handle, const_cast<char *> (destroy)));
   }
} Manual_DLL_interface;
#endif
   
struct Interface_expected;
struct Abstract_expected;
struct Base_expected;
struct Interface_real;
struct Abstract_real;
struct Base_real;
   
struct Base_test {
private:
#ifdef MANUAL_DLL_LOAD
   static const Manual_DLL_interface manual_interface;
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
   
typedef struct Interface_expected {
   void set(int n, char c) {
      pv_number.second = n;
      pv_char.second = c;
   }
   pair<string, int> pv_number = {"pv_number", 0};
   pair<string, char> pv_char = {"pv_char", '0'};
} Interface_expected;

typedef struct Abstract_expected : public Interface_expected {
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
} Abstract_expected;

typedef struct Base_expected : public Abstract_expected {
} Base_expected;

typedef struct Interface_real {
   void set(int n, char c) {
      pv_number = n;
      pv_char = c;
   }
   int pv_number;
   char pv_char;
} Interface_real;

typedef struct Abstract_real : public Interface_real {
   void set(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
      Interface_real::set(pv_n, pv_c);
      X = x;
      pv_Y = pv_y;
      virt_area = area;
      number = n;
   }
   double X;
   double pv_Y;
   double virt_area;
   int number;
} Abstract_real;

typedef struct Base_real : public Abstract_real {
} Base_real;

Result_codes test_base();

}

#endif
