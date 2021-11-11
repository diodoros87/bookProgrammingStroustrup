/*
#define MANUAL_DLL_LOAD
*/
#include "print.h"
#include "connector.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>

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

#define LOGS_MONEY(money, n_union, format, value) \
LOG("\nAddress of money is: %p", &money); \
LOG("\nAddress pointed by money is: %p", money); \
LOG("\nAddress of n_union is: %p\n", &n_union); \
print_many("Value of n_union->type is: ", format, value); \
LOG("%c", '\n')

#define allocate(buffer, source) \
buffer = (char *) malloc (strlen(source) + 1); \
if (buffer == NULL) { \
   LOG_EXIT(__FUNCTION__, "out of memory: malloc() returns NULL ", EXIT_FAILURE);  \
} \
strcpy(buffer, source)

int set_format(const Number type, char * format) {
   if (format) {
      LOG_EXIT(__FUNCTION__, "input c-string format must be NULL ", EXIT_FAILURE);   
   }
   if (1 != strlen(format)) {
      LOG_EXIT(__FUNCTION__, "c-string format must be length = 1 ", EXIT_FAILURE);   
   }   
   switch(type) {
      case CHAR:
         allocate(format, 1, "c");
         break;
      case SHORT:
         allocate(format, 2, "hd");
         break;
      case 's':
         arg_string = va_arg( arg_list, char * );
         LOG( " %s ", arg_string );
         break;
      case 'e':
         arg_long_double = va_arg( arg_list, long double );
         LOG( " %e ", arg_long_double );
         break;
      case 'f':
         arg_double = va_arg( arg_list, double );
         LOG( " %f ", arg_double );
         break;
      case 'c':
         arg_char = va_arg( arg_list, char );
         LOG( " %c ", arg_char );
         break;
      default: {
         LOG_EXIT(__FUNCTION__, " improper format = %d", EXIT_FAILURE);
      }
   }
   if (1 != strlen(format)) {
      LOG_EXIT(__FUNCTION__, "c-string format must be length = 1 ", EXIT_FAILURE);   
   } 
   
} 

typedef int (*p_func_many)(Money_type * money_ptr, const Money_functions function, const Number type, 
                                  union Number_pointer_union * n_union, const char * ,... );
int run_money(const Number type) {
#ifdef MANUAL_DLL_LOAD
   void* handle = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   p_func_many p_function = get_symbol(handle, "Money_type__function");
#else
   p_func_many p_function = Money_type__function;
#endif
   Money_type money = NULL;
   union Number_pointer_union n_union;
   Result_codes result = p_function(&money, INIT_1, type, &n_union, "20");
   if (result == OK) {
      LOGS_MONEY(money, n_union, " s", n_union.f);
      money = NULL;
      result = p_function(&money, CREATE_1, type, &n_union, "9");
      if (result == OK) {
         LOGS_MONEY(money, n_union, "s", n_union.f);
         money = NULL;
         result = p_function(&money, INIT_2, type, &n_union, "0", (long double)(8.0));  /* mandatory casting when using va_list function  */
         if (result == OK) {
            LOGS_MONEY(money, n_union, "s", n_union.f);
            money = NULL;
            result = p_function(&money, CREATE_2, type, &n_union, "0", (long double)(-5.0));  /* mandatory casting when using va_list function  */
            if (result == OK) {
               LOG("\nAddress pointed by money is: %p\n", money);
               LOG("\nAddress of money is: %p\n", &money);
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
/*
#else
int run_money(void) {
   Money_type money = NULL;
   Result_codes result = Money_type__function(&money, INIT_1, type, &n_union, "8");
   if (result == OK) {
      LOG("\nAddress of money is: %p\n", &money);
      LOG("\nAddress pointed by money is: %p\n", money);
      money = NULL;
      result = Money_type__create_1(&money, "9.0");
      if (result == OK) {
         LOG("\nAddress pointed by money is: %p\n", money);
         if (result == OK) {
            money = NULL;
            result = Money_type__init_2(&money, "6.", 9);
            LOG("\nAddress pointed by money is: %p\n", money);
            if (result == OK) {
               money = NULL;
               result = Money_type__create_2(&money, "9e3", 7);
               LOG("\nAddress pointed by money is: %p\n", money);
            }
         }
      }
   }
   return result; 
}
#endif
*/

void test_print_many(void) {
   float f = 6.0f;
   LOG("\nAddress pointed by money is: %p\n", &f);
   print_many("1 test of print_many", "p g Lg o  rr7 d g s d s", (float*)(&f), f, (long double)(-5.0), 399, 6.0, "QQQQQQ", 7, "rrrr");
   int i = 66;
   LOG("\nAddress pointed by money is: %p\n", &i);
   print_many("2 test of print_many", " p  d   Lg  d g s d s", (int*)(&i), i, -5.0L, 399, 6.0, "QQQQQQ", 7, "rrrr");
   void * nul = &i;
   print_many(" 3 test of print_many", " p  c hd hu f g f  g  u  lu ld llu lld ", nul, 'i', (short)7, (unsigned short)USHRT_MAX, 6.0f, 7.0f,
                           77.8, 66.6, (unsigned)99, (unsigned long)ULONG_MAX, (long)LONG_MAX, (unsigned long long)ULLONG_MAX, (long long)LLONG_MAX);
}

int main(void) {
   FUNCTION_INFO(__FUNCTION__);
   set_handler(handle_terminate);
   atexit (at_exit);
   test_print_many();
   Demo_functions demo_functions;
   load_demo(&demo_functions);
   Result_codes result = run_demo(&demo_functions);
   if (OK == result)
      result = run_money(INT);
   
   return result;
}
