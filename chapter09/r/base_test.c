
#include "print.h"
#include "utility.h"
#include "base_test.h"

#include <string.h>

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_BASE_SO     "libbase_cpp.so"
#endif

static Interface_expected interface_expected = { {"pv_number", 0}, {"pv_char", '0'} };

static Abstract_expected abstract_expected = { { {"pv_number", 0}, {"pv_char", '0'} },
                {"x", 0}, {"y", 0}, {"virt_area", 0}, {"number", 0} };

static Base_expected base_expected = { { { {"pv_number", 0}, {"pv_char", '0'} },
                {"x", 0}, {"y", 0}, {"virt_area", 0}, {"number", 0} } };
/*
static Derived_expected * derived_expected = { { { { {"pv_number", 0}, {"pv_char", '0'} },
                {"x", 0}, {"y", 0}, {"virt_area", 0}, {"number", 0} } }, 
   {"z", 0}
};
*/
static Interface_real interface_real =  { 0, '0'};

static Abstract_real abstract_real = { { 0, '0'}, 0, 0, 0, 0 };

static Base_real base_real = { { { 0, '0'}, 0, 0, 0, 0 } };
/*
static Derived_real derived_real = { { { { 0, '0'}, 0, 0, 0, 0 } }, 0 }; */

static Base_functions functions;

static Base_t * base = NULL;

void Base_test::test_interface(int n, char c) {
#ifdef MANUAL_DLL_LOAD
   Interface * inter = base;
   interface_real.set(inter->pv_number(), inter->pv_char());
#else
   Interface & inter = base;
   interface_real.set(inter.pv_number(), inter.pv_char());
#endif
   interface_expected.set(n, c);
   interface_print_and_assert(interface_expected, interface_real);
}

void Base_test::test_abstract(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
#ifdef MANUAL_DLL_LOAD
   Abstract * abs = base;
   abstract_real.set(abs->pv_number(), abs->pv_char(), abs->X(), abs->pv_Y(), abs->virt_area(), abs->number());
#else
   Abstract & abs = base;
   abstract_real.set(abs.pv_number(), abs.pv_char(), abs.X(), abs.pv_Y(), abs.virt_area(), abs.number());
#endif
   abstract_expected.set(pv_n, pv_c, x, pv_y, area, n);
   abstract_print_and_assert(abstract_expected, abstract_real);
}

static void test_base(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
   base_real.set(base->pv_number(), base->pv_char(), base->X(), base->pv_Y(), base->virt_area(), base->number());
   base_expected.set(pv_n, pv_c, x, pv_y, area, n);
   abstract_print_and_assert(base_expected, base_real);
}

