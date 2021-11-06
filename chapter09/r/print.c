#include "print.h"
#include <stdarg.h>
#include <stdlib.h>

int print_many(
  const char * msg,   /* message to be printed     */
  const char * types, /* parameter types (i,s)     */
  ... )          /* variable arguments     */
{
   va_list arg_list;
   int   arg_int;
   char *arg_string;
   const char *types_ptr = types;

   types_ptr = types;
   LOG("\n%s -- %s\n", msg, types );
   va_start( arg_list, types );
   while( *types_ptr != '\0' ) {
      if (*types_ptr == 'i') {
         arg_int = va_arg( arg_list, int );
         LOG( "integer: %d\n", arg_int );
      } else if (*types_ptr == 's') {
         arg_string = va_arg( arg_list, char * );
         LOG( "string:  %s\n", arg_string );
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
   abort();
   /*LOG("  Info: %s\n", info) ;
   
   //assert(0);
   //return 0; */
}
