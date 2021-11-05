#ifndef CONNECTOR_H 
#define CONNECTOR_H 

#ifdef __cplusplus
 #define EXTERNC extern "C"
 #else
 #define EXTERNC
 #endif

typedef void* Money_int;

EXTERNC Money_int Money_int__init_1(const char * dollars);
EXTERNC Money_int Money_int__create_1(const char * dollars);
EXTERNC Money_int Money_int__init_2(const char * dollars, const long double cents);
EXTERNC Money_int Money_int__create_2(const char * dollars, const long double cents);

EXTERNC void demo_init(const char * name);
EXTERNC void demo_set_name(const char * name);
EXTERNC const char * demo_get_name();
EXTERNC void demo_destroy();

#undef EXTERNC

#ifdef MANUAL_DLL_LOAD
typedef struct {
   void (*init)(const char * );
   void (*set_name)(const char * );
   const char * (*get_name) ();
   void (*destroy)();
   void * handle;
} Demo_functions;

typedef struct {
   void (*create_1)(const char * );
   void (*create_2)(const char * , const long double);
   void * handle;
} Money_functions;
#endif

#endif
