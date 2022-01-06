#include "base_c_test.hpp"
#include "print.hpp"
#include "utility.hpp"

#include <functional>

using namespace std::placeholders;
using std::function;
using std::numeric_limits;
using std::bind;

#include "base.h"

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_BASE_SO     "libbase_c.so"
#endif

namespace tests {
   
void Base_C_test::load_base_functions() {
#ifdef MANUAL_DLL_LOAD
   cerr << "\nMANUAL DLL LOAD\n";
   functions.handle = get_handle(LIB_BASE_SO, RTLD_LAZY);
   functions.init = reinterpret_cast<Result_codes (*)(Base_t ** const, const double, const double)>
                           (get_symbol(functions.handle, "Base_init"));
   functions.Base_destroy = reinterpret_cast<void (*)(Base_t ** const)>
                           (get_symbol(functions.handle, "Base_destroy"));
   functions.abstract.interface.Interface_destroy = reinterpret_cast<void (*)(Interface_t ** const)>
                           (get_symbol(functions.handle, "Interface_destroy"));
   functions.abstract.interface.pv_number = reinterpret_cast<int (*)(const Interface_t * const)>
                           (get_symbol(functions.handle, "pv_number"));
   functions.abstract.interface.pv_char = reinterpret_cast<char (*)(const Interface_t * const)>
                           (get_symbol(functions.handle, "pv_char"));
   functions.abstract.X = reinterpret_cast<double (*)(const Abstract_t * const)>
                           (get_symbol(functions.handle, "X"));
   functions.abstract.virt_set_X = reinterpret_cast<Result_codes (*)(Abstract_t * const, const double)>
                           (get_symbol(functions.handle, "virt_set_X"));
   functions.abstract.pv_Y = reinterpret_cast<double (*)(const Abstract_t * const)>
                           (get_symbol(functions.handle, "pv_Y"));
   functions.virt_set_Y = reinterpret_cast<Result_codes (*)(Base_t * const, const double)>
                           (get_symbol(functions.handle, "virt_set_Y"));
   functions.abstract.virt_area = reinterpret_cast<double (*)(const Abstract_t * const)>
                           (get_symbol(functions.handle, "virt_area"));
   
   functions.abstract.Abstract_number = reinterpret_cast<int (*)(const Abstract_t * const)>
                           (get_symbol(functions.handle, "Abstract_number"));
   functions.Base_number = reinterpret_cast<int (*)(const Base_t * const)>
                           (get_symbol(functions.handle, "Base_number"));
#else
   cerr << "\nAUTOMATIC DLL LOAD\n";
   functions.handle = nullptr;
   functions.init = Base_init;
   functions.Base_destroy = Base_destroy;
   functions.abstract.interface.Interface_destroy = Interface_destroy;
   
   functions.abstract.interface.pv_number = pv_number;
   functions.abstract.interface.pv_char = pv_char;
   functions.abstract.X = X;
   functions.abstract.virt_set_X = virt_set_X;
   functions.abstract.pv_Y = pv_Y;
   functions.virt_set_Y = virt_set_Y;
   functions.abstract.virt_area = virt_area;
   
   functions.abstract.Abstract_number = Abstract_number;
   functions.Base_number = Base_number;
#endif
}

Interface_expected Base_C_test::interface_expected;
Abstract_expected Base_C_test::abstract_expected;
Base_expected Base_C_test::base_expected;
Interface_real Base_C_test::interface_real;
Abstract_real Base_C_test::abstract_real;
Base_real Base_C_test::base_real;

/*
static void load_base_functions(Base_functions & functions) {
#ifdef MANUAL_DLL_LOAD
   cerr << "\nMANUAL DLL LOAD\n";
   functions.handle   = get_handle("libbase.so", RTLD_LAZY);
   functions.init     = reinterpret_cast<Result_codes (*)(Base_t **, const char * const)> (get_symbol(functions.handle, "Base_init"));
   functions.set_name = reinterpret_cast<Result_codes (*)(Base_t * const, const char * const)> (get_symbol(functions.handle, "Base_set"));
   functions.get_name = reinterpret_cast<Result_codes (*)(const Base_t * const, char ** const)> (get_symbol(functions.handle, "Base_get_name"));
   functions.destroy  = reinterpret_cast<void (*)(Base_t ** const)> (get_symbol(functions.handle, "Base_destroy"));
#else
   cerr << "\nAUTOMATIC DLL LOAD\n";
   functions.handle = nullptr;
   functions.init = reinterpret_cast<Result_codes (*)(Base_t **, const char * const)> (Base_init);
   functions.set_name = reinterpret_cast<Result_codes (*)(Base_t * const, const char * const)> (Base_set);
   functions.get_name = reinterpret_cast<Result_codes (*)(const Base_t * const, char ** const)> (Base_get_name);
   functions.destroy = reinterpret_cast<void (*)(Base_t ** const)> (Base_destroy);
#endif
}
*/

void Base_C_test::test_interface(int n, char c) {
   Interface_t * inter = reinterpret_cast<Interface_t * > (base);
   interface_real.set(functions.abstract.interface.pv_number(inter), functions.abstract.interface.pv_char(inter));

   interface_expected.set(n, c);
   interface_print_and_assert(interface_expected, interface_real);
}

void Base_C_test::test_abstract(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   Abstract_t * abs = reinterpret_cast<Abstract_t * > (base);
   abstract_real.set(functions.abstract.interface.pv_number (reinterpret_cast<Interface_t * > ((abs))), 
                     functions.abstract.interface.pv_char (reinterpret_cast<Interface_t * > ((abs))),
      functions.abstract.X(abs), functions.abstract.pv_Y(abs), functions.abstract.virt_area(abs),
      functions.abstract.Abstract_number(abs));
   
   abstract_expected.set(pv_n, pv_c, x, pv_y, area, n);
   abstract_print_and_assert(abstract_expected, abstract_real);
}

void Base_C_test::test_base(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   base_real.set(functions.abstract.interface.pv_number (reinterpret_cast<Interface_t * > (base)),
                 functions.abstract.interface.pv_char(reinterpret_cast<Interface_t * > (base)),
      functions.abstract.X(reinterpret_cast<Abstract_t * > (base)), 
      functions.abstract.pv_Y(reinterpret_cast<Abstract_t * > (base)), 
      functions.abstract.virt_area(reinterpret_cast<Abstract_t * > (base)), 
      functions.Base_number(base) );

   base_expected.set(pv_n, pv_c, x, pv_y, area, n);
   abstract_print_and_assert(base_expected, base_real);
}

#ifdef MANUAL_DLL_LOAD
Result_codes Base_C_test::main_test_base_linking() {
   cerr << "\nMANUAL DLL LOAD\n";
   Result_codes result = functions.init(&base, 4.0, 8.0);
   if (result == OK) {
      test_interface(BASE, BASE_CHAR);
      test_abstract(BASE, BASE_CHAR, 4.0, 8.0, 4.0 * 8.0, ABSTRACT);
      test_base(BASE, BASE_CHAR, 4.0, 8.0, 4.0 * 8.0, BASE);
      
      result = bind_execute_function_assert<Base_t *, Abstract_t *, Abstract_t *>(base, functions.abstract.X, 7.5, "x", __func__, functions.abstract.virt_set_X);
      if (OK == result)
         result = bind_execute_function_assert<Base_t *, Base_t *, Abstract_t *>(base, functions.abstract.pv_Y, -9.8, "y", __func__, functions.virt_set_Y);
      if (INVALID_ARG == result)
         result = OK;
      else
         assert_many(result != INVALID_ARG, "result == ", result);
      
      if (OK == result)
         result = bind_execute_function_assert<Base_t *, Base_t *, Abstract_t *>(base, functions.abstract.pv_Y, 105.8, "y", __func__, functions.virt_set_Y); 
      if (OK == result) {
         print_and_assert(functions.abstract.virt_area(reinterpret_cast<Abstract_t * > (base)), 7.5 * 105.8, "virt_area");
         result = static_cast<Result_codes> (close_handle(&(functions.handle)));
      }
   }
   functions.Base_destroy(&base);
   if (result != OK)
      close_handle(&(functions.handle));
   assert_many(result == OK, "result == ", result);
   assert_many(base == nullptr, "base pointer == ", base);
   return result;
}
#else
Result_codes Base_C_test::main_test_base_linking() {
   cerr << "\nAUTOMATIC DLL LOAD\n";
   test_interface(BASE, BASE_CHAR);
   test_abstract(BASE, BASE_CHAR, -6.0, 0.0, -6.0 * 0.0, ABSTRACT);
   test_base(BASE, BASE_CHAR, -6.0, 0.0, -6.0 * 0.0, BASE);

   Result_codes result = bind_execute_member_function_assert(base, &Base::X, 7.5, "x", __func__, &Base::virt_set_X, 7.5);
   if (OK == result)
      result = bind_execute_member_function_assert(base, &Base::pv_Y, numeric_limits<double>::infinity(), "y", __func__, &Base::virt_set_Y, numeric_limits<double>::infinity());
   if (INVALID_ARG == result)
      result = OK;
   else
      assert_many(result != INVALID_ARG, "result == ", result);
   
   if (OK == result)
      result = bind_execute_member_function_assert(base, &Base::pv_Y, 105.8, "y", __func__, &Base::virt_set_Y, 105.8);
   if (OK == result)
      print_and_assert(base.virt_area(), 7.5 * 105.8, "virt_area");
   
   assert_many(result == OK, "result == ", result);
   return result;
}
#endif

Result_codes Base_C_test::test_base_linking() {
   load_base_functions();
   Result_codes result = main_test_base_linking();
   assert_many(result == OK, "result == ", result);
   return result; 
}

Result_codes test_base_c() {
   function<Result_codes()> test = bind(&Base_C_test::test_base_linking);
   Result_codes result = call_catch_exception(test);
   assert_many(result == OK, "result == ", result);
   return result; 
}
/*
Result_codes test_base() {
   Base_functions functions;
   load_base_functions(functions);
   Base_t * base = nullptr;
   Base_t ** const base_2ptr = &base;
   Result_codes result = functions.init(base_2ptr, "Baruch Spinoza"); 
   if (OK == result) {
      char * name = nullptr;
      result = functions.get_name(base, &name);
      if (OK == result) {
         cerr << TIE( "C++", unmove(__cplusplus), __func__, " base name = ", name) << '\n';
         assert_many(string(name) == "Baruch Spinoza", "name == ", name);
         free(name);
         result = functions.set_name(base, "Newton");
         if (OK == result) {
            name = nullptr;
            result = functions.get_name(base, &name);
            if (OK == result) {
               cerr << TIE( "C++", unmove(__cplusplus), __func__, " base name = ", name) << '\n';
               assert_many(string(name) == "Newton", "name == ", name);
               free(name);
               name = nullptr;
#ifdef MANUAL_DLL_LOAD
               functions.destroy(base_2ptr);
               assert_many(base == nullptr, "base == ", base);
               assert_many(*base_2ptr == nullptr, "*base_2ptr == ", base);
               result = static_cast<Result_codes> (close_handle(&(functions.handle)));
               assert_many(result == OK, "result == ", result);
               return result;
#endif
            }
         }
      }
      if (name)
         free(name);
   }
#ifdef MANUAL_DLL_LOAD
   close_handle(&(functions.handle));
#endif
   if (base)
      functions.destroy(base_2ptr);
   assert_many(base == nullptr, "base == ", base);
   assert_many(*base_2ptr == nullptr, "*base_2ptr == ", base);
   assert_many(result == OK, "result == ", result);
   return result;
}
*/   
}
