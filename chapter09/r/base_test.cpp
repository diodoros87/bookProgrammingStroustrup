#include "base_test.hpp"
#include "print.hpp"
#include "utility.hpp"
#include "base.hpp"

#include "connector.h"

#include <functional>

using namespace std::placeholders;
using std::function;

using std::bind;

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   //#include "base_functions.h"
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
   if (nullptr == b) {
      close_handle(&(base_functions.handle));
      return INVALID_ARG;
   }
   print_and_assert(b->X(), 4.0, "x", __func__);
   print_and_assert(b->pv_Y(), 8.0, "y", __func__);
   //cerr << TIE( "C++", unmove(__cplusplus), __func__, " base's name = ", x) << '\n';
   //assert_many(x == 4, "x == ", x);
   //Result_codes result = bind_execute_member_function(b, &Base::virt_set_X, 7.5);
   Result_codes result = bind_execute_member_function_assert(b, &Base::X, 7.5, "x", __func__, &Base::virt_set_X, 7.5);
   if (OK == result)
      result = bind_execute_member_function_assert(b, &Base::pv_Y, -9.8, "y", __func__, &Base::virt_set_Y, -9.8);
   //print_and_assert(b->X(), 7.5, "x", __func__);
   name = b->get_name();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, " b name = ", name) << '\n';
   assert_many(name == "Socrates", "name == ", name);
   base_functions.destroy(b);
   result = static_cast<Result_codes> (close_handle(&(base_functions.handle)));
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
   
   Result_codes result = bind_execute_member_function_assert(b, &Base::X, 7.5, "x", __func__, &Base::virt_set_X, 7.5);
   if (OK == result)
      result = bind_execute_member_function_assert(b, &Base::pv_Y, -9.8, "y", __func__, &Base::virt_set_Y, -9.8);
   /*
   string name = b.get_name();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, name) << '\n';
   assert_many(name == "Leibniz", "name == ", name);
   function<void(const string&)> set = bind(&Base::set_name, std::ref(b), _1);
   Result_codes result = call_catch_exception(set, "Descartes");
   name = b.get_name();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, name) << '\n';
   assert_many(name == "Descartes", "name == ", name);
   */
   return result;
}
#endif

Result_codes test_base() {
   function<Result_codes()> test = bind(&test_base_linking);
   Result_codes result = call_catch_exception(test);
   return result; 
}

}
