#include "singleton.h"
#include "print.h"
#include "utility.h"
#include "hierarchy_test.h"

SINGLETON_STACK(Interface_expected, { {"pv_number", 0}, {"pv_char", '0'} });

static Interface_expected * interface_expected = NULL;

void set_interface_expected(Interface_expected * interface, const int n, const char c) {
   REQUIRE_NON_NULL(interface, "interface is null");
   pv_number.number = n;
   pv_char.ch = c;
}

SINGLETON_HEAP(Abstract_expected, { {"pv_number", 0}, {"pv_char", '0'} });

void set_abstract_expected(Abstract_expected * abstract, int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   REQUIRE_NON_NULL(abstract, "abstract is null");
   set_interface_expected((Interface_expected *)abstract, pv_n, pv_c);
   X.number = x;
   pv_Y.number = pv_y;
   virt_area.number = area;
   number.number = n;
}

SINGLETON_HEAP(Base_expected, { {"pv_number", 0}, {"pv_char", '0'} });

void set_base_expected(Base_expected * base, int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   REQUIRE_NON_NULL(abstract, "abstract is null");
   set_abstract_expected((Abstract_expected *)base, pv_n, pv_c);
}
