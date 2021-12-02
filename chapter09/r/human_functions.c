#include "human_functions.h"
#include "print.h"
#include "utility.h"
#include "human.h"

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_HUMAN_SO     "libhuman.so"
#endif

struct Human_functions {
   Result_codes (*init)(Human_t **, const char * const);
   Result_codes (*set_name)(Human_t * const, const char * const);
   Result_codes (*get_name) (const Human_t * const, char **);
   Result_codes (*destroy)(Human_t ** const);
   void * handle;
};

const int functions_null = -85;

static void load_human(Human_functions * functions) {
   if (! functions) {
      FUNCTION_INFO(__FUNCTION__);
      LOG("%s\n", "human_functions is NULL ");
      longjmp(JMP_BUF, functions_null);
   }
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
   functions->init = Human_init;
   functions->set_name = Human_set;
   functions->get_name = Human_get_name;
   functions->destroy = Human_destroy;
#endif
}

static Result_codes run_human(const Human_functions * const functions) { 
   if (! functions) {
      LOG_EXIT(__FUNCTION__, "human functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
   Human_t * human = NULL;
   Result_codes result = functions->init(&human, "Claudius Ptolemaeus"); 
   if (OK == result) {
      char * name = NULL;
      result = functions->get_name(human, &name);
      if (OK == result) {
         LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
         free(name);
         result = functions->set_name(human, "Plato");
         if (result == OK) {
            name = NULL;
            result = functions->get_name(human, &name);
            if (OK == result) {
               LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
               free(name);
               functions->destroy(&human);
               assert_many(human == NULL, "assert failed: ", "s p", "pointer to human == ", human);
#ifdef MANUAL_DLL_LOAD
               result = close_handle(&(functions->handle));
               assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
               return result;
#endif
            }
         }
      }
   }
#ifdef MANUAL_DLL_LOAD
   close_handle(&(functions->handle));
#endif
   if (human)
      functions->destroy(&human);
   assert_many(human == NULL, "assert failed: ", "s p", "pointer to human == ", human);
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

Result_codes test_human(void) {
   Human_functions functions;
   load_human(&functions); 
   Result_codes result = run_human(&functions);
   return result;
}

Result_codes test_human_linking(void) {
   volatile int jmp_value = 0;
   const int jmp_value_2 = -7;
   if ((jmp_value = setjmp(JMP_BUF)) != 0) {
      FUNCTION_INFO(__FUNCTION__);
      LOG("\nAfter calling test_print_many longjmp set value to %d\n", jmp_value);
      assert_many(jmp_value_2 == jmp_value, "assert failed: ", "s d", "jmp_value should be ", jmp_value_2);
      if ((jmp_value = setjmp(JMP_BUF)) != 0) {
         FUNCTION_INFO(__FUNCTION__);
         LOG("\nlongjmp set value to %d\n ", jmp_value);
         assert_many(functions_null == jmp_value, "assert failed: ", "s d", "jmp_value should be ", functions_null);
      }
      else 
         load_human(NULL);

      return test_human(); 
   }
   else 
      longjump_test(JMP_BUF, jmp_value_2);
}
