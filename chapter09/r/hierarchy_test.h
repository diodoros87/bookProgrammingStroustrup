#ifndef HIERARCHY_TEST_H 
#define HIERARCHY_TEST_H

#include "result_codes.h"

typedef struct Interface_expected {
   pair_int_str pv_number = {"pv_number", 0};
   pair_char_str pv_char = {"pv_char", '0'};
} Interface_expected;

void set_interface_expected(Interface_expected * interface, const int n, const char c);

typedef struct Abstract_expected : public Interface_expected {
   void set(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
      Interface_expected::set(pv_n, pv_c);
      X.second = x;
      pv_Y.second = pv_y;
      virt_area.second = area;
      number.second = n;
   }
   pair_double_str X = {"x", 0};
   pair_double_str pv_Y = {"y", 0};
   pair_double_str virt_area = {"virt_area", 0};
   pair_int_str number = {"number", 0};
} Abstract_expected;

void set_abstract_expected(Abstract_expected * abstract, int pv_n, char pv_c, double x, double pv_y, double area, int n);

typedef struct Base_expected : public Abstract_expected {
} Base_expected;

void set_base_expected(Base_expected * base, int pv_n, char pv_c, double x, double pv_y, double area, int n);

typedef struct Interface_real {
   int pv_number;
   char pv_char;
} Interface_real;

typedef struct Abstract_real : public Interface_real {
   Interface_real interface;
   double X;
   double pv_Y;
   double virt_area;
   int number;
} Abstract_real;

typedef struct Base_real : public Abstract_real {
} Base_real;
