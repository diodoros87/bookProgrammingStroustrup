#include "human_derived.h"
#include "human_private.h"
#include "print.h"
#include "result_codes.h"
#include "c_string.h"
#include "allocate.h"
#include "utility.h"

#include <string.h>

struct Human_derived_t {
   Human_t human;
   unsigned int * age;
};

Human_derived_t* Human_derived_malloc(void) {
   LOG_FUNC(__FUNCTION__);
   Human_derived_t * new = NULL;
   ALLOCATE(new, sizeof (Human_derived_t));
   return new;
}

static Result_codes validate(const unsigned int age) {
   if (age <= 0 || age >= 150) 
      return INVALID_ARG;
   return OK;
}

Result_codes Human_derived_init(Human_derived_t ** const object, const char * const name, const unsigned int age) {
   Result_codes result = Human_init((Human_t **)object, name);
   if (OK != result)
      return result;
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "object is null");
   REQUIRE_NON_NULL(*object, "*object is null - after call of base struct");
   
   result = validate(age);
   if (OK != result)
      return result;
   
   *object = Human_derived_malloc();
   if (! *object)
      return BAD_ALLOC;
   
   (*object)->age = (unsigned int *) copy_bytes(&age, sizeof(unsigned int));/*
   ALLOCATE_SINGLE_TYPE(object->age, unsigned int);*/
   if (NULL == (*object)->age)
      return BAD_ALLOC;
   return OK;
}

void Human_derived_destroy(Human_derived_t ** const object) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "human is null");
   REQUIRE_NON_NULL(*object, "*human is null");
   free((*object)->age); 
   /*Human_destroy_protected((Human_t *) *object);*/
   free(*object);
   *object = NULL; /* to avoid double free or corruption (fasttop)  */
}

Result_codes Human_derived_set_age(Human_derived_t * const object, const unsigned int age) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "human is null");
   REQUIRE_NON_NULL(age, "age is null");
   Result_codes result = validate(age);
   if (OK != result)
      return result;
   
   free(object->age);
   
   object->age = (unsigned int *) copy_bytes(&age, sizeof(unsigned int));/*
   ALLOCATE_SINGLE_TYPE(object->age, unsigned int);*/
   if (NULL == object->age)
      return BAD_ALLOC;
   return OK;
}

Result_codes Human_derived_get_age(const Human_derived_t * const object, unsigned int ** const age) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "human is null");
   REQUIRE_NON_NULL(age, "age is null");
   if (*age) {
      LOG("%s\n", "*age must be null");
      return INVALID_ARG;
   }
   *age = (unsigned int *) copy_bytes(&(object->age), sizeof(unsigned int));/*
   ALLOCATE_SINGLE_TYPE(object->age, unsigned int);*/
   if (NULL == *age)
      return BAD_ALLOC;
   return OK;
}
