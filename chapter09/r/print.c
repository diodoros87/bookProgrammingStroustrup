#include "print.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define CHECK_INPUT_ARG(msg, types) \
   assert_one(NULL != (msg)); \
   assert_one(NULL != (types)); \
   if (! (msg) || ! (types)) { \
      FUNCTION_INFO(__FUNCTION__); \
      LOG("\n error message = %s \n", (msg)); \
      return 0; \
   }   

const char DELIMITER = ' ';

static int print_error (char * string, const size_t size) {
   size_t index;
   if (NULL == string) {
      LOG_EXIT(__FUNCTION__, "\n error string = NULL \n", EXIT_FAILURE);  /* brackets - multiline macro */
   }
   LOG( " improper format = \'%s", "");
   for (index = 0; *string != '\0' && index < size; index++) {
      LOG( "%c", *string);
   }
   LOG( "%c", '\'');
}

int print_many(const char * msg,  const char * types, ... ) {
   va_list arg_list;
   char *first;
   char *second;/*
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
   long double          arg_long_double;*/
   float                arg_float;
   size_t index;
   FUNCTION_INFO(__FUNCTION__);
   CHECK_INPUT_ARG(msg, types);
   LOG("\n %s ", msg );
   va_start( arg_list, types );
   first = types;
   do {
      second = strchr(first, DELIMITER);
      index = (second == NULL) ? 1 : second - first;
      LOG( " index = %u ", index );
      LOG( " first = %p ", first );
      LOG( " second = %p ", second );
      switch(index) {
         case 0:
            break;
         case 1:
            if (0 == strncmp(first, "c", index)) {
               LOG( " %c ", va_arg( arg_list, char ) );
            } 
            else if (0 == strncmp(first, "h", index)) {
               LOG( " %h ", va_arg( arg_list, short ) );
            } 
            else if (0 == strncmp(first, "d", index)) {
               LOG( " %d ", va_arg( arg_list, int ) );
            }
            else if (0 == strncmp(first, "u", index)) {
               LOG( " %u ", va_arg( arg_list, unsigned ) );
            } 
            else if (0 == strncmp(first, "f", index)) {
               LOG( " %f ", va_arg( arg_list, float) );
            }
            else if (0 == strncmp(first, "g", index)) {
               LOG( " %f ", va_arg( arg_list, double) );
            }
            else if (0 == strncmp(first, "s", index)) {
               LOG( " %s ", va_arg( arg_list, char*) );
            }
            else if (0 == strncmp(first, "p", index)) {
               LOG( " %p ", va_arg( arg_list, void*) );
            }
            else 
               print_error( first, index);
            break;
         case 2:
            if (0 == strncmp(first, "uh", index)) {
               LOG( " %hu ", va_arg( arg_list, unsigned short ) );
            }
            if (0 == strncmp(first, "ul", index)) {
               LOG( " %hu ", va_arg( arg_list, unsigned long ) );
            }
            else if (0 == strncmp(first, "ld", index)) {
               LOG( " %ld ", va_arg( arg_list, long ) );
            }
            else if (0 == strncmp(first, "Lg", index)) {
               LOG( " %Lg ", va_arg( arg_list, long double) );
            }
            else 
               print_error( first, index);
            break;
         case 3:
            if (0 == strncmp(first, "ull", index)) {
               LOG( " %ull ", va_arg( arg_list, unsigned long long ) );
            }
            else 
               print_error( first, index);
            break;
         default:
            print_error( first, index);
      }
      LOG( " 2 second = %p ", second );
      if (NULL == second || *(second + 1) == '\0')
         break;
      else
         first = second + 1;
   } while (first != NULL);
   va_end( arg_list );
   return 0;
}

#undef CHECK_INPUT_ARG

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
