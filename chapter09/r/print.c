#include "print.h"
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

#define CHECK_INPUT_ARG(msg, types) \
   assert_one(NULL != (msg)); \
   assert_one(NULL != (types)); \
   if (! (msg) || ! (types)) { \
      FUNCTION_INFO(__FUNCTION__); \
      LOG("\n messagefff = %s \n", (msg)); \
      return 0; \
   }

int print_many(const char * msg,   /* message to be printed     */
  const char * types, /* parameter types (i,s)     */
  ... )          /* variable arguments     */ {
   va_list arg_list;
   int   arg_int;
   double   arg_double;
   char *arg_string;
   const char *types_ptr = types;
   
   CHECK_INPUT_ARG(msg, types);
   FUNCTION_INFO(__FUNCTION__);
   LOG("\nmessage = %s \n", msg );
   types_ptr = types;
   va_start( arg_list, types );
   while( types_ptr && *types_ptr != '\0' ) {
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
            arg_double = va_arg( arg_list, double );
            LOG( " %e ", arg_double );
            break;
         default:
            LOG( " improper format = %c", *types_ptr);
      }
      ++types_ptr;
   }
   va_end( arg_list );
   return 0;
}
/*
void print_many(char * string...) {
   va_list list;
   va_start (list)
}
*/

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
