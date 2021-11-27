#include "regular_expr.h"
#include "print.h"
#include "result_codes.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>

regex_t compile_regex(const char * pattern, const int cflags) {
   if (! pattern) {
      LOG_EXIT(__FUNCTION__, "regex pattern is null\n", EXIT_FAILURE);
   }
   regex_t regex;
   int result = regcomp(&regex, pattern, cflags);
   if (0 != result) {
      // Any value different from 0 means it was not possible to 
      // compile the regular expression, either for memory problems
      // or problems with the regular expression syntax.
      if (result == REG_ESPACE)
         LOG ("%s\n", strerror(ENOMEM));
      else
         LOG ("Syntax error in the regular pattern %s\n", pattern);             
   }
   if (0 != result) {
      LOG_EXIT(__FUNCTION__, "Could not compile regex\n", EXIT_FAILURE);
   }
   return regex;
}

Result_codes print_regerror (int errcode, size_t length, regex_t * compiled) {
   if (! compiled) {
      LOG("%s regex %p  is null", __FUNCTION__, compiled);
      return INVALID_ARG;
   } 
  char* buffer = calloc(length, sizeof(char));
  (void) regerror (errcode, compiled, buffer, length);
  LOG("Regex match failed: %s\n", buffer);
  free(buffer);
  return OK;
}

Result_codes match_regex(regex_t * regex, const char * const str) {
   if (! str || ! regex) {
      LOG("%s  str: %p or regex %p  is null", __FUNCTION__, str, regex);
      return INVALID_ARG;
   } 
   int result = regexec (regex, str, 0, NULL, 0);
   switch (result) {
      case 0:
         return OK;
      case REG_NOMATCH:
         LOG (" argument of str: '%s' is not matches by regex\n", str);
         return INVALID_ARG;
      default:  {
         // The function returned an error; print the string 
         // describing it.
         // Get the size of the buffer required for the error message.
         size_t length = regerror (result, regex, NULL, 0);
         print_regerror (result, length, regex);       
         return INVALID_ARG;
      }
   }
}

Result_codes regex_matches(const char *str, const char *pattern, const int cflags) {
   if (! pattern || ! str) {
      LOG("%s  pattern: %p or str %p  is null", __FUNCTION__, pattern, str);
      return INVALID_ARG;
   } 
   regex_t re = compile_regex(pattern, cflags);
   Result_codes result = match_regex(&re, str);
   regfree(&re);
   return result;
}
