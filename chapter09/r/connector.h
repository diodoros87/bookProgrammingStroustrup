#ifndef CONNECTOR_H 
#define CONNECTOR_H 

#ifdef __cplusplus
 #define EXTERNC extern "C"
 #else
 #define EXTERNC
 #endif
 
 EXTERNC void set_handler(void (*pfunc)(void));

typedef void* Money_int;

EXTERNC Money_int Money_int__init_1(const char * dollars);
EXTERNC Money_int Money_int__create_1(const char * dollars);
EXTERNC Money_int Money_int__init_2(const char * dollars, const long double cents);
EXTERNC Money_int Money_int__create_2(const char * dollars, const long double cents);

EXTERNC int demo_init(const char * name);
EXTERNC int demo_set_name(const char * name);
EXTERNC int demo_get_name(char ** name);
EXTERNC int demo_destroy();

#undef EXTERNC

typedef enum {
   OK                 = 0,
   BAD_ALLOC          = 1,
   BAD_FUNTION_CALL   = 2,
   INVALID_ARG        = 3,
   BAD_CAST           = 4,
   STD_ERROR          = 55,
   UNRECOGNIZED_ERROR = -38
} Result_codes;

#ifdef MANUAL_DLL_LOAD
typedef struct {
   int (*init)(const char * );
   int (*set_name)(const char * );
   int (*get_name) (char ** );
   int (*destroy)();
   void * handle;
} Demo_functions;

typedef struct {
   void (*create_1)(const char * );
   void (*create_2)(const char * , const long double);
   void * handle;
} Money_functions;
#endif

#endif
