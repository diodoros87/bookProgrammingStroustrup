#include "human.h"
#include "print.h"
#include "result_codes.h"
#include "regular_expr.h"
#include "c_string.h"

#include <string.h>

#define REQUIRE_NON_NULL(pointer) \
if (! (pointer) ) { \
   LOG_EXIT(__FUNCTION__, "Pointer is NULL", EXIT_FAILURE); \
}
/*
const regex_t * const set_regex() {
   static regex NAME_REGEX = get_regex("(^[[:upper:]][[:lower:]]*( [[:upper:]][[:lower:]]*)?$)", REG_EXTENDED);
   return NAME_REGEX;
}

const regex_t * const NAME_REGEX = set_regex();
*/

const char * const NAME_REGEX = "^[[:upper:]][[:lower:]]*( [[:upper:]][[:lower:]]*)?$";

struct Human_t {
   char * name;
};

Human_t* Human_malloc() {
   LOG_FUNC(__FUNCTION__);
   Human_t * new = NULL;
   ALLOCATE(new, sizeof (Human_t));
   return new;
}

Result_codes Human_init(Human_t ** object, const char * name) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object);
   if (*object) {
      LOG_FUNC(__FUNCTION__);
      LOG("%s\n", "*object must be null");
      return INVALID_ARG;
   }
   Result_codes result = regex_matches(name, NAME_REGEX, REG_EXTENDED);
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

void Human_destroy(Human_t ** object) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object);
   REQUIRE_NON_NULL(*object);
   free((*object)->name);
   free(*object);
   *object = NULL; /* to avoid double free or corruption (fasttop)  */
}

Result_codes Human_set(Human_t * object, const char * name) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object);
   Result_codes result = regex_matches(name, NAME_REGEX, REG_EXTENDED);
   if (OK != result)
      return result;
   free(object->name);
   ALLOCATE(object->name, strlen(name) + 1);
   if (NULL == object->name)
      return BAD_ALLOC;
   strcpy(object->name, name);
   return OK;
}

Result_codes Human_get_name(Human_t * object, char ** name) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object);
   REQUIRE_NON_NULL(name);
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
