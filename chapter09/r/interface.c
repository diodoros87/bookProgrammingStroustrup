#include "interface_private.h"
#include "interface.h"

#include "print.h"
#include "result_codes.h"
#include "c_string.h"
#include "utility.h"

#include <string.h>

#define MUST_NULL(pointer, pointer_name) \
if (NULL == pointer) { \
   LOG_FUNC(__FUNCTION__); \
   LOG("%s must be null\n", pointer_name); \
   return INVALID_ARG; \
}

const int NB = -8;
const char * const class_name = "Interface";

Interface_t * Interface_malloc(void) {
   LOG_FUNC(__FUNCTION__);
   Interface_t * new = NULL;
   ALLOCATE_STRING(new, sizeof (Interface_t));
   return new;
}

Result_codes Interface_init(Interface_t ** const object) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "interface is null");
   MUST_NULL(*object, "*interface");
   
   *object = Interface_malloc();
   if (! *object)
      return BAD_ALLOC;
   
   (*object)->pv_valid_f   = NULL;
   (*object)->pv_number_f  = NULL;
   (*object)->pv_char_f    = NULL;
   (*object)->interface_destroy = Interface_destroy;
   
   return OK;
}

Result_codes pv_valid(const Interface_t * const object, bool_t * const b) {
   return object->pv_valid_f(object, b);
}

Result_codes pv_number(const Interface_t * const object, int * const number) {
   return object->pv_number_f(object, number);
}

Result_codes pv_char(const Interface_t * const object, char * const ch) {
   return object->pv_char_f(object, ch);
}

void Interface_destroy(Interface_t ** const object) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "interface is null");
   REQUIRE_NON_NULL(*object, "*interface is null");
   
   free(*object);
   *object = NULL; /* to avoid double free or corruption (fasttop)  */
}
