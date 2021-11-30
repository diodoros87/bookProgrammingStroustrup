#include "demo_functions.h"


typedef struct {
   Result_codes (*init)(const char * );
   Result_codes (*set_name)(const char * );
   Result_codes (*get_name) (char ** );
   Result_codes (*destroy)(void);
   void * handle;
} Demo_functions;

void load_demo(Demo_functions * demo_functions) {
   if (! demo_functions) {
      FUNCTION_INFO(__FUNCTION__);
      LOG("%s\n", "demo_functions is NULL ");
      longjmp(JMP_BUF, 3);
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

Result_codes run_demo(const Demo_functions * const demo_functions) { 
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
#ifdef MANUAL_DLL_LOAD
            if (OK == result) {
               result = close_handle(&(demo_functions->handle));
               assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
               return result;
            }
#endif
         }
      }
   }
#ifdef MANUAL_DLL_LOAD
   close_handle(&(demo_functions->handle));
#endif
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

Result_codes test_demo(void) {
   Demo_functions functions;
   load_human(&functions); 
   Result_codes result = run_human(&functions);
   return result;
}
