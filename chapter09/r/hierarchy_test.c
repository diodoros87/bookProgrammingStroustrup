#include "singleton.h"
#include "print.h"
#include "utility.h"
#include "hierarchy_test.h"
/*   
   EXPECTED PART BELOW
*/
/*   */

extern Singleton_Stack_Interface_expected();
void set_interface_expected(Interface_expected * interface_expected, const int n, const char c) {
   //REQUIRE_NON_NULL(interface, "interface is null");
   INIT_SINGLETON(interface_expected, Singleton_Stack_Interface_expected)
   interface_expected->pv_number.number = n;
   interface_expected->pv_char.ch = c;
}
/*   */
extern Singleton_Stack_Abstract_expected();
void set_abstract_expected(Abstract_expected * abstract_expected, int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   //REQUIRE_NON_NULL(abstract, "abstract is null");
   INIT_SINGLETON(abstract_expected, Singleton_Stack_Abstract_expected)
   set_interface_expected(abstract_expected->interface, pv_n, pv_c);
   abstract_expected->X.number = x;
   abstract_expected->pv_Y.number = pv_y;
   abstract_expected->virt_area.number = area;
   abstract_expected->number.number = n;
}
/*   */
extern Singleton_Stack_Base_expected();
void set_base_expected(Base_expected * base_expected, int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   INIT_SINGLETON(base_expected, Singleton_Stack_Base_expected)
   set_abstract_expected(base_expected->abstract, pv_n, pv_c, x, pv_y, area, n);
}
/*   */

void set_derived_expected(Derived_expected * derived_expected, int pv_n, char pv_c, double x, double pv_y, double area, int n, double z) {
   INIT_SINGLETON(derived_expected, Singleton_Stack_Derived_expected)
   set_base_expected((Base_expected *)derived_expected, pv_n, pv_c, x, pv_y, area, n);
   Z->number = z;
}
/*   
   REAL PART BELOW
*/

void set_interface_real(Interface_real * interface_real, const int n, const char c) {
   //REQUIRE_NON_NULL(interface, "interface is null");
   INIT_SINGLETON(interface_real, Singleton_Stack_Interface_real)
   interface_real->pv_number = n;
   interface_real->pv_char = c;
}
/*   */

void set_abstract_real(Abstract_real * abstract_real, int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   INIT_SINGLETON(abstract_real, Singleton_Stack_Abstract_real)
   set_interface_real(abstract_real->interface, pv_n, pv_c);
   abstract_expected->X = x;
   abstract_expected->pv_Y = pv_y;
   abstract_expected->virt_area = area;
   abstract_expected->number = n;
}
/*   */

void set_base_real(Base_real * base_real, int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   INIT_SINGLETON(base_real, Singleton_Stack_Base_real)
   set_abstract_real(base_real->abstract, pv_n, pv_c, x, pv_y, area, n);
}
/*   */
void set_derived_real(Derived_real * derived_real, int pv_n, char pv_c, double x, double pv_y, double area, int n, double z) {
   INIT_SINGLETON(derived_real, Singleton_Stack_Derived_real)
   set_base_expected((Base_real *)derived_real, pv_n, pv_c, x, pv_y, area, n);
   Z = z;
}

void interface_print_and_assert(const Interface_expected * const expected, const Interface_real * const real) {
   print_and_assert(real->pv_number, expected->pv_number.second, expected->pv_number.first, "%d", "s d");
   print_and_assert(real->pv_char,expected->pv_char.second, expected->pv_char.first, "%c", "s c");
}

void abstract_print_and_assert(const Abstract_expected * const expected, const Abstract_real * const real) {
   interface_print_and_assert(expected, real);
   print_and_assert(real->X, expected->X.second, expected->X.first);
   print_and_assert(real->pv_Y, expected->pv_Y.second, expected->pv_Y.first);
   print_and_assert(real->virt_area, expected->virt_area.second, expected->virt_area.first);
   print_and_assert(real->number, expected->number.second, expected->number.first);
}

void base_print_and_assert(const Base_expected * const expected, const Base_real * const real) {
   abstract_print_and_assert((Abstract_expected *)expected, (Abstract_real *)real);
}

void derived_print_and_assert(const Derived_expected * const expected, const Derived_real * const real) {
   base_print_and_assert((Base_expected *)expected, (Base_real *)real);
   print_and_assert(real->Z, expected->Z.second, expected->Z.first);
}
