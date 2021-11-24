#ifndef C_STRING_H
#define C_STRING_H

#include <stdlib.h>

#define ALLOCATE(buffer, n) \
(buffer) = (char *) malloc (n); \
if ((buffer) == NULL) { \
   LOG("%s", "out of memory: malloc() returns NULL ");  \
}

char * concatenate(const char * first, const char * second);
   
char * to_string_u(unsigned long long x);
char * to_string_i(long long x);
char * to_string_d(long double x);
                  
#define to_string(x) _Generic((x), \
                           short int:              to_string_i, \
                           int:                    to_string_i, \
                           long int:               to_string_i, \
                           long long int:          to_string_i, \
                           unsigned short int:     to_string_u, \
                           unsigned int:           to_string_u, \
                           unsigned long int:      to_string_u, \
                           unsigned long long int: to_string_u, \
                           float:                  to_string_d, \
                           double:                 to_string_d, \
                           long double:            to_string_d  \
                     )(x)     
                     
#endif
