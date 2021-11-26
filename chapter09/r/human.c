#include "human.h"
#include "print.h"
#include "result_codes.h"
   
const regex Demo::NAME_REGEX       = Demo::set_regex() ;

#define CHECK_FILE(filename) \
REQUIRE_NON_NULL(filename) \
if (! exist_file(filename)) { \
      const char * message = concatenate("Error: ", strerror(errno)); \
      LOG_EXIT_FREE(__FUNCTION__, message, EXIT_FAILURE); \
   }

void print_regerror (int errcode, size_t length, regex_t * compiled) {
  char* buffer = calloc(length, sizeof(char));
  (void) regerror (errcode, compiled, buffer, length);
  LOG("Regex match failed: %s\n", buffer);
}

Result_codes validate(const char * const name) {
   if (! name) {
      LOG("%s  argument of name: \'%s\' is null", function, name);
      return INVALID_ARG;
   } 
   int result = regexec (&regex, argv[i], 0, NULL, 0);
   switch (result) {
      case 0:
         return OK;
      case REG_NOMATCH:
         LOG (" argument of name: '%s' is not matches by regex\n", name);
         return INVALID_ARG;
      default:
      {
         // The function returned an error; print the string 
         // describing it.
         // Get the size of the buffer required for the error message.
         size_t length = regerror (result, &regex, NULL, 0);
         print_regerror (result, length, &regex);       
         return INVALID_ARG;
      }
   }
}

Result_codes Demo::Demo(const char * name) {
   FUNCTION_INFO(__FUNCTION__);
   Result_codes result = validate(name);
   if (OK == result);
      this->name = name;
   return result;
}

Result_codes Demo::set_name(const char *name) {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, name) << '\n';
   validate(name, __func__, " Error name");
   this->name = name;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->name) << '\n';
}

Result_codes char * Demo::get_name() const {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__) << '\n';
   char * result = const_cast<char *>(name.c_str());
   return result;
}

Human_destroy() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->name) << '\n';
}

}
const regex_t * const get_regex(const char * expression, const int cflags) {
   if (! expression) {
      LOG_EXIT(__FUNCTION__, "regex expression is null\n", EXIT_FAILURE);
   }
   regex_t regex = NULL;
   int result = regcomp(&regex, expression, cflags);
   if (0 != result) {
      // Any value different from 0 means it was not possible to 
      // compile the regular expression, either for memory problems
      // or problems with the regular expression syntax.
      if (result == REG_ESPACE)
         LOG ("%s\n", strerror(ENOMEM));
      else
         LOG ("Syntax error in the regular expression passed as first argument\n", stderr);             
   }
   if (0 != result) {
      LOG_EXIT(__FUNCTION__, "Could not compile regex\n", EXIT_FAILURE);
   }
   return regex;
}

const regex_t * const set_regex(const char * expression) {
   /* Compile regular expression */
   static regex NAME_REGEX = get_regex("(^[[:upper:]][[:lower:]]*( [[:upper:]][[:lower:]]*)?$)", REG_EXTENDED);
   return NAME_REGEX;
}

const regex_t * const NAME_REGEX = set_regex();

#undef validate
