#include "human_functions.h"

typedef struct {
   Result_codes (*init)(Human_t *, const char * const);
   Result_codes (*set_name)(Human_t * const, const char * const);
   Result_codes (*get_name) (const Human_t * const, char **);
   Result_codes (*destroy)(Human_t * const);
   void * handle;
} Human_functions;

static void load_human(Human_functions * functions) {
   if (! Human_functions) {
      FUNCTION_INFO(__FUNCTION__);
      LOG("%s\n", "functions is NULL ");
      longjmp(JMP_BUF, 3);
   }s
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
   functions->handle = get_handle(LIB_HUMAN_SO, RTLD_LAZY);
   functions->init = get_symbol(functions->handle, "Human_init");
   functions->set_name = get_symbol(functions->handle, "Human_set");
   functions->get_name = get_symbol(functions->handle, "Human_get_name");
   functions->destroy = get_symbol(functions->handle, "Human_destroy");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
   functions->handle = NULL;
   functions->init = demo_init;
   functions->set_name = demo_set_name;
   functions->get_name = demo_get_name;
   functions->destroy = demo_destroy;
#endif
}

static Result_codes run_human(const Human_functions * const functions) { 
   if (! functions) {
      LOG_EXIT(__FUNCTION__, "human functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
   Human_t human;
   Result_codes result = functions->init(&human, "Claudius Ptolemaeus"); 
   if (OK == result) {
      char * name = NULL;
      result = functions->get_name(&human, &name);
      if (OK == result) {
         LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
         name = NULL;
         functions->set_name(&human, "Plato");
         result = functions->get_name(&human, &name);
         if (OK == result) {
            LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
            result = functions->destroy(&human);
#ifdef MANUAL_DLL_LOAD
            if (OK == result) {
               result = close_handle(&(functions->handle));
               assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
               return result;
            }
#endif
         }
      }
   }
#ifdef MANUAL_DLL_LOAD
   close_handle(&(functions->handle));
#endif
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

Result_codes test_human(void) {
   Human_functions functions;
   load_human(&functions); 
   Result_codes result = run_human(&functions);
   return result;
}
