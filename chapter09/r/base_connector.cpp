#include "base_connector.h"
#include "utility.hpp"
#include "null.hpp"

using namespace Hierarchy;
using namespace std;

//extern Result_codes demo_set_name(Demo * const object, const char * name);
//extern Result_codes demo_get_name(const Demo * const object, char ** name);

template<>
Base * Base_connector::Abstract_connector<Base>::instance = nullptr;

static Base_connector connector;

Result_codes Base_connector::init(const double x, const double y) {
   Result_codes result = ::init<Base>(Abstract_connector<Base>::instance, Constructor<Base>(), x, y);
   return result;
}

Result_codes Base_connector::pv_X(double * const x) {
   if (OK != check_pointer(x, __func__, " Error number pointer")) 
      return INVALID_ARG;
   *x = instance->X();
   return OK;
}

Result_codes base_cpp_init(const double x, const double y) {
   Result_codes result = connector.init(x, y);
   return result;
}

Result_codes base_cpp_pv_number(int * const n) {
   Result_codes result = (&connector)->pv_number(n);
   return result;
}

Result_codes base_cpp_pv_char(char * const c) {
   Result_codes result = connector.pv_char(c);
   return result;
}

Result_codes base_cpp_pv_X(double  * const  x) {
   Result_codes result = connector.pv_X(x);
   return result;
}

Result_codes base_cpp_virt_set_X(const double x) {
   Result_codes result = connector.virt_set_X(x);
   return result;
}

Result_codes base_cpp_pv_Y(double * const y) {
   Result_codes result = connector.pv_Y(y);
   return result;
}


Result_codes base_cpp_virt_set_Y(const double y) {
   Result_codes result = connector.virt_set_Y(y);
   return result;
}

Result_codes base_cpp_virt_area(double * const area) {
   Result_codes result = connector.virt_area(area);
   return result;
}

Result_codes base_cpp_number(int * const number) {
   Result_codes result = connector.number(number);
   return result;
}

Result_codes base_cpp_destroy() {
   Result_codes result = connector.destruct();
   return result;
}
   
// Result_codes demo_derived_set_name(const char * name) {
//    Result_codes result = Base_connector::set_name(name);
//    return result;
// }
// 
// Result_codes demo_derived_get_name(char ** name) {
//    Result_codes result = Base_connector::get_name(name);
//    return result;
// }
// 
// Result_codes demo_derived_set_age(const unsigned int age) {
//    Result_codes result = Base_connector::set_age(age);
//    return result;
// }
// 
// Result_codes demo_derived_get_age(unsigned int * const age) {
//    Result_codes result = Base_connector::get_age(age);
//    return result;
// }
// 
// Result_codes demo_derived_destroy(void) {
//    Result_codes result = Base_connector::destruct();
//    return result;
// }
