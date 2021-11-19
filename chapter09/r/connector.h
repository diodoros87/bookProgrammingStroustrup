#ifndef CONNECTOR_H 
#define CONNECTOR_H 

typedef enum {
   INIT_1                 = -101,
   CREATE_1               = -102,
   INIT_2                 = -103,
   CREATE_2               = -104
} Money_functions;

typedef enum {
   OPEN_FILE_ERROR    = -4,
   RENAME_FILE_ERROR  = -3,
   FILE_CLOSE_ERROR   = -2,
   SYSTEM_ERROR       = -1,
   OK                 = 0,
   BAD_ALLOC          = 1,
   BAD_FUNTION_CALL   = 2,
   INVALID_ARG        = 3,
   BAD_CAST           = 4,
   REGEX_ERROR        = 5,
   OUT_OF_RANGE_ERROR = 6,
   RUNTIME_ERROR      = 7,
   STD_ERROR          = 8,
   UNRECOGNIZED_ERROR = 9,
   CHILD_PROCESS_FAIL = 127
} Result_codes;

typedef enum {
   SHORT       = 49,
   U_SHORT     = 50,
   INT         = 51,
   U_INT       = 52,
   LONG        = 53,
   U_LONG      = 54,
   LONG_LONG   = 55,
   U_LONG_LONG = 56,
   FLOAT       = 57,
   DOUBLE      = 58,
   LONG_DOUBLE = 59
} Number;

union Number_pointer_union {
   short                s;
   unsigned short       us;
   int                  i;
   unsigned int         ui;
   long                 l;
   unsigned long        ul;
   long long            ll;
   unsigned long long   ull;
   float                f;
   double               d;
   long double          ld;
};

#ifdef __cplusplus
 #define EXTERNC extern "C"
 #else
 #define EXTERNC
 #endif
 
EXTERNC void set_handler(void (*pfunc)(void));

typedef void* Money_type ;

EXTERNC Result_codes Money_type__function(Money_type * money_ptr, const Money_functions function, const Number type, 
                                  union Number_pointer_union * const n_union, const char * dollars, ... );

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
