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

int close_handle(void * handle) {
   const int result = dlclose(handle);
   if (0 != result) {
      LOG_EXIT(__FUNCTION__, "Closing handle was failed", result);   /* brackets - multiline macro */
   }
   return 0;
}

#ifdef MANUAL_DLL_LOAD
int load_demo(void) { 
   print_many("PARAMETERS: 1, \"abc\", 546",
         "isi", 1, "abc", 546);
   FUNCTION_INFO(__FUNCTION__);
   Demo_functions demo_functions;
   demo_functions.handle = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   demo_functions.init = get_symbol(demo_functions.handle, "demo_init");
   demo_functions.set_name = get_symbol(demo_functions.handle, "demo_set_name");
   demo_functions.get_name = get_symbol(demo_functions.handle, "demo_get_name");
   demo_functions.destroy = get_symbol(demo_functions.handle, "demo_destroy");
    
   int result = demo_functions.init("Nicolaus Copernicus"); 
   LOG("result = %d\n", result);
   if (OK == result) {
      char * name = NULL;
      result = demo_functions.get_name(&name);
      if (OK == result) {
         LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
         demo_functions.set_name("Socrates");
         result = demo_functions.get_name(&name);
         if (OK == result) {
            LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
            result = demo_functions.destroy();
         }
         if (OK == result)
            result = close_handle(demo_functions.handle);
      }
   }
   assert_many(result == OK, "message", "8, 9, 6", "hg");
   
   return result;
}

int load_money(void) {
   FUNCTION_INFO(__FUNCTION__);
   Money_functions money_functions;
   money_functions.handle = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   money_functions.create_1 = get_symbol(money_functions.handle, "Money_int__init_1");
   money_functions.create_1 = get_symbol(money_functions.handle, "Money_int__create_1");
   void* money = Money_int__init_1("ANSI C");
   LOG("\nAddress of money is: %p\n", money);
   money = Money_int__create_1("ANSI C");
   LOG("\nAddress of money is: %p\n", money);
   money_functions.create_2 = get_symbol(money_functions.handle, "Money_int__init_2");
   money_functions.create_2 = get_symbol(money_functions.handle, "Money_int__create_2");
   money = Money_int__init_2("ANSI C", 9);
   LOG("\nAddress of money is: %p\n", money);
   money = Money_int__create_2("ANSI C", 7);
   LOG("\nAddress of money is: %p\n", money);
   
   return close_handle(money_functions.handle);
}
#else
void load_demo(void) {
   FUNCTION_INFO(__FUNCTION__);
   LOG("\n%s \n", LANGUAGE);
   LOG_FUNC(__FUNCTION__);
   demo_init("Nicolaus Copernicus");
   LOG("%s: %s human name is %s: ", LANGUAGE, __FUNCTION__, demo_get_name());
   demo_set_name("Socrates");
   LOG("%s: %s human name is %s: ", LANGUAGE, __FUNCTION__, demo_get_name());
   demo_destroy();
}

void load_money(void) {
   void* money = Money_int__init_1("ANSI C");
   LOG("\nAddress of money is: %p\n", money);
   money = Money_int__create_1("ANSI C");
   LOG("\nAddress of money is: %p\n", money);
   money = Money_int__init_2("ANSI C", 9);
   LOG("\nAddress of money is: %p\n", money);
   money = Money_int__create_2("ANSI C", 7);
   LOG("\nAddress of money is: %p\n", money);
}
#endif

void at_exit (void) {
   puts (" At exit\n");
   FUNCTION_INFO(__FUNCTION__);
}

int main(void) {
   set_handler(handle_terminate);
   atexit (at_exit);
   FUNCTION_INFO(__FUNCTION__);
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
#endif
   load_demo();
   load_money();
   
   return EXIT_SUCCESS;
}
