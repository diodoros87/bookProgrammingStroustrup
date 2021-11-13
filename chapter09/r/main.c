/*
#define MANUAL_DLL_LOAD
*/
#include "print.h"
#include "connector.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include<float.h>

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
#endif

#define LIB_CONNECTOR_SO "libconnector.so"

void handle_terminate(void) { 
   LOG("%s", "handler called - must've been some exception ?!\n");
}

static void at_exit (void) {
   puts (" At exit\n");
   FUNCTION_INFO(__FUNCTION__);
}

void load_demo(Demo_functions * demo_functions) {
   if (! demo_functions) {
      LOG_EXIT(__FUNCTION__, "demo_functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
   demo_functions->handle = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   demo_functions->init = get_symbol(demo_functions->handle, "demo_init");
   demo_functions->set_name = get_symbol(demo_functions->handle, "demo_set_name");
   demo_functions->get_name = get_symbol(demo_functions->handle, "demo_get_name");
   demo_functions->destroy = get_symbol(demo_functions->handle, "demo_destroy");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
   demo_functions->handle = NULL;
   demo_functions->init = demo_init;
   demo_functions->set_name = demo_set_name;
   demo_functions->get_name = demo_get_name;
   demo_functions->destroy = demo_destroy;
#endif
}

int run_demo(const Demo_functions * demo_functions) { 
   if (! demo_functions) {
      LOG_EXIT(__FUNCTION__, "demo_functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
   Result_codes result = demo_functions->init("Nicolaus Copernicus"); 
   if (OK == result) {
      char * name = NULL;
      result = demo_functions->get_name(&name);
      if (OK == result) {
         LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
         name = NULL;
         demo_functions->set_name("Aristotle");
         result = demo_functions->get_name(&name);
         if (OK == result) {
            LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
            result = demo_functions->destroy();
         }
#ifdef MANUAL_DLL_LOAD
         if (OK == result) {
            result = close_handle(&(demo_functions->handle));
            assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
            return result;
         }
#endif
      }
   }
#ifdef MANUAL_DLL_LOAD
   close_handle(&(demo_functions->handle));
#endif
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

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
         LOG(" improper type = %d", type);
         LOG_EXIT(__FUNCTION__, "", EXIT_FAILURE);
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
         LOG_EXIT(__FUNCTION__, "", EXIT_FAILURE);
      }
   } 
}

#define LOGS_MONEY(money, n_union, format, type) \
LOG("\nAddress of money is: %p", &money); \
LOG("\nAddress pointed by money is: %p", money); \
LOG("\nAddress of n_union is: %p\n", &n_union); \
print_union_member(type, n_union, format); \
LOG("%c", '\n')

typedef int (*p_func_many)(Money_type * money_ptr, const Money_functions function, const Number type, 
                                  union Number_pointer_union * n_union, const char * ,... );

int run_money(const Number type, const char * dollars, const long double cents) {
   if (type < SHORT || type > LONG_DOUBLE || ! dollars) {
      LOG(" type = %d \t dollars = %p", type, dollars);
      LOG_EXIT(__FUNCTION__, "invalid input argument(s)", EXIT_FAILURE);
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
                  result = close_handle(&handle); /*
                  free(format); */
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
#endif /*
   free(format);*/
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

#undef LOGS_MONEY

void test_print_many(void) {
   float f = 6.0f;
   LOG("\nAddress pointed by money is: %p\n", &f);            /* mandatory casting when using va_list function  */
   print_many("1 test of print_many", "p G Lg o  rr7 d G s d s", (float*)(&f), f, (long double)(-5.0), 399, 6.0, "QQQQQQ", 7, "rrrr");
   int i = 66;
   LOG("\nAddress pointed by money is: %p\n", &i);
   print_many("2 test of print_many", " p  d       Lg  d G s d s", (int*)(&i), i, -5.0L, 399, 6.0, "QQQQQQ", 7, "rrrr");
   void * nul = &i;
   print_many(" 3 test of print_many", " p  c hd hu F G F G  u  lu ld llu lld ", nul, 'i', (short)7, (unsigned short)USHRT_MAX, 6.0f, 7.0f,
                           77.8, 66.6, (unsigned)99, (unsigned long)ULONG_MAX, (long)LONG_MAX, (unsigned long long)ULLONG_MAX, (long long)LLONG_MAX);
}
/*
typedef struct {
   Type type;
   char * dollars;
   long double cents;
} Money_testing;

void init(Money_testing * test, const Type type, ) {
   if (test == NULL) {
      LOG_EXIT(__FUNCTION__, "input Money_testing is NULL ", EXIT_FAILURE);   
   }
      
}
*/

#define Types_SIZE 11

#define TEST_ALLOC(TYPE, dollars, number, cents) \
if (OK == result) { \
   dollars = to_string((number)); \
   result = run_money(TYPE, dollars, cents); \
   free(dollars); \
}

#undef ALLOCATE

int test_money(void) { /*
   static const Type types [Types_SIZE] = { SHORT, U_SHORT, INT, U_INT, LONG, U_LONG, LONG_LONG, U_LONG_LONG, FLOAT, DOUBLE, LONG_DOUBLE };
   for (int i = 0; i < Types_SIZE; i++) */
      //run_money(Types_SIZE[i], Money_testing); 
   Result_codes result = run_money(SHORT, "-5", 77.8);
   if (OK == result)
      result = run_money(U_SHORT, "33", 77.8);
   if (OK == result)
      result = run_money(INT, "5", 77.8);
   if (OK == result)
      result = run_money(U_INT, "54", 77.8);
   if (OK == result)
      result = run_money(LONG, "756", 77.8);
   if (OK == result)
      result = run_money(U_LONG, "56", 77.8);
   if (OK == result)
      result = run_money(LONG_LONG, "75776", -77.8);
   if (OK == result)
      result = run_money(U_LONG_LONG, "777756", -7.8);
   if (OK == result)
      result = run_money(FLOAT, "3333.88", 77.8);
   if (OK == result)
      result = run_money(DOUBLE, "3333.7", 77.8);
   if (OK == result)
      result = run_money(LONG_DOUBLE, "-8883333", 77.8);
    
   char * max_dollars;
   TEST_ALLOC(SHORT, max_dollars, SHRT_MIN / 110, 0);
   TEST_ALLOC(U_SHORT, max_dollars, USHRT_MAX / 110, 0);
   TEST_ALLOC(INT, max_dollars, INT_MIN / 110, 0);
   TEST_ALLOC(U_INT, max_dollars, UINT_MAX / 110, 0);
   TEST_ALLOC(LONG, max_dollars, LONG_MIN / 110, 0);
   TEST_ALLOC(U_LONG, max_dollars, ULONG_MAX / 110, 0);
   TEST_ALLOC(LONG_LONG, max_dollars, LLONG_MIN / 110, 0);
   TEST_ALLOC(U_LONG_LONG, max_dollars, ULLONG_MAX / 110, 0);
   TEST_ALLOC(FLOAT, max_dollars, FLT_MAX  / 110, 0);
   TEST_ALLOC(DOUBLE, max_dollars, DBL_MAX  / 110, 0);
   TEST_ALLOC(LONG_DOUBLE, max_dollars, LDBL_MAX / 110, 0);
   return result;
}

#undef TEST_ALLOC

int main(void) {
   FUNCTION_INFO(__FUNCTION__);
   set_handler(handle_terminate);
   atexit (at_exit);
   test_print_many();
   Demo_functions demo_functions;
   load_demo(&demo_functions);
   Result_codes result = run_demo(&demo_functions);
   if (OK == result)
      result = test_money();
   return result;
}
