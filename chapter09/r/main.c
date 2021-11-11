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

#ifdef ALLOC
#define ALLOCATE(buffer, source) \
(buffer) = (char *) malloc (strlen(source) + 1); \
if ((buffer) == NULL) { \
   LOG("%s", "out of memory: malloc() returns NULL ");  \
} \
else { \
   strcpy((buffer), (source)); \
   LOG("allocate: %s\n", buffer); \
} \
return buffer
#endif

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

#undef ALLOCATE

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
   const char * format = get_format(type);
   Result_codes result = p_function(&money, INIT_1, type, &n_union, "20");
   if (result == OK) {
      LOGS_MONEY(money, n_union, format, n_union.i);
      money = NULL;
      result = p_function(&money, CREATE_1, type, &n_union, "9");
      if (result == OK) {
         LOGS_MONEY(money, n_union, format, n_union);
         money = NULL;
         result = p_function(&money, INIT_2, type, &n_union, "0", (long double)(8.0));  /* mandatory casting when using va_list function  */
         if (result == OK) {
            LOGS_MONEY(money, n_union, format, n_union);
            money = NULL;
            result = p_function(&money, CREATE_2, type, &n_union, "0", (long double)(-5.0));  /* mandatory casting when using va_list function  */
            if (result == OK) {
               LOGS_MONEY(money, n_union, format, n_union);
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
   print_many("1 test of print_many", "p G Lg o  rr7 d G s d s", (float*)(&f), f, (long double)(-5.0), 399, 6.0, "QQQQQQ", 7, "rrrr");
   int i = 66;
   LOG("\nAddress pointed by money is: %p\n", &i);
   print_many("2 test of print_many", " p  d       Lg  d G s d s", (int*)(&i), i, -5.0L, 399, 6.0, "QQQQQQ", 7, "rrrr");
   void * nul = &i;
   print_many(" 3 test of print_many", " p  c hd hu F G F G  u  lu ld llu lld ", nul, 'i', (short)7, (unsigned short)USHRT_MAX, 6.0f, 7.0f,
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
   if (OK == result)
      result = run_money(INT);
   if (OK == result)
      result = run_money(LONG_LONG);
   return result;
}
