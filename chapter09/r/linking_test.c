/*
#define MANUAL_DLL_LOAD
*/
#include "print.h"
#include "c_string.h"
#include "connector.h"
#include "result_codes.h"
#include "human.h"
#include "utility.h"
#include "human_functions.h"
#include "demo_functions.h"

#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <float.h>

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_CONNECTOR_SO "libconnector.so"
   #define LIB_HUMAN_SO     "libhuman.so"
#endif

void handle_terminate(void) { 
   LOG("%s", "handler called - must've been some exception ?!\n");
}

static void at_exit (void) {
   puts (" At exit\n");
   FUNCTION_INFO(__FUNCTION__);
}

/*
#ifdef MANUAL_DLL_LOAD 
static void load_demo(const Demo_functions * demo_functions, const Shared_library * const shared_lib) {
   LOG("%s", "\nMANUAL DLL LOAD\n");
   if (! demo_functions || ! shared_lib) {
      FUNCTION_INFO(__FUNCTION__);
      LOG("%s\n", "demo_functions / shared_lib is NULL ");
      longjmp(JMP_BUF, 3);
   }
   demo_functions->handle = get_handle(shared_lib->shared_lib_name, RTLD_LAZY);
   demo_functions->init = get_symbol(demo_functions->handle, shared_lib->init);
   demo_functions->set_name = get_symbol(demo_functions->handle, shared_lib->set_name);
   demo_functions->get_name = get_symbol(demo_functions->handle, shared_lib->get_name);
   demo_functions->destroy = get_symbol(demo_functions->handle, shared_lib->destroy);
}
#else
static void load_demo(Demo_functions * const demo_functions, Result_codes (*a)(const char * ),
   Result_codes (*b)(const char * ), Result_codes (*c) (char ** ), Result_codes (*d)(void)) {
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
   if (! demo_functions || !a || !b || !c || !d) {
      FUNCTION_INFO(__FUNCTION__);
      LOG("%s\n", "input argument is NULL ");
      longjmp(JMP_BUF, 3);
   }   
   demo_functions->handle = NULL;
   demo_functions->init = a;
   demo_functions->set_name = b;
   demo_functions->get_name = c;
   demo_functions->destroy = d;
}
#endif
*/

char * get_format(const Number type) { 
   switch(type) {
      case SHORT: 
         return "hd";
      case U_SHORT: 
         return "hu";
      case INT: 
         return "d";
      case U_INT: 
         return "u";
      case LONG: 
         return "ld";
      case U_LONG: 
         return "lu";
      case LONG_LONG: 
         return "lld";
      case U_LONG_LONG: 
         return "llu";
      case FLOAT:
      case DOUBLE:
         return "G";
      case LONG_DOUBLE:
         return "Lg";
      default: {
         const char * type_string = to_string((int)type);
         const char * message = concatenate_many_free_args(" improper type = ", 0, type_string, 1, NULL);
         LOG_EXIT_FREE(__FUNCTION__, message, EXIT_FAILURE);
      }
   } 
}

int print_union_member(const Number type, union Number_pointer_union n_union, const char * format) {
   switch(type) {
      case SHORT: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.s);
      case U_SHORT: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.us);
      case INT: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.i);
      case U_INT: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.ui);
      case LONG: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.l);
      case U_LONG: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.ul);
      case LONG_LONG: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.ll);
      case U_LONG_LONG:  
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.ull);
      case FLOAT:
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.f);
      case DOUBLE: 
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.d);
      case LONG_DOUBLE:
         return print_many("Value of n_union->type is: ", format ? format : get_format(type), n_union.ld);
      default: {
         LOG(" improper type = %d", type);
         return INVALID_ARG;
      }
   } 
}

#define LOGS_MONEY(money, n_union, format, type) \
LOG("\nAddress of money is: %p", &money); \
LOG("\nAddress pointed by money is: %p", money); \
LOG("\nAddress of n_union is: %p\n", &n_union); \
print_union_member(type, n_union, format); \
LOG("%c", '\n')

typedef Result_codes (*p_func_many)(Money_type * money_ptr, const Money_functions function, const Number type, 
                                  union Number_pointer_union * n_union, const char * ,... );

