#include "human_derived_functions.h"
#include "print.h"
#include "utility.h"
#include "human.h"

#include <string.h>

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "shared_lib_open.h"
   #define LIB_HUMAN_SO     "libhumanderived.so"
#endif
/*
#define print_and_assert(value, expected_value, value_string, expression, format) \
   LOG("%s: %s %s = %s", LANGUAGE, __FUNCTION__, value_string, #value); \
   assert_many((expression), "assert failed: ", format, name, " == ", name)
*/
static void load_human(Human_derived_functions * functions) {
   if (! functions) {
      LOG_EXIT(__FUNCTION__, "human functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
#ifdef MANUAL_DLL_LOAD
   LOG("%s", "\nMANUAL DLL LOAD\n");
   functions->handle = get_handle(LIB_HUMAN_SO, RTLD_LAZY);
   functions->init = get_symbol(functions->handle, "Human_derived_init");/*
   functions->set_name = get_symbol(functions->handle, "Human_derived_set_age");
   functions->get_name = get_symbol(functions->handle, "Human_derived_get_age");*/
   functions->destroy = get_symbol(functions->handle, "Human_derived_destroy");
#else
   LOG("%s", "\nAUTOMATIC DLL LOAD\n");
   functions->handle = NULL;
   functions->init = Human_derived_init;/*
   functions->set_name = Human_derived_set_age;
   functions->get_name = Human_derived_get_name;*/
   functions->destroy = Human_derived_destroy;
#endif
}

static Result_codes check_name(const Human_derived_t * human, const char * expected_name) {
   REQUIRE_NON_NULL(human, "human derived is null");
   REQUIRE_NON_NULL(expected_name, "expected_name is null");
   char * name = NULL;
   Result_codes result = Human_get_name((Human_t *)human, &name);
   if (OK == result) {
      LOG("%s: %s human name = %s", LANGUAGE, __FUNCTION__, name);
      assert_many(strcmp(name, expected_name) == 0, "assert failed: ", "s s", "name == ", name);
   }
   free(name);
   return result;
}

static Result_codes check_age(const Human_derived_t * human, const unsigned int expected) {
   REQUIRE_NON_NULL(human, "human derived is null");
   unsigned int * age = NULL;
   Result_codes result = Human_derived_get_age(human, &age);
   if (OK == result) {
      LOG("%s: %s human name = %u", LANGUAGE, __FUNCTION__, *age);
      assert_many(*age == expected, "assert failed: ", "s u", " *age == ", *age);
   }
   free(age);
   return result;
}

static Result_codes run_human(const Human_derived_functions * const functions) { 
   if (! functions) {
      LOG_EXIT(__FUNCTION__, "human functions is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
   Human_derived_t * human = NULL;
   Result_codes result = functions->init(&human, "Claudius Ptolemaeus", 9); 
   if (OK == result)
      result = check_name(human, "Claudius Ptolemaeus"); 
   if (OK == result)
      result = check_age(human, 9); 
   if (OK == result)
      result = Human_set((Human_t *)human, "Plato");
   if (OK == result)
      result = check_name(human, "Plato"); 
   if (OK == result)
      result = Human_derived_set_age(human, 84);
   if (OK == result)
      result = check_age(human, 84); 
#ifdef MANUAL_DLL_LOAD
   if (OK == result) {
      functions->destroy(&human);
      assert_many(human == NULL, "assert failed: ", "s p", "pointer to human == ", human);
      result = close_handle(&(functions->handle));
      assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
      return result;
   }
   close_handle(&(functions->handle));
#endif
   if (human)
      functions->destroy(&human);
   assert_many(human == NULL, "assert failed: ", "s p", "pointer to human == ", human);
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}

Result_codes test_human_derived(void) {
   FUNCTION_INFO(__FUNCTION__);
   Human_derived_functions functions;
   load_human(&functions); 
   Result_codes result = run_human(&functions);
   return result;
}
