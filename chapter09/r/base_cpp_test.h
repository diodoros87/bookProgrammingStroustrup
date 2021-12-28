#ifndef BASE_CPP_TEST_H
#define BASE_CPP_TEST_H

#include "pair.h"
#include "result_codes.h"

typedef struct __attribute__ ((__packed__)) Interface_connector {
   Result_codes (*pv_number) (int * const);
   Result_codes (*pv_char) (char * const);
} Interface_connector;

typedef struct __attribute__ ((__packed__)) Abstract_connector {
   Interface_connector interface;
   Result_codes (*pv_X)(double * const );
   Result_codes (*virt_set_X) (const double);
   Result_codes (*pv_Y)(double * const );
   Result_codes (*virt_set_Y) (const double);
   Result_codes (*virt_area)(double * const);
   Result_codes (*number) (int * const);
} Abstract_connector;

typedef struct __attribute__ ((__packed__)) Base_connector {
   Abstract_connector abstract;
   Result_codes (*init)(const double, const double);
   Result_codes (*destroy)(void);
   void * handle;
} Base_connector;

// typedef struct Interface_expected {
//    pair_int_str pv_number = {"pv_number", 0};
//    pair_char_str pv_char = {"pv_char", '0'};
// } Interface_expected;

Result_codes test_base_cpp(void);

#endif
