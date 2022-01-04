#include "interface.h"
#include "abstract.h"
#include "abstract_private.h"
#include "print.h"
#include "c_string.h"
#include "allocate.h"
#include "utility.h"

#include <string.h>
#include <math.h>

const char * const abstract_class_name = "Abstract";
const int ABSTRACT = 0;

void Abstract_destroy_local(Abstract_t ** const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   REQUIRE_NON_NULL(*object, "*abstract is null");
   
   free((*object)->x);
   Interface_destroy_local((Interface_t **) object);
   /*free(*object);*/
   *object = NULL; /* to avoid double free or corruption (fasttop)  */
}

static int pv_number_local(const Abstract_t * const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   
   return ABSTRACT; 
}

static double X_local(const Abstract_t * const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   REQUIRE_NON_NULL(object->x, "pointer to x is null");
   
   return *(object->x);
}

Result_codes virt_set_X_local(Abstract_t * const object, const double number) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   REQUIRE_NON_NULL(object->x, "pointer to x is null");
   
   Result_codes result = Abstract_validate(number, __FUNCTION__);
   if (OK == result)
      *(object->x) = number;
   return result;
}

static double virt_area_local(const Abstract_t * const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   REQUIRE_NON_NULL(object->x, "pointer to x is null");
   
   return *(object->x);
}

int Abstract_number(const Abstract_t * const ptr) { 
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(ptr, "abstract is null");
   
   return ABSTRACT; 
}

Result_codes Abstract_init(Abstract_t ** const object, const double x) {
   Result_codes result = Interface_init((Interface_t **)object);
   if (OK != result)
      return result;
   
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   REQUIRE_NON_NULL(*object, "*abstract is null - after call of base struct");
   
   REALLOCATE_STRING(*object, sizeof (Abstract_t));
   if (! *object)
      return BAD_ALLOC;
   
   (*object)->pv_Y_f  = NULL;
   (*object)->X_f  = X_local;
   (*object)->virt_set_X_f = virt_set_X_local;
   (*object)->virt_area_f = virt_area_local;
   (*object)->interface.pv_number_f  = pv_number_local;
   /* (*object)->interface.pv_char_f    = NULL; */
   /* (*object)->interface.pv_valid_f    = NULL; */
   (*object)->interface.interface_destroy_f = Abstract_destroy_local;
   (*object)->abstract_destroy_f  = Abstract_destroy_local;
   
   result = Abstract_validate(x, __FUNCTION__);
   if (OK == result) {
      (*object)->x = (double *) copy_bytes(&x, sizeof(double));
      if (NULL == (*object)->x)
         return BAD_ALLOC;
   }

   return result;
}

double pv_Y(const Abstract_t * const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   
   return object->pv_Y_f(object);
}

double X(const Abstract_t * const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   
   return object->X_f(object);
}

double virt_area(const Abstract_t * const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   
   return object->virt_area_f(object);
}

Result_codes virt_set_X(Abstract_t * const object, const double n) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   
   return object->virt_set_X_f(object, n);
}

void Abstract_destroy(Abstract_t ** const object) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(object, "abstract is null");
   REQUIRE_NON_NULL(*object, "*abstract is null");
   
   return (*object)->abstract_destroy_f(object);
}

Result_codes Abstract_validate(const double number, const char * const function) {
   LOG("%s %s \n", abstract_class_name, __FUNCTION__);
   REQUIRE_NON_NULL(function, "function is null");
   
   if (! isfinite(number)) {
      LOG(" %s: argument of number: %f is not finite \n", function, number);
      return INVALID_ARG;
   }
   return OK;
}