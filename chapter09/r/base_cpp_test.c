#include "singleton.h"
#include "print.h"
#include "utility.h"
#include "base_cpp_test.h"
#include "base_connector.h"
#include "hierarchy_test.h"

#include <string.h>

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_BASE_CONNECTOR_SO     "libbase_cpp_connector.so"
#endif

static Interface_expected * interface_expected = NULL;
SINGLETON_STACK(Interface_expected, ( { {"pv_number", 0}, {"pv_char", '0'} } ));

static Interface_expected * abstract_expected = NULL;
SINGLETON_STACK(Abstract_expected, { { {"pv_number", 0}, {"pv_char", '0'} },
                {"x", 0}, {"y", 0}, {"virt_area", 0}, {"number", 0} });

static Interface_expected * base_expected = NULL;
SINGLETON_STACK(Base_expected, { { { {"pv_number", 0}, {"pv_char", '0'} },
                {"x", 0}, {"y", 0}, {"virt_area", 0}, {"number", 0} } });

static Derived_expected * derived_expected = NULL;
SINGLETON_STACK(Derived_expected, { { { { {"pv_number", 0}, {"pv_char", '0'} },
                {"x", 0}, {"y", 0}, {"virt_area", 0}, {"number", 0} } }, 
   {"z", 0}
});

static Interface_real * interface_real = NULL;
SINGLETON_STACK(Interface_real, { 0, '0'});

static Abstract_real * abstract_real = NULL;
SINGLETON_STACK(Abstract_real, { { 0, '0'}, 0, 0, 0, 0 });

static Base_real * base_real = NULL;
SINGLETON_STACK(Base_real, { { { 0, '0'}, 0, 0, 0, 0 } });

static Derived_real * derived_real = NULL;
SINGLETON_STACK(Derived_real, { { { { 0, '0'}, 0, 0, 0, 0 } }, 0 });

static void load_base_connector(Base_connector * functions) {
   if (! functions) {
      LOG_EXIT(__FUNCTION__, "functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
   functions->handle = get_handle(LIB_BASE_CONNECTOR_SO, RTLD_LAZY);
   functions->init = get_symbol(functions->handle, "base_cpp_init");
   functions->destroy = get_symbol(functions->handle, "base_cpp_destroy");
   functions->abstract.interface.pv_number = get_symbol(functions->handle, "base_cpp_pv_number");
   functions->abstract.interface.pv_char = get_symbol(functions->handle, "base_cpp_pv_char");
   functions->abstract.pv_X = get_symbol(functions->handle, "base_cpp_pv_X");
   functions->abstract.virt_set_X = get_symbol(functions->handle, "base_cpp_virt_set_X");
   functions->abstract.pv_Y = get_symbol(functions->handle, "base_cpp_pv_Y");
   functions->abstract.virt_set_Y = get_symbol(functions->handle, "base_cpp_virt_set_Y");
   functions->abstract.virt_area = get_symbol(functions->handle, "base_cpp_virt_area");
   functions->abstract.number = get_symbol(functions->handle, "base_cpp_number");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
   functions->handle = NULL;
   functions->init = base_cpp_init;
   functions->destroy = base_cpp_destroy;
   functions->abstract.interface.pv_number = base_cpp_pv_number;
   functions->abstract.interface.pv_char = base_cpp_pv_char;
   functions->abstract.pv_X = base_cpp_pv_X;
   functions->abstract.virt_set_X = base_cpp_virt_set_X;
   functions->abstract.pv_Y = base_cpp_pv_Y;
   functions->abstract.virt_set_Y = base_cpp_virt_set_Y;
   functions->abstract.virt_area = base_cpp_virt_area;
   functions->abstract.number = base_cpp_number;
#endif
}

// static Result_codes check_name(Base_connector * functions, const char * const expected_name) {
//    REQUIRE_NON_NULL(functions, "functions is null");
//    REQUIRE_NON_NULL(expected_name, "expected_name is null");
//    char * name = NULL;
//    Result_codes result = functions->get_name(&name);
//    if (OK == result) {
//       LOG("%s: %s base_connector name = %s", LANGUAGE, __FUNCTION__, name);
//       assert_many(strcmp(name, expected_name) == 0, "assert failed: ", "s s", "name == ", name);
//    }
//    return result;
// }
// 
// static Result_codes check_age(Base_connector * functions, const unsigned int expected) {
//    REQUIRE_NON_NULL(functions, "functions is null");
//    unsigned int age;
//    Result_codes result = functions->get_age(&age);
//    if (OK == result) {
//       LOG("%s: %s base_connector age = %u", LANGUAGE, __FUNCTION__, age);
//       assert_many(age == expected, "assert failed: ", "s u", " age == ", age);
//    }
//    return result;
// }

static Result_codes run_base_connector(const Base_connector * const functions) { 
   if (! functions) {
      LOG_EXIT(__FUNCTION__, "base_connector functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
   Result_codes result = functions->init(5, 8); 
   double x;
   if (OK == result)
      result = functions->abstract.pv_X(&x); 
   LOG("%s: %s base_connector x = %f\n", LANGUAGE, __FUNCTION__, x);
   if (OK == result)
      result = functions->abstract.virt_set_X(7); 
   //LOG("%s: %s base_connector y = %f\n", LANGUAGE, __FUNCTION__, x);
   if (OK == result)
      result = functions->abstract.pv_X(&x); 
   LOG("%s: %s base_connector y = %f\n", LANGUAGE, __FUNCTION__, x);
   if (OK == result)
      result = functions->abstract.pv_Y(&x); 
   LOG("%s: %s base_connector y = %f\n", LANGUAGE, __FUNCTION__, x);
//    if (OK == result)
//       result = check_name(functions, "Claudius Ptolemaeus"); 
//    if (OK == result)
//       result = check_age(functions, 9); 
//    if (OK == result)
//       result = functions->set_name("Plato");
//    if (OK == result)
//       result = check_name(functions, "Plato"); 
//    if (OK == result)
//       result = functions->set_age(84);
//    if (OK == result)
//       result = check_age(functions, 84); 
// #ifdef MANUAL_DLL_LOAD
//    if (OK == result) {
//       functions->destroy();
//       result = close_handle(&(functions->handle));
//       assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
//       return result;
//    }
//    close_handle(&(functions->handle));
// #endif
   functions->destroy();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

Result_codes test_base_cpp(void) {
   FUNCTION_INFO(__FUNCTION__);
   Base_connector functions;
   load_base_connector(&functions); 
   Result_codes result = run_base_connector(&functions);
   return result;
}
