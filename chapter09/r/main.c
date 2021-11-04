#include "print.h"
#include "connector.h"
#include "shared_lib_open.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <dlfcn.h>

#define LANGUAGE "C90"

#define LIB_CONNECTOR_SO "libconnector.so"

typedef struct {
   void (*init)(const char * );
   void (*set_name)(const char * );
   const char * (*get_name) ();
   void (*destroy)();
   void * handle;
} Demo_functions;

void demo_case() {
   Demo_functions demo_functions;
   demo_functions.handle = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   demo_functions.init = get_symbol(demo_functions.handle, "demo_init");
   demo_functions.set_name = get_symbol(demo_functions.handle, "demo_set_name");
   demo_functions.get_name = get_symbol(demo_functions.handle, "demo_get_name");
   demo_functions.destroy = get_symbol(demo_functions.handle, "demo_destroy");
   
   LOG("\n%s \n", LANGUAGE);
   LOG_FUNC(__FUNCTION__);
   demo_functions.init("Nicolaus Copernicus");
   LOG("%s: %s human name is %s: ", LANGUAGE, __FUNCTION__, demo_functions.get_name());
   demo_functions.set_name("Socrates");
   LOG("%s: %s human name is %s: ", LANGUAGE, __FUNCTION__, demo_functions.get_name());
   demo_functions.destroy();
   
   int result = dlclose(demo_functions.handle);
   if (0 != result)
      exit(result);
}

int main(void) {
   demo_case();
   
   void* money = Money_int__init_1("ANSI C");
   LOG("\nAddress of money is: %p\n", money);
   money = Money_int__create_1("ANSI C");
   LOG("\nAddress of money is: %p\n", money);
   money = Money_int__init_2("ANSI C", 9);
   LOG("\nAddress of money is: %p\n", money);
   money = Money_int__create_2("ANSI C", 7);
   LOG("\nAddress of money is: %p\n", money);
   
   return 0;
}
