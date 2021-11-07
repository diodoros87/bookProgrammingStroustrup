#ifndef CONNECTOR_H 
#define CONNECTOR_H 

typedef enum {
   INIT_1                 = 0,
   CREATE_1               = 1,
   INIT_2                 = 2,
   CREATE_2               = 3
} Money_functions;

typedef enum {
   OK                 = 0,
   BAD_ALLOC          = 1,
   BAD_FUNTION_CALL   = 2,
   INVALID_ARG        = 3,
   BAD_CAST           = 4,
   REGEX_ERROR        = 5,
   STD_ERROR          = 55,
   UNRECOGNIZED_ERROR = -38
} Result_codes;

#ifdef __cplusplus
 #define EXTERNC extern "C"
 #else
 #define EXTERNC
 #endif
 
EXTERNC void set_handler(void (*pfunc)(void));

typedef void* Money_type ;

EXTERNC Result_codes Money_type__function(Money_type * money_ptr, const Money_functions function, char * dollars, ... );

EXTERNC Result_codes Money_type__init_1(Money_type * money_ptr, const char * dollars);
EXTERNC Result_codes Money_type__create_1(Money_type * money_ptr, const char * dollars);
EXTERNC Result_codes Money_type__init_2(Money_type * money_ptr, const char * dollars, const long double cents);
EXTERNC Result_codes Money_type__create_2(Money_type * money_ptr, const char * dollars, const long double cents);

EXTERNC Result_codes demo_init(const char * name);
EXTERNC Result_codes demo_set_name(const char * name);
EXTERNC Result_codes demo_get_name(char ** name);
EXTERNC Result_codes demo_destroy();

#undef EXTERNC

typedef struct {
   Result_codes (*init)(const char * );
   Result_codes (*set_name)(const char * );
   Result_codes (*get_name) (char ** );
   Result_codes (*destroy)();
   void * handle;
} Demo_functions;

#endif
