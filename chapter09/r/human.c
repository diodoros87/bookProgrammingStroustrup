#include "human.h"
#include "print.h"
#include "result_codes.h"
#include "regular_expr.h"
#include "c_string.h"
#include "singleton.h"

#include <string.h>

#define REQUIRE_NON_NULL(pointer, message) \
if (! (pointer) ) { \
   LOG_EXIT(__FUNCTION__, message, EXIT_FAILURE); \
}

const char * const NAME_REGEX_STRING = "^[[:upper:]][[:lower:]]*( [[:upper:]][[:lower:]]*)?$";

SINGLETON(regex_t, compile_regex, NAME_REGEX_STRING, REG_EXTENDED);

struct Human_t {
   char * name;
};

Human_t* Human_malloc(void) {
   LOG_FUNC(__FUNCTION__);
   Human_t * new = NULL;
   ALLOCATE(new, sizeof (Human_t));
   return new;
}

static Result_codes validate(const char * const name) {
   regex_t * NAME_REGEX = Singleton_regex_t();
   if (! NAME_REGEX)
      return INVALID_ARG;
   Result_codes result = match_regex(NAME_REGEX, name);
   return result;
   /*
   regex_t ** NAME_REGEX = Singleton_regex_t();
   Result_codes result = match_regex(*NAME_REGEX, name);*/
}

Result_codes Human_init(Human_t ** const object, const char * const name) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "human is null");
   REQUIRE_NON_NULL(name, "name is null");
   if (*object) {
      LOG_FUNC(__FUNCTION__);
      LOG("%s\n", "*object must be null");
      return INVALID_ARG;
   }
   /*
   Result_codes result = regex_matches(name, NAME_REGEX_STRING, REG_EXTENDED);*/
   Result_codes result = validate(name);
   if (OK != result)
      return result;
   
   *object = Human_malloc();
   if (! *object)
      return BAD_ALLOC;
   ALLOCATE((*object)->name, strlen(name) + 1);
   if (NULL == (*object)->name)
      return BAD_ALLOC;
   strcpy((*object)->name, name);
   return OK;
}

void Human_destroy(Human_t ** const object) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "human is null");
   REQUIRE_NON_NULL(*object, "*human is null");
   free((*object)->name);
   free(*object);
   *object = NULL; /* to avoid double free or corruption (fasttop)  */
   /*
   regex_t ** NAME_REGEX = Singleton_regex_t();
   regfree(*NAME_REGEX);*/
   regex_t * NAME_REGEX = Singleton_regex_t();
   if (NAME_REGEX) {
      regfree(NAME_REGEX);
      free(NAME_REGEX);
      NAME_REGEX = NULL;
   }
}

Result_codes Human_set(Human_t * const object, const char * const name) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "human is null");
   REQUIRE_NON_NULL(name, "name is null");
   /*
   Result_codes result = regex_matches(name, NAME_REGEX_STRING, REG_EXTENDED);*/
   Result_codes result = validate(name);
   if (OK != result)
      return result;
   
   free(object->name);
   ALLOCATE(object->name, strlen(name) + 1);
   if (NULL == object->name)
      return BAD_ALLOC;
   strcpy(object->name, name);
   return OK;
}

Result_codes Human_get_name(const Human_t * const object, char ** const name) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "human is null");
   REQUIRE_NON_NULL(name, "name is null");
   if (*name) {
      LOG("%s\n", "*name must be null");
      return INVALID_ARG;
   }
   ALLOCATE(*name, strlen(object->name) + 1);
   if (NULL == name)
      return BAD_ALLOC;
   strcpy(*name, object->name);
   return OK;
}
