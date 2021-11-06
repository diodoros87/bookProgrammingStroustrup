#include <stdio.h>

#define LANGUAGE "C90"

extern int print_many(const char * msg,   /* message to be printed     */
  const char * types, /* parameter types (i,s)     */...);

extern int print_assert(const char *file, int line, const char * date, const char * time, const char *msg) ;

#define assert_many(EX,...) \
  ((EX) || (print_many(__VA_ARGS__)) || \
   print_assert(__FILE__, __LINE__, __DATE__, __TIME__, #EX))

#define LOG(format, ...) \
   fprintf(stderr, format, __VA_ARGS__); \
      if (ferror (stderr)) \
         perror("\nError while printing to stderr\n")
   
#define LOG_FUNC(function_name) fprintf (stderr, "\nF: " #function_name  " = %s \t", function_name)
   
#define FUNCTION_INFO(function_name) \
   LOG("\n%s \t", LANGUAGE); \
   LOG_FUNC(function_name)

#define LOG_EXIT(function_name, info, status) \
   FUNCTION_INFO(function_name); \
   LOG("\ninfo = %s\n exit status = %d\n", info, status); \
   exit(status)
