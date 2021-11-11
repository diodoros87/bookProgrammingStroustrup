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

static void case_1 (const char * format, const size_t index, va_list arg_list) ;
static void case_2 (const char * format, const size_t index, va_list arg_list) ;
static void case_3 (const char * format, const size_t index, va_list arg_list) ;

int print_many(const char * msg,  const char * types, ... ) {   /* types is type-format c-string */
   va_list arg_list;
   char *first;   /* the remainder of the c-string to be considered  */
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
         case 0:   /* DELIMITERS in format string are skipped */
            break;
         case 1:
            case_1(first, index, arg_list); 
            break;
         case 2:
            case_2(first, index, arg_list);
            break;
         case 3:
            case_3(first, index, arg_list);
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

#undef CHECK_INPUT_ARG

#define CHECK_INPUT_ARG(format) \
   assert_one(NULL != (format)); \
   if (NULL == format) { \
      LOG_EXIT(__FUNCTION__, "\n error format = NULL \n", EXIT_FAILURE);  \
   }

static void case_1 (const char * format, const size_t index, va_list arg_list) {
   CHECK_INPUT_ARG(format);
   if (0 == strncmp(format, "c", index)) {
      LOG( " %c ", va_arg( arg_list, int ) );  /*  can't use va_arg(varlist, char) because the types are promoted to int; 
      using of va_arg(varlist, int) instead */
   } 
   else if (0 == strncmp(format, "d", index)) {
      LOG( " %d ", va_arg( arg_list, int ) );
   }
   else if (0 == strncmp(format, "u", index)) {
      LOG( " %u ", va_arg( arg_list, unsigned ) );
   } 
   else if (0 == strncmp(format, "f", index)) {
      LOG( " %f ", va_arg( arg_list, double) );  /*  can't use va_arg(varlist, float) because the types are promoted to double; 
      using of va_arg(varlist, double) instead */
   }
   else if (0 == strncmp(format, "g", index)) {
      LOG( " %g ", va_arg( arg_list, double) );
   }
   else if (0 == strncmp(format, "s", index)) {
      LOG( " %s ", va_arg( arg_list, char*) );
   }
   else if (0 == strncmp(format, "p", index)) {
      LOG( " %p ", va_arg( arg_list, void*) );  /* many (all ?) pointers in va_arg can be cast to void* */
   }
   else 
      print_error( format, index);
} 

static void case_2 (const char * format, const size_t index, va_list arg_list) {
   CHECK_INPUT_ARG(format);
   if (0 == strncmp(format, "hd", index)) {
      LOG( " %hd ", va_arg( arg_list, int ) );  /*  can't use va_arg(varlist, short) because the types are promoted to int; 
      using of va_arg(varlist, int) instead */
   } 
   else if (0 == strncmp(format, "hu", index)) {
      LOG( " %hu ", va_arg( arg_list, unsigned ) );  /*  can't use va_arg(varlist, unsigned short) because the types are promoted to unsigned; 
      using of va_arg(varlist, unsigned) instead */
   }
   else if (0 == strncmp(format, "lu", index)) {
      LOG( " %lu ", va_arg( arg_list, unsigned long ) );
   }
   else if (0 == strncmp(format, "ld", index)) {
      LOG( " %ld ", va_arg( arg_list, long ) );
   }
   else if (0 == strncmp(format, "Lg", index)) {
      LOG( " %Lg ", va_arg( arg_list, long double) );
   }
   else 
      print_error( format, index);
} 

static void case_3 (const char * format, const size_t index, va_list arg_list) {
   CHECK_INPUT_ARG(format);
   if (0 == strncmp(format, "llu", index)) {
      LOG( " %llu ", va_arg( arg_list, unsigned long long ) );
   }
   else if (0 == strncmp(format, "lld", index)) {
      LOG( " %lld ", va_arg( arg_list, long long ) );
   }
   else 
      print_error( format, index);
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
