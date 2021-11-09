/*
#define MANUAL_DLL_LOAD
*/
#include "print.h"
#include "connector.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

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
            assert_many(result == OK, "assert failed: ", "si", "result == ", result);
            return result;
         }
#endif
      }
   }
#ifdef MANUAL_DLL_LOAD
   close_handle(&(demo_functions->handle));
#endif
   assert_many(result == OK, "assert failed: ", "si", "result == ", result);
   return result;
}

#ifdef MANUAL_DLL_LOAD
typedef int (*p_func_many)(Money_type* , Money_functions, char *, ... );

int run_money(void) {
   void* handle = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   p_func_many p_function = get_symbol(handle, "Money_type__function");
   Money_type money = NULL;
   Result_codes result = p_function(&money, INIT_1, "20");
   if (result == OK) {
      LOG("\nAddress of money is: %p\n", &money);
      money = NULL;
      result = p_function(&money, CREATE_1, "-9");
      if (result == OK) {
         LOG("\nAddress pointed by money is: %p\n", money);
         money = NULL;
         result = p_function(&money, INIT_2, "0", (long double)(8.0));  /* mandatory casting when using va_list function  */
         if (result == OK) {
            LOG("\nAddress pointed by money is: %p\n", money);
            money = NULL;
            result = p_function(&money, CREATE_2, "0", (long double)(-5.0));  /* mandatory casting when using va_list function  */
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
int run_money(void) {
   Money_type money = NULL;
   Result_codes result = Money_type__init_1("ANSI C");
   LOG("\nAddress of money is: %p\n", &money);
   LOG("\nAddress pointed by money is: %p\n", money);
   money = Money_type__create_1("ANSI C");
   LOG("\nAddress pointed by money is: %p\n", money);
   money = Money_type__init_2("ANSI C", 9);
   LOG("\nAddress pointed by money is: %p\n", money);
   money = Money_type__create_2("ANSI C", 7);
   LOG("\nAddress pointed by money is: %p\n", money);
}
#endif

int main(void) {
   FUNCTION_INFO(__FUNCTION__);
   set_handler(handle_terminate);
   atexit (at_exit);
   Demo_functions demo_functions;
   load_demo(&demo_functions);
   Result_codes result = run_demo(&demo_functions);
   if (OK == result)
      result = run_money();
   
   return result;
}
