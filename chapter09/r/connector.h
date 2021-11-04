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

void AAA_sayHi(const char *name);

#undef EXTERNC

#endif
 /*
mylibrary_mytype_t mylibrary_mytype_init() {
   return new MyType;
}

void mylibrary_mytype_destroy(mylibrary_mytype_t untyped_ptr) {
   MyType* typed_ptr = static_cast<MyType*>(untyped_ptr);
   delete typed_ptr;
}

void mylibrary_mytype_doit(mylibrary_mytype_t untyped_self, int param) {
   MyType* typed_self = static_cast<MyType*>(untyped_self);
   typed_self->doIt(param);
} */
