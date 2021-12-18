#include "base_test.hpp"
#include "print.hpp"
#include "utility.hpp"
#include "base.hpp"

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
   Base * (*create)(const double, const double);
   void (*destroy)(Base * & );
   void * handle;
} Manual_DLL_interface;

static void load_base(Manual_DLL_interface & base_functions) {
   base_functions.handle   = get_handle("libbase_cpp.so", RTLD_LAZY);
   base_functions.create   = reinterpret_cast<Base * (*)(const double, const double)> (get_symbol(base_functions.handle, "base_create"));
   base_functions.destroy  = reinterpret_cast<void (*)(Base * & )> (get_symbol(base_functions.handle, "base_destroy"));
}

static Result_codes test_base_linking() {
   cerr << "\nMANUAL DLL LOAD\n";
   Manual_DLL_interface base_functions;
   load_base(base_functions);
   Base * b = base_functions.create(4, 8); 
   Result_codes result = OK;
   if (nullptr == b)
      result = INCORRECT_VALUE;
   if (result == OK) {
      print_and_assert(b->X(), 4.0, "x", __func__);
      print_and_assert(b->pv_Y(), 8.0, "y", __func__);
      print_and_assert(b->pv_char(), Base::BASE_CHAR, "BASE_CHAR", __func__);
      print_and_assert(b->number(), Base::BASE, "number", __func__);
      
      result = bind_execute_member_function_assert(b, &Base::X, 7.5, "x", __func__, &Base::virt_set_X, 7.5);
      if (OK == result)
         result = bind_execute_member_function_assert(b, &Base::pv_Y, -9.8, "y", __func__, &Base::virt_set_Y, -9.8);
      if (INVALID_ARG == result)
         result = OK;
      else
         assert_many(result != INVALID_ARG, "result == ", result);
      
      if (OK == result)
         result = bind_execute_member_function_assert(b, &Base::pv_Y, 105.8, "y", __func__, &Base::virt_set_Y, 105.8); 
      if (OK == result) {
         print_and_assert(b->virt_area(), 7.5 * 105.8, "virt_area", __func__);
         result = static_cast<Result_codes> (close_handle(&(base_functions.handle)));
      }
   }
   base_functions.destroy(b);
   if (result != OK)
      close_handle(&(base_functions.handle));
   assert_many(result == OK, "result == ", result);
   assert_many(b == nullptr, "b pointer == ", b);
   return result;
}
#else
static Result_codes test_base_linking() {
   cerr << "\nAUTOMATIC DLL LOAD\n";
   Base b = {-6, 0};
   print_and_assert(b.X(), -6.0, "x", __func__);
   print_and_assert(b.pv_Y(), 0.0, "y", __func__);
   print_and_assert(b.pv_char(), Base::BASE_CHAR, "BASE_CHAR", __func__);
   print_and_assert(b.number(), Base::BASE, "number", __func__);

   Result_codes result = bind_execute_member_function_assert(b, &Base::X, 7.5, "x", __func__, &Base::virt_set_X, 7.5);
   if (OK == result)
      result = bind_execute_member_function_assert(b, &Base::pv_Y, numeric_limits<double>::infinity(), "y", __func__, &Base::virt_set_Y, numeric_limits<double>::infinity());
   if (INVALID_ARG == result)
      result = OK;
   else
      assert_many(result != INVALID_ARG, "result == ", result);
   
   if (OK == result)
      result = bind_execute_member_function_assert(b, &Base::pv_Y, 105.8, "y", __func__, &Base::virt_set_Y, 105.8);
   if (OK == result)
      print_and_assert(b.virt_area(), 7.5 * 105.8, "virt_area", __func__);
   
   assert_many(result == OK, "result == ", result);
   return result;
}
#endif

Result_codes test_base() {
   function<Result_codes()> test = bind(&test_base_linking);
   Result_codes result = call_catch_exception(test);
   return result; 
}

}
