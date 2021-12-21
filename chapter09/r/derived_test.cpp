#include "derived_test.hpp"
#include "print.hpp"
#include "utility.hpp"
#include "derived.hpp"

#include <functional>

using namespace std::placeholders;
using std::function;
using std::numeric_limits;

using std::bind;

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
#endif
   
namespace tests {

using namespace Hierarchy;

static Result_codes test_derived_linking(Derived & d) {
   print_and_assert(d.X(), 4.0, "x", __func__);
   print_and_assert(d.pv_Y(), 101.0, "y", __func__);
   print_and_assert(d.Z(), 7.0, "z", __func__);
   print_and_assert(d.pv_char(), Derived::DERIVED_CHAR, "DERIVED_CHAR", __func__);
   print_and_assert(d.number(), Derived::DERIVED, "number", __func__);

   Result_codes result = bind_execute_member_function_assert(d, &Derived::X, 7.5, "x", __func__, &Derived::virt_set_X, 7.5);
   if (OK == result)
      result = incorrect_call(d, &Derived::X, numeric_limits<double>::infinity(), "x", __func__, &Derived::virt_set_X, numeric_limits<double>::infinity());
   if (OK == result) {
      print_and_assert(d.X(), 7.5, "x", __func__);
      result = bind_execute_member_function_assert(d, &Derived::pv_Y, 0.8, "y", __func__, &Derived::virt_set_Y, 0.8);
   }
   if (OK == result)
         result = incorrect_call(d, &Derived::pv_Y, -105.8, "y", __func__, &Derived::virt_set_Y, -105.8); 
   if (OK == result) {
      print_and_assert(d.pv_Y(), 0.8, "y", __func__);
      result = bind_execute_member_function_assert(d, &Derived::Z, -66.8, "z", __func__, &Derived::virt_set_Z, -66.8);
   }
   if (OK == result)
         result = incorrect_call(d, &Derived::Z, -0.0, "z", __func__, &Derived::virt_set_Z, -0.0); 
   if (OK == result)
         result = incorrect_call(d, &Derived::Z, 190.0, "z", __func__, &Derived::virt_set_Z, 190.0);
   if (OK == result) {
      print_and_assert(d.Z(), -66.8, "z", __func__);
      print_and_assert(d.virt_area(), 7.5 * 0.8 * -66.8, "virt_area", __func__);
   }
   assert_many(result == OK, "result == ", result);
   return result;
}

#ifdef MANUAL_DLL_LOAD
typedef struct Manual_DLL_interface {
   Derived * (*create)(const double, const double, const double);
   void (*destroy)(Derived * & );
   void * handle;
} Manual_DLL_interface;

static void load_derived(Manual_DLL_interface & derived_functions) {
   derived_functions.handle   = get_handle("libderived_cpp.so", RTLD_LAZY);
   derived_functions.create   = reinterpret_cast<Derived * (*)(const double, const double, const double)> (get_symbol(derived_functions.handle, "derived_create"));
   derived_functions.destroy  = reinterpret_cast<void (*)(Derived * & )> (get_symbol(derived_functions.handle, "derived_destroy"));
}

static Derived * test_constructor(Derived * (*create)(const double, const double, const double)) {
   Derived * d = create(4, -3, 7); 
   assert_many(d == nullptr, "d pointer == ", d);
   d = create(4, 55, 107); 
   assert_many(d == nullptr, "d pointer == ", d);
   d = create(4, 101, 7); 
   assert_many(d != nullptr, "d pointer == ", d);
   return d;
}

static Result_codes test_derived_linking() {
   cerr << "\nMANUAL DLL LOAD\n";
   Manual_DLL_interface derived_functions;
   load_derived(derived_functions);
   Derived * d = test_constructor(derived_functions.create);
   Result_codes result = (nullptr == d) ? INCORRECT_VALUE : OK;
   if (result == OK) {
         result = test_derived_linking(*d);
      if (result == OK)
         result = static_cast<Result_codes> (close_handle(&(derived_functions.handle)));
      else
         close_handle(&(derived_functions.handle));
      derived_functions.destroy(d);
   }
   assert_many(result == OK, "result == ", result);
   assert_many(d == nullptr, "d pointer == ", d);
   return result;
}
#else
static Result_codes test_constructor(Derived & d) {
   Result_codes result = init<Derived>(d, Constructor<Derived>(), 4, -3, 7); 
   assert_many(result == INVALID_ARG, "result == ", result);
   result = init<Derived>(d, Constructor<Derived>(), 4, 55, 107);
   assert_many(result == INVALID_ARG, "result == ", result);
   result = init<Derived>(d, Constructor<Derived>(), 4, 101, 7);
   assert_many(result == OK, "result == ", result);
   return result;
}

static Result_codes test_derived_linking() {
   cerr << "\nAUTOMATIC DLL LOAD\n";
   Derived d;
   Result_codes result = test_constructor(d);
   assert_many(result == OK, "result == ", result);
   if (result == OK)
      result = test_derived_linking(d);
   assert_many(result == OK, "result == ", result);
   return result;
}
#endif

Result_codes test_derived() {
   function<Result_codes()> test = bind(static_cast<Result_codes(*)()> (&test_derived_linking));
   Result_codes result = call_catch_exception(test);
   return result; 
}

}
