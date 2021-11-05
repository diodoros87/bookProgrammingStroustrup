#include <stdio.h>

#define LANGUAGE "C90"

#define LOG(format, ...) \
   fprintf(stderr, format, __VA_ARGS__)
   
#define LOG_FUNC(function_name) fprintf (stderr, "\nF: " #function_name  " = %s \t", function_name)
   
#define FUNCTION_INFO(function_name) \
   LOG("\n%s \t", LANGUAGE); \
   LOG_FUNC(function_name)

#define LOG_EXIT(function_name, info, status) \
   FUNCTION_INFO(function_name); \
   LOG("\ninfo = %s\n exit status = %d\n", info, status); \
   exit(status)
