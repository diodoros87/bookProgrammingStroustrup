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

static void print_error (char * string, const size_t size) {
   size_t index;
   if (NULL == string) {
      LOG_EXIT(__FUNCTION__, "\n error string = NULL \n", EXIT_FAILURE);  /* brackets - multiline macro */
   }
   LOG( " improper format = \'%s", "");
   for (index = 0; *string != '\0' && index < size; index++) {
      LOG( "%c", *(string + index));
   }
   LOG( "%c", '\'');
}

static void case_1 (const char * first, const char * types, const size_t index, va_list args) ;

int print_many(const char * msg,  const char * types, ... ) {
   va_list arg_list;
   char *first;
   char *second; /* c-string started in DELIMITER index  */
   size_t index;  /* index of DELIMITER in second c-string */
   FUNCTION_INFO(__FUNCTION__);
   CHECK_INPUT_ARG(msg, types);
   LOG("\n %s ", msg );
   va_start( arg_list, types );
   first = types;
   do {
      second = strchr(first, DELIMITER);
      if (NULL == second)   /* no DELIMITER in first c-string */
         index = strlen(first);   /* whole first c-string */
      else
         index = second - first;  /* c-string started in DELIMITER index  */
      switch(index) {
         case 0:
            break;
         case 1:
            if (0 == strncmp(first, "c", index)) {
               LOG( " %c ", va_arg( arg_list, int ) );  /*  can't use va_arg(varlist, char) because the types are promoted to int; 
               using of va_arg(varlist, int) instead */
            } 
            else if (0 == strncmp(first, "d", index)) {
               LOG( " %d ", va_arg( arg_list, int ) );
            }
            else if (0 == strncmp(first, "u", index)) {
               LOG( " %u ", va_arg( arg_list, unsigned ) );
            } 
            else if (0 == strncmp(first, "f", index)) {
               LOG( " %f ", va_arg( arg_list, double) );  /*  can't use va_arg(varlist, float) because the types are promoted to double; 
               using of va_arg(varlist, double) instead */
            }
            else if (0 == strncmp(first, "g", index)) {
               LOG( " %g ", va_arg( arg_list, double) );
            }
            else if (0 == strncmp(first, "s", index)) {
               LOG( " %s ", va_arg( arg_list, char*) );
            }
            else if (0 == strncmp(first, "p", index)) {
               LOG( " %p ", va_arg( arg_list, void*) );  /* many (all ?) pointers in va_arg can be cast to void* */
            }
            else 
               print_error( first, index);
            break;
         case 2:
            if (0 == strncmp(first, "hd", index)) {
               LOG( " %hd ", va_arg( arg_list, int ) );  /*  can't use va_arg(varlist, short) because the types are promoted to int; 
               using of va_arg(varlist, int) instead */
            } 
            else if (0 == strncmp(first, "hu", index)) {
               LOG( " %hu ", va_arg( arg_list, unsigned ) );  /*  can't use va_arg(varlist, unsigned short) because the types are promoted to unsigned; 
               using of va_arg(varlist, unsigned) instead */
            }
            else if (0 == strncmp(first, "lu", index)) {
               LOG( " %lu ", va_arg( arg_list, unsigned long ) );
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
            if (0 == strncmp(first, "llu", index)) {
               LOG( " %llu ", va_arg( arg_list, unsigned long long ) );
            }
            else if (0 == strncmp(first, "lld", index)) {
               LOG( " %lld ", va_arg( arg_list, long long ) );
            }
            else 
               print_error( first, index);
            break;
         default:
            print_error( first, index);
      } 
      if (NULL == second || *(second + 1) == '\0')
         break;
      else
         first = second + 1;
   } while (first != NULL);
   va_end( arg_list );
   return 0;
}

static void case_1 (const char * first, const char * types, const size_t index, va_list args) {
   size_t index;
   if (NULL == types) {
      LOG_EXIT(__FUNCTION__, "\n error types = NULL \n", EXIT_FAILURE);  /* brackets - multiline macro */
   }
   if (NULL == first) {
      LOG_EXIT(__FUNCTION__, "\n error first = NULL \n", EXIT_FAILURE);  /* brackets - multiline macro */
   }
   if (0 == strncmp(first, "c", index)) {
      LOG( " %c ", va_arg( arg_list, int ) );  /*  can't use va_arg(varlist, char) because the types are promoted to int; 
      using of va_arg(varlist, int) instead */
   } 
   else if (0 == strncmp(first, "d", index)) {
      LOG( " %d ", va_arg( arg_list, int ) );
   }
   else if (0 == strncmp(first, "u", index)) {
      LOG( " %u ", va_arg( arg_list, unsigned ) );
   } 
   else if (0 == strncmp(first, "f", index)) {
      LOG( " %f ", va_arg( arg_list, double) );  /*  can't use va_arg(varlist, float) because the types are promoted to double; 
      using of va_arg(varlist, double) instead */
   }
   else if (0 == strncmp(first, "g", index)) {
      LOG( " %g ", va_arg( arg_list, double) );
   }
   else if (0 == strncmp(first, "s", index)) {
      LOG( " %s ", va_arg( arg_list, char*) );
   }
   else if (0 == strncmp(first, "p", index)) {
      LOG( " %p ", va_arg( arg_list, void*) );  /* many (all ?) pointers in va_arg can be cast to void* */
   }
   else 
      print_error( first, index);
   break;
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
