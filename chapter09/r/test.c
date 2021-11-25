#undef MANUAL_DLL_LOAD
#include "print.h"
#include "system.h"
#include "c_string.h"
#include "result_codes.h"

#include <stdlib.h>
#include <string.h>

int test(const int tests, const char * const command) {
   if (! command) { 
      LOG_EXIT(__FUNCTION__, "command is null", EXIT_FAILURE);
   }
   if (tests < 0 || tests > 99) {
      const char * test_string = to_string((int)tests);
      const char * message = concatenate_many_free_args("improper tests = ", 0, test_string, 1, NULL);
      LOG_EXIT_FREE(__FUNCTION__, message, EXIT_FAILURE);
   }
   int result = OK;
   for (int i = 0; i < tests && OK == result; i++)
      result = call_system(command);
   return result;
}

int main(const int argc, const char ** argv) {
   const char * program_name = strrchr(argv[0], '/');
   LOG("%s\n", program_name ? ++program_name : argv[0]);
   const char * command = "LD_LIBRARY_PATH=. valgrind --leak-check=full --show-leak-kinds=all ./c_main test";
   int result = test(2, command);
   if (result == OK) {
      command = "./c_main";
      result = test(2, command);
   }
   LOG(" Program name = %s", program_name);
   FUNCTION_INFO(__FUNCTION__);
   LOG(" Final result = %d\n", result);
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}
