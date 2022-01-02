#ifndef INTERFACE_H
#define INTERFACE_H

#include "result_codes.h"
#include "print.h"

typedef int bool_t;

struct Interface_t;

typedef struct Interface_t Interface_t;

#ifdef __cplusplus
extern "C" {
#endif

void Interface_destroy(Interface_t ** const);

Result_codes pv_number(const Interface_t * const, int * const);

Result_codes pv_char(const Interface_t * const, char * const);

#ifdef  __cplusplus
}
#endif

extern const int NB;
extern const char * const class_name;

#endif
