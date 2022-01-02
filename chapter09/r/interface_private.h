#ifndef INTERFACE_PRIVATE_H
#define INTERFACE_PRIVATE_H

#include "interface.h"

typedef Result_codes (*pv_valid_t) (const Interface_t * const, const double, bool_t * const);
typedef Result_codes (*pv_number_t) (const Interface_t * const, int * const);
typedef Result_codes (*pv_char_t) (const Interface_t * const, char * const);

typedef void (*Interface_destroy_t) (Interface_t ** const);

typedef struct Interface_t {
   pv_valid_t pv_valid_f;
   pv_number_t pv_number_f;
   pv_char_t pv_char_f;
   Interface_destroy_t interface_destroy_f;
} Interface_t;

Interface_t * Interface_malloc(void);

Result_codes Interface_init(Interface_t ** const);

Result_codes pv_valid(const Interface_t * const, const double, bool_t * const);

#endif
