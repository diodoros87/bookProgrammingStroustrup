/*
#define MANUAL_DLL_LOAD
*/
#include "print.h"
#include "connector.h"
#include "shared_lib_open.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <dlfcn.h>

#define LIB_CONNECTOR_SO "libconnector.so"

void handle_terminate(void) {
   LOG("%s", "handler called - must've been some exception ?!\n");
}

static void at_exit (void) {
   puts (" At exit\n");
   FUNCTION_INFO(__FUNCTION__);
}

int close_handle(void ** handle) {
   if (! handle) {
      LOG_EXIT(__FUNCTION__, "handle is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
   if (! *handle) {
      LOG_EXIT(__FUNCTION__, "address pointed by handle is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }   
   const int result = dlclose(*handle);
   if (0 != result) {
      LOG_EXIT(__FUNCTION__, "Closing handle was failed", result);   /* brackets - multiline macro */
   }
   return 0;
}

#ifdef MANUAL_DLL_LOAD
typedef int (*p_func_many)(Money_int* , Money_functions, char *, ... );

int load_demo(void) { 
   FUNCTION_INFO(__FUNCTION__);
   Demo_functions demo_functions;
   demo_functions.handle = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   demo_functions.init = get_symbol(demo_functions.handle, "demo_init");
   demo_functions.set_name = get_symbol(demo_functions.handle, "demo_set_name");
   demo_functions.get_name = get_symbol(demo_functions.handle, "demo_get_name");
   demo_functions.destroy = get_symbol(demo_functions.handle, "demo_destroy");
    
   Result_codes result = demo_functions.init("Nicolaus Copernicus"); 
   if (OK == result) {
      char * name = NULL;
      result = demo_functions.get_name(&name);
      if (OK == result) {
         LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
         name = NULL;
         demo_functions.set_name("Aristotle");
         result = demo_functions.get_name(&name);
         if (OK == result) {
            LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
            result = demo_functions.destroy();
         }
         if (OK == result) {
            result = close_handle(&(demo_functions.handle));
            assert_many(result == OK, "assert failed: ", "si", "result == ", result);
            return result;
         }
      }
   }
   close_handle(&(demo_functions.handle));
   assert_many(result == OK, "assert failed: ", "si", "result == ", result);
   return result;
}

int load_money(void) {
   FUNCTION_INFO(__FUNCTION__);
   void* handle = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   p_func_many p_function = get_symbol(handle, "Money_int__function");
   void* money = NULL;
   Result_codes result = p_function(&money, INIT_1, "INIT_1 ANSI C");
   if (result == OK) {
      LOG("\nAddress of money is: %p\n", &money);
      money = NULL;
      result = p_function(&money, CREATE_1, "CREATE_1 ANSI C");
      if (result == OK) {
         LOG("\nAddress pointed by money is: %p\n", money);
         money = NULL;
         result = p_function(&money, INIT_2, "INIT_2 ANSI C", (long double)(8.0));  /* mandatory casting when using va_list function  */
         if (result == OK) {
            LOG("\nAddress pointed by money is: %p\n", money);
            money = NULL;
            result = p_function(&money, CREATE_2, "CREATE_2 ANSI C", (long double)(5.0));  /* mandatory casting when using va_list function  */
            if (result == OK) {
               LOG("\nAddress pointed by money is: %p\n", money);
               LOG("\nAddress of money is: %p\n", &money);
               result = close_handle(&handle);
               assert_many(result == OK, "assert failed: ", "si", "result == ", result);
               return result;
            }
         }
      }
   }
   close_handle(&handle);
   assert_many(result == OK, "assert failed: ", "si", "result == ", result);
   return result;
}
#else
void load_demo(void) {
   FUNCTION_INFO(__FUNCTION__);
   demo_init("Nicolaus Copernicus");
   LOG("%s: %s human name is %s: ", LANGUAGE, __FUNCTION__, demo_get_name());
   demo_set_name("Socrates");
   LOG("%s: %s human name is %s: ", LANGUAGE, __FUNCTION__, demo_get_name());
   demo_destroy();
}

void load_money(void) {
   void* money = Money_int__init_1("ANSI C");
   LOG("\nAddress of money is: %p\n", &money);
   LOG("\nAddress pointed by money is: %p\n", money);
   money = Money_int__create_1("ANSI C");
   LOG("\nAddress pointed by money is: %p\n", money);
   money = Money_int__init_2("ANSI C", 9);
   LOG("\nAddress pointed by money is: %p\n", money);
   money = Money_int__create_2("ANSI C", 7);
   LOG("\nAddress pointed by money is: %p\n", money);
}
#endif

int main(void) {
   FUNCTION_INFO(__FUNCTION__);
   set_handler(handle_terminate);
   atexit (at_exit);
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
#endif
   load_demo();
   load_money();
   
   return EXIT_SUCCESS;
}
