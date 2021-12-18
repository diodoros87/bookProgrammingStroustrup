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

#ifdef MANUAL_DLL_LOAD
typedef struct Manual_DLL_interface {
   Derived * (*create)(const double, const double);
   void (*destroy)(Derived * & );
   void * handle;
} Manual_DLL_interface;

static void load_derived(Manual_DLL_interface & derived_functions) {
   derived_functions.handle   = get_handle("libderived_cpp.so", RTLD_LAZY);
   derived_functions.create   = reinterpret_cast<Derived * (*)(const double, const double)> (get_symbol(derived_functions.handle, "derived_create"));
   derived_functions.destroy  = reinterpret_cast<void (*)(Derived * & )> (get_symbol(derived_functions.handle, "derived_destroy"));
}

static Result_codes test_derived_linking() {
   cerr << "\nMANUAL DLL LOAD\n";
   Manual_DLL_interface derived_functions;
   load_derived(derived_functions);
   Derived * b = derived_functions.create(4, 8); 
   Result_codes result = OK;
   if (nullptr == b)
      result = INCORRECT_VALUE;
   if (result == OK) {
      print_and_assert(b->X(), 4.0, "x", __func__);
      print_and_assert(b->pv_Y(), 8.0, "y", __func__);
      print_and_assert(b->pv_char(), Derived::DERIVED_CHAR, "DERIVED_CHAR", __func__);
      print_and_assert(b->number(), Derived::DERIVED, "number", __func__);
      
      result = bind_execute_member_function_assert(b, &Derived::X, 7.5, "x", __func__, &Derived::virt_set_X, 7.5);
      if (OK == result)
         result = bind_execute_member_function_assert(b, &Derived::pv_Y, -9.8, "y", __func__, &Derived::virt_set_Y, -9.8);
      if (INVALID_ARG == result)
         result = OK;
      else
         assert_many(result != INVALID_ARG, "result == ", result);
      
      if (OK == result)
         result = bind_execute_member_function_assert(b, &Derived::pv_Y, 105.8, "y", __func__, &Derived::virt_set_Y, 105.8); 
      if (OK == result) {
         print_and_assert(b->virt_area(), 7.5 * 105.8, "virt_area", __func__);
         result = static_cast<Result_codes> (close_handle(&(derived_functions.handle)));
      }
   }
   derived_functions.destroy(b);
   if (result != OK)
      close_handle(&(derived_functions.handle));
   assert_many(result == OK, "result == ", result);
   assert_many(b == nullptr, "b pointer == ", b);
   return result;
}
#else
static Result_codes test_derived_linking() {
   cerr << "\nAUTOMATIC DLL LOAD\n";
   Derived b = {-6, 0};
   print_and_assert(b.X(), -6.0, "x", __func__);
   print_and_assert(b.pv_Y(), 0.0, "y", __func__);
   print_and_assert(b.pv_char(), Derived::DERIVED_CHAR, "DERIVED_CHAR", __func__);
   print_and_assert(b.number(), Derived::DERIVED, "number", __func__);

   Result_codes result = bind_execute_member_function_assert(b, &Derived::X, 7.5, "x", __func__, &Derived::virt_set_X, 7.5);
   if (OK == result)
      result = bind_execute_member_function_assert(b, &Derived::pv_Y, numeric_limits<double>::infinity(), "y", __func__, &Derived::virt_set_Y, numeric_limits<double>::infinity());
   if (INVALID_ARG == result)
      result = OK;
   else
      assert_many(result != INVALID_ARG, "result == ", result);
   
   if (OK == result)
      result = bind_execute_member_function_assert(b, &Derived::pv_Y, 105.8, "y", __func__, &Derived::virt_set_Y, 105.8);
   if (OK == result)
      print_and_assert(b.virt_area(), 7.5 * 105.8, "virt_area", __func__);
   
   assert_many(result == OK, "result == ", result);
   return result;
}
#endif

Result_codes test_derived() {
   function<Result_codes()> test = bind(&test_derived_linking);
   Result_codes result = call_catch_exception(test);
   return result; 
}

}
