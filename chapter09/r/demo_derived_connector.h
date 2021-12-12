#ifndef DEMO_DERIVED_CONNECTOR_H 
#define DEMO_DERIVED_CONNECTOR_H 

#include "result_codes.h"

#ifdef __cplusplus
extern "C" {
#endif
   
Result_codes demo_derived_init(const char * const name, const unsigned int age);
Result_codes demo_derived_set_name(const char * name);
Result_codes demo_derived_get_name(char ** const name);
Result_codes demo_derived_set_age(const unsigned int age);
Result_codes demo_derived_get_age(unsigned int * const age);
Result_codes demo_derived_destroy(void);
   
   
#ifdef  __cplusplus
}
#endif