static void load_base_functions() {
   if (! functions) {
      LOG_EXIT(__FUNCTION__, "functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
   functions.handle = get_handle(LIB_BASE_SO, RTLD_LAZY);
   functions.init = get_symbol(functions.handle, "Base_init");
   functions.Base_destroy = get_symbol(functions.handle, "Base_destroy");
   functions.abstract.interface.pv_number = get_symbol(functions.handle, "pv_number");
   functions.abstract.interface.pv_char = get_symbol(functions.handle, "pv_char");
   functions.abstract.X = get_symbol(functions.handle, "X");
   functions.abstract.virt_set_X = get_symbol(functions.handle, "virt_set_X");
   functions.abstract.pv_Y = get_symbol(functions.handle, "pv_Y");
   functions.virt_set_Y = get_symbol(functions.handle, "virt_set_Y");
   functions.abstract.virt_area = get_symbol(functions.handle, "virt_area");
   functions.Base_number = get_symbol(functions.handle, "Base_number");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
   functions.handle = NULL;
   functions.init = Base_init;
   functions.Base_destroy = Base_destroy;
   functions.abstract.interface.pv_number = pv_number;
   functions.abstract.interface.pv_char = pv_char;
   functions.abstract.X = X;
   functions.abstract.virt_set_X = virt_set_X;
   functions.abstract.pv_Y = pv_Y;
   functions.virt_set_Y = virt_set_Y;
   functions.abstract.virt_area = virt_area;
   functions.Base_number = Base_number;
#endif
}

#ifdef MANUAL_DLL_LOAD
   Result_codes load_base() {
      base = manual_interface.create(4, 8);
      return (NULL == base) ? INCORRECT_VALUE : OK;
   }
#endif

static Result_codes check_name(Base_functions * functions, const char * const expected_name) {
   REQUIRE_NON_NULL(functions, "functions is null");
   REQUIRE_NON_NULL(expected_name, "expected_name is null");
   char * name = NULL;
   Result_codes result = functions.get_name(&name);
   if (OK == result) {
      LOG("%s: %s base_functions name = %s", LANGUAGE, __FUNCTION__, name);
      assert_many(strcmp(name, expected_name) == 0, "assert failed: ", "s s", "name == ", name);
   }
   return result;
}

static Result_codes check_age(Base_functions * functions, const unsigned int expected) {
   REQUIRE_NON_NULL(functions, "functions is null");
   unsigned int age;
   Result_codes result = functions.get_age(&age);
   if (OK == result) {
      LOG("%s: %s base_functions age = %u", LANGUAGE, __FUNCTION__, age);
      assert_many(age == expected, "assert failed: ", "s u", " age == ", age);
   }
   return result;
}

static Result_codes run_base_functions(const Base_functions * const functions) { 
   if (! functions) {
      LOG_EXIT(__FUNCTION__, "base_functions functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
   Result_codes result = functions.init(&base, -6.0, 0.0); 
   double x;
   if (OK == result)
      result = functions.abstract.X(&x); 
   LOG("%s: %s base_functions x = %f\n", LANGUAGE, __FUNCTION__, x);
   if (OK == result)
      result = functions.abstract.virt_set_X(7); 
   //LOG("%s: %s base_functions y = %f\n", LANGUAGE, __FUNCTION__, x);
   if (OK == result)
      result = functions.abstract.X(&x); 
   LOG("%s: %s base_functions y = %f\n", LANGUAGE, __FUNCTION__, x);
   if (OK == result)
      result = functions.abstract.pv_Y(&x); 
   LOG("%s: %s base_functions y = %f\n", LANGUAGE, __FUNCTION__, x);
//    if (OK == result)
//       result = check_name(functions, "Claudius Ptolemaeus"); 
//    if (OK == result)
//       result = check_age(functions, 9); 
//    if (OK == result)
//       result = functions.set_name("Plato");
//    if (OK == result)
//       result = check_name(functions, "Plato"); 
//    if (OK == result)
//       result = functions.set_age(84);
//    if (OK == result)
//       result = check_age(functions, 84); 
// #ifdef MANUAL_DLL_LOAD
//    if (OK == result) {
//       functions.destroy();
//       result = close_handle(&(functions.handle));
//       assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
//       return result;
//    }
//    close_handle(&(functions.handle));
// #endif
   functions.destroy();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

#ifdef MANUAL_DLL_LOAD
Result_codes test_base_linking() {
   cerr << "\nMANUAL DLL LOAD\n";
   Result_codes result = functions.init(&base, 4.0, 8.0);
   if (result == OK) {
      test_interface(Base::BASE, Base::BASE_CHAR);
      test_abstract(Base::BASE, Base::BASE_CHAR, 4.0, 8.0, 4.0 * 8.0, Abstract::ABSTRACT);
      test_base(Base::BASE, Base::BASE_CHAR, 4.0, 8.0, 4.0 * 8.0, Base::BASE);
      
      result = bind_execute_member_function_assert(base, &Base::X, 7.5, "x", __func__, &Base::virt_set_X, 7.5);
      if (OK == result)
         result = bind_execute_member_function_assert(base, &Base::pv_Y, -9.8, "y", __func__, &Base::virt_set_Y, -9.8);
      if (INVALID_ARG == result)
         result = OK;
      else
         assert_many(result != INVALID_ARG, "result == ", result);
      
      if (OK == result)
         result = bind_execute_member_function_assert(base, &Base::pv_Y, 105.8, "y", __func__, &Base::virt_set_Y, 105.8); 
      if (OK == result) {
         print_and_assert(base->virt_area(), 7.5 * 105.8, "virt_area");
         result = static_cast<Result_codes> (close_handle(&(manual_interface.handle)));
      }
   }
   manual_interface.destroy(base);
   if (result != OK)
      close_handle(&(manual_interface.handle));
   assert_many(result == OK, "result == ", result);
   assert_many(base == nullptr, "base pointer == ", base);
   return result;
}
#else
Result_codes test_base_linking() {
   cerr << "\nAUTOMATIC DLL LOAD\n";
   Result_codes result = 
   test_interface(Base::BASE, Base::BASE_CHAR);
   test_abstract(Base::BASE, Base::BASE_CHAR, -6.0, 0.0, -6.0 * 0.0, Abstract::ABSTRACT);
   test_base(Base::BASE, Base::BASE_CHAR, -6.0, 0.0, -6.0 * 0.0, Base::BASE);

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

Result_codes test_base_cpp(void) {
   FUNCTION_INFO(__FUNCTION__);
   load_base_functions(); 
   Result_codes result = run_base_functions(&functions);
   return result;
}