static Result_codes run_money(const Number type, const char * dollars, const long double cents) {
   if (type < SHORT || type > LONG_DOUBLE || ! dollars) {
      LOG(" type = %d \t dollars = %p\n", type, dollars);
      return INVALID_ARG;
   }
#ifdef MANUAL_DLL_LOAD
   void* handle = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   p_func_many p_function = get_symbol(handle, "Money_type__function");
#else
   p_func_many p_function = Money_type__function;
#endif
   Money_type money = NULL;
   union Number_pointer_union n_union;
   const char * format = get_format(type);
   Result_codes result = p_function(&money, INIT_1, type, &n_union, dollars);
   if (result == OK) {
      LOGS_MONEY(money, n_union, format, type);
      money = NULL;
      result = p_function(&money, CREATE_1, type, &n_union, dollars);
      if (result == OK) {
         LOGS_MONEY(money, n_union, format, type);
         money = NULL;
         result = p_function(&money, INIT_2, type, &n_union, dollars, cents);  
         if (result == OK) {
            LOGS_MONEY(money, n_union, format, type);
            money = NULL;
            result = p_function(&money, CREATE_2, type, &n_union, dollars, cents); 
            if (result == OK) {
               LOGS_MONEY(money, n_union, format, type);
#ifdef MANUAL_DLL_LOAD
               if (OK == result) {
                  result = close_handle(&handle); 
                  assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
                  return result;
               }
#endif
            }
         }
      }
   }
#ifdef MANUAL_DLL_LOAD
   close_handle(&handle);
#endif 
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

#undef LOGS_MONEY

const int test_print_many_longjmp = 22;

static _Noreturn void test_print_many(void) {
   float f = 6.0f;
   LOG("\nAddress pointed by money is: %p\n", &f);            /* mandatory casting when using va_list function  */
   print_many("1 test of print_many", "p G Lg o  rr7 d G s d s", (float*)(&f), f, (long double)(-5.0), 399, 6.0, "QQQQQQ", 7, "rrrr");
   int i = 66;
   LOG("\nAddress pointed by money is: %p\n", &i);
   print_many("2 test of print_many", " p  d       Lg  d G s d s", (int*)(&i), i, -5.0L, 399, 6.0, "QQQQQQ", 7, "rrrr");
   void * nul = &i;
   print_many(" 3 test of print_many", " p  c hd hu F G F G  u  lu ld llu lld ", nul, 'i', (short)7, (unsigned short)USHRT_MAX, 6.0f, 7.0f,
                           77.8, 66.6, (unsigned)99, (unsigned long)ULONG_MAX, (long)LONG_MAX, (unsigned long long)ULLONG_MAX, (long long)LLONG_MAX);
   longjmp(JMP_BUF, test_print_many_longjmp);
}

#define TEST_ALLOC(TYPE, dollars, number, cents) \
if (OK == result) { \
   dollars = to_string((number)); \
   result = run_money(TYPE, dollars, cents); \
   free(dollars); \
}

#define LOG_MAX(TYPE, number, format) \
format = get_format(TYPE); \
LOG(#TYPE " = " #number " = %s", ""); \
print_many("", format, number); \
LOG(" %c ", '\n')

static Result_codes test_money(void) {
   Result_codes result = OK;
   char * max_dollars;
   char * format;
   LOG_MAX(SHORT, SHRT_MIN, format);
   TEST_ALLOC(SHORT, max_dollars, SHRT_MIN / 110, 0);
   LOG_MAX(SHORT, SHRT_MIN / 110, format);
   
   TEST_ALLOC(U_SHORT, max_dollars, USHRT_MAX / 110, 0);
   LOG_MAX(U_SHORT, USHRT_MAX / 110, format);
   
   TEST_ALLOC(INT, max_dollars, INT_MIN / 110, 0);
   TEST_ALLOC(U_INT, max_dollars, UINT_MAX / 110, 0);
   TEST_ALLOC(LONG, max_dollars, LONG_MIN / 110, 0);
   TEST_ALLOC(U_LONG, max_dollars, ULONG_MAX / 110, 0);
   TEST_ALLOC(LONG_LONG, max_dollars, LLONG_MIN / 110, 0);
   TEST_ALLOC(U_LONG_LONG, max_dollars, ULLONG_MAX / 110, 0);
   TEST_ALLOC(FLOAT, max_dollars, FLT_MAX  / 110, 0);
   TEST_ALLOC(DOUBLE, max_dollars, DBL_MIN  * 100, 0);
   TEST_ALLOC(LONG_DOUBLE, max_dollars, LDBL_MIN * 100, 0);
   
   LOG_MAX(FLOAT, FLT_MAX  / 110, format);
   LOG_MAX(SHORT, SHRT_MIN, format);
   LOG_MAX(LONG_LONG, LLONG_MAX, format);
   LOG_MAX(LONG_LONG, LLONG_MIN, format);
   char * number_string = to_string(LLONG_MIN);
   LOG("%s\n", number_string);
   free(number_string);
   return result;
}

#undef TEST_ALLOC

static Result_codes main_test_linking(void) {
   Result_codes result = test_demo_linking();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   if (OK == result)
      result = test_human_linking();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   if (OK == result)
      result = test_money();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}
/*
static Result_codes test_linking(void) {
   volatile int jmp_value = 0;
   if ((jmp_value = setjmp(JMP_BUF)) != 0) {
      LOG("\nAfter calling test_print_many longjmp set value to %d\n", jmp_value);
      if ((jmp_value = setjmp(JMP_BUF)) != 0) {
         FUNCTION_INFO(__FUNCTION__);
         LOG("\nlongjmp set value to %d\n Return with %d ?\n", jmp_value, EXIT_FAILURE);
      }
      else 
         load_demo(NULL);
      
      return main_test_linking(); 
   }
   else 
      test_print_many();
}
*/
static Result_codes tests(void) {
   volatile int jmp_value = 0;
   const int jmp_value_2 = -35;
   if ((jmp_value = setjmp(JMP_BUF)) != 0) {
      FUNCTION_INFO(__FUNCTION__);
      LOG("\nAfter calling test_print_many longjmp set value to %d\n", jmp_value);
      assert_many(test_print_many_longjmp == jmp_value, "assert failed: ", "s d", "jmp_value should be ", test_print_many_longjmp);
      if ((jmp_value = setjmp(JMP_BUF)) != 0) {
         FUNCTION_INFO(__FUNCTION__);
         LOG("\nlongjmp set value to %d\n Return with %d ?\n", jmp_value, EXIT_FAILURE);
         assert_many(jmp_value_2 == jmp_value, "assert failed: ", "s d", "jmp_value should be ", jmp_value_2);/*
         return EXIT_FAILURE;*/
      }
      else 
         longjump_test(JMP_BUF, jmp_value_2);
      
      return main_test_linking(); 
   }
   else 
      test_print_many();
}

int main(void) {
   FUNCTION_INFO(__FUNCTION__);
   set_handler(handle_terminate);
   atexit (at_exit);
   Result_codes result = tests();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}
