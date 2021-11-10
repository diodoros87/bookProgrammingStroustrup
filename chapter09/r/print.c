#include "print.h"
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

#define CHECK_INPUT_ARG(msg, types) \
   assert_one(NULL != (msg)); \
   assert_one(NULL != (types)); \
   if (! (msg) || ! (types)) { \
      FUNCTION_INFO(__FUNCTION__); \
      LOG("\n error message = %s \n", (msg)); \
      return 0; \
   }
   
char * strchr_wrapper (const char * STR, const char character) {
   if (! STR) {
      FUNCTION_INFO(__FUNCTION__);
      LOG("\n error STR = %p \n", STR);
      return NULL;
   }
   pch=strchr(str,'s');
   while (pch! = NULL) {
      printf ("found at %d\n",pch-str+1);
      pch=strchr(pch+1,'s');
   }
}

const char DELIMITER = '%';

int print_many(const char * msg,   /* message to be printed     */
  const char * types, /* parameter types (i,s,e)     */
  ... )          /* variable arguments     */ {
   va_list arg_list;
   char *first;
   char *second;
   char                 *arg_string;
   char                 arg_char;
   short                arg_short;
   unsigned short       arg_u_short;
   int                  arg_int;
   unsigned int         arg_u_int;
   long                 arg_long;
   unsigned long        arg_u_long;
   long long            arg_long_long;
   unsigned long long   arg_u_long_long;
   float                arg_float;
   double               arg_double;
   long double          arg_long_double;
   size_t index;
   
   CHECK_INPUT_ARG(msg, types);
   LOG("\n %s ", msg );
   va_start( arg_list, types );
   first = types;
   //length = strlen(first);
   second = strchr(types, DELIMITER);
   while( second ) {
      index = second - first;
      switch(index) {
         case 0:
            break;
         case 1:
            if (strncmp(first, "c", index)) {
               LOG( " %c ", va_arg( arg_list, char ) );
            } 
            else if (strncmp(first, "h", index)) {
               LOG( " %h ", va_arg( arg_list, short ) );
            } 
            else if (strncmp(first, "d", index)) {
               LOG( " %d ", arg_int );
            }
            else if (strncmp(first, "u", index)) {
               LOG( " %u ", va_arg( arg_list, unsigned ) );
            } 
            else if (strncmp(first, "f", index)) {
               LOG( " %g ", va_arg( arg_list, float) );
            }
            else if (strncmp(first, "g", index)) {
               LOG( " %g ", va_arg( arg_list, double) );
            }
            else if (strncmp(first, "s", index)) {
               LOG( " %s ", va_arg( arg_list, char*) );
            }
            break;
         case 2:
            if (strncmp(first, "hu", index)) {
               LOG( " %hu ", va_arg( arg_list, unsigned short ) );
            }
            if (strncmp(first, "lu", index)) {
               LOG( " %hu ", va_arg( arg_list, unsigned long ) );
            }
            else if (strncmp(first, "ld", index)) {
               LOG( " %ld ", va_arg( arg_list, long ) );
            }
            else if (strncmp(first, "Lg", index)) {
               LOG( " %Lg ", va_arg( arg_list, long double) );
            }
         case 3:
            if (strncmp(first, "ull", index)) {
               LOG( " %hu ", va_arg( arg_list, unsigned long long ) );
            }
            else {
               LOG( " improper format = %c", *types_ptr);
            }
               
      }
      switch(*types_ptr) {
         case 'i':
            arg_int = va_arg( arg_list, int );
            LOG( " %d ", arg_int );
            break;
         case 's':
            arg_string = va_arg( arg_list, char * );
            LOG( " %s ", arg_string );
            break;
         case 'e':
            arg_long_double = va_arg( arg_list, long double );
            LOG( " %e ", arg_long_double );
            break;
         case 'f':
            arg_double = va_arg( arg_list, double );
            LOG( " %f ", arg_double );
            break;
         case 'c':
            arg_char = va_arg( arg_list, char );
            LOG( " %c ", arg_char );
            break;
         default:
            LOG( " improper format = %c", *types_ptr);
      }
      ++types_ptr;
   }
   va_end( arg_list );
   return 0;
}

int print_assert(const char *file, int line, const char * date, const char * time, const char *msg) {
   LOG("\nAssertion failed:\n \
Expression:\t\n%s\n", msg);
   LOG("Source file:\t\t%s , Line %d\n", file, line);
   LOG("Date:\t\t%s\n", date);
   LOG("Time:\t\t%s\n", time);
   assert(0);
   abort();
   return 0; 
}
