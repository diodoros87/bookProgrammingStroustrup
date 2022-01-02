#include "print.h"
#include "abstract.h"
#include "abstract_private.h"
#include "c_string.h"
#include "allocate.h"
#include "utility.h"

#include <string.h>
#include <math.h>

#define MUST_NULL(pointer, pointer_name) \
if (NULL == pointer) { \
   LOG_FUNC(__FUNCTION__); \
   LOG("%s must be null\n", pointer_name); \
   return INVALID_ARG; \
}

const char * const class_name = "Abstract";
const int ABSTRACT = 0;

static Result_codes pv_number_local(const Abstract_t * const ptr, int * const n) {
   return ABSTRACT; 
}

int number(const Abstract_t * const ptr) { 
   REQUIRE_NON_NULL(ptr, "abstract is null");
   return ABSTRACT; 
}

int number(const Abstract_t * const ptr) { 
   REQUIRE_NON_NULL(ptr, "abstract is null");
   return ABSTRACT; 
}

Result_codes Abstract_init(Abstract_t ** const object, const double x) {
   Result_codes result = Interface_init((Interface_t **)object);
   if (OK != result)
      return result;
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   REQUIRE_NON_NULL(*object, "*abstract is null - after call of base struct");
   
   result = validate(age);
   if (OK != result)
      return result;
   
   REALLOCATE_STRING(*object, sizeof (Abstract_t));
   if (! *object)
      return BAD_ALLOC;
   
   (*object)->x = (unsigned int *) copy_bytes(&x, sizeof(double));
   if (NULL == (*object)->x)
      return BAD_ALLOC;
   
   (*object)->interface.pv_number_f  = pv_number_local;
   /* (*object)->interface.pv_char_f    = NULL; */
   (*object)->interface_destroy_f = Abstract_destroy;
   (*object)->abstract_destroy_f  = Abstract_destroy;
   return OK;
}

Result_codes pv_valid(const Abstract_t * const object, const double n, bool_t * const b) {
   return object->pv_valid_f(object, n, b);
}

Result_codes pv_number(const Abstract_t * const object, int * const number) {
   return object->pv_number_f(object, number);
}

Result_codes pv_char(const Abstract_t * const object, char * const ch) {
   return object->pv_char_f(object, ch);
}

void Abstract_destroy(Abstract_t ** const object) {
   LOG_FUNC(__FUNCTION__);
   REQUIRE_NON_NULL(object, "interface is null");
   REQUIRE_NON_NULL(*object, "*interface is null");
   free((*object)->x);
   Interface_destroy((Interface_t **) object);
   /*free(*object);*/
   *object = NULL; /* to avoid double free or corruption (fasttop)  */
}

Result_codes validate(const double number, const char * const function) {
   LOG_FUNC(__FUNCTION__); 
   if (! isfinite(number)) {
      LOG(" argument of number: %d is not finite", number, str, regex);
      return INVALID_ARG;
   }
}

void Abstract::virt_set_X(const double number) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__) << '\n';
   validate(number, __func__);
   this->x = number;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, this->x) << '\n';
}

double Abstract::virt_area() const {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__) << '\n';
   return x;
}

Abstract::Abstract(const double number) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__) << '\n';
   validate(number, __func__);
   this->x = number;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, this->x) << '\n';
}

Abstract::~Abstract() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, this->x) << '\n';
}

