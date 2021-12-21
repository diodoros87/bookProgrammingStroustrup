#ifndef BASE_TEST_HPP
#define BASE_TEST_HPP

#include "result_codes.h"

#include <string>
#include <map>
#include <vector>

using std::pair;
using std::map;
using std::string;
using std::vector;

namespace tests {
   
typedef struct Interface_functions {
   void set(int n, char c) {
      pv_number.second = n;
      pv_char.second = c;
   }
   pair<string, int> pv_number = {"pv_number", 0};
   pair<string, char> pv_char = {"pv_char", '0'};
} Interface_functions;

typedef struct Abstract_functions : public Interface_functions {
   void set(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
      Interface_functions::set(pv_n, pv_c);
      X.second = x;
      pv_Y.second = pv_y;
      virt_area.second = area;
      number.second = n;
   }
   pair<string, double> X = {"X", 0};
   pair<string, double> pv_Y = {"pv_Y", 0};
   pair<string, double> virt_area = {"virt_area", 0};
   pair<string, int> number = {"number", 0};
} Abstract_functions;

typedef struct Base_functions : public Abstract_functions {
} Base_functions;

#ifdef MANUAL_DLL_LOAD
typedef struct Manual_DLL_interface {
   const Base (*create)  const (const double, const double);
   const void (*destroy) const (Base * & );
   const void * const handle;
} Manual_DLL_interface;
#endif

Result_codes test_base();

}

#endif
