/*
#define MANUAL_DLL_LOAD
*/
#include "print.h"
#include "result_codes.h"
#include "file_modify.h"
#include "system.h"

#include <string.h>
#include <stdlib.h>

Result_codes change_makefile(void) {
   struct File_modify_t * modifier = NULL;/*
   struct File_modify_t * modifier;*/
   Result_codes result = File_modify_init(&modifier, "Makefile");
   if (result == OK)
      result = File_modify_set(modifier, "Makefile");  /* only for test  */
   if (result == OK) {
      char * filename = NULL;
      result = File_modify_get_filename(modifier, &filename);
      LOG("File name = %s\n", filename);
      free(filename);
   }
   if (result == OK)
      result = edit_makefile(modifier);
   File_modify_destroy(&modifier);
   free(modifier);
   return OK;
}

Result_codes make_clean_make(void) {
   char *exec_args[] = { "make", "clean", NULL };
   int result = execute(exec_args);
   if (result != SYSTEM_ERROR) 
      result = change_makefile();
   if (result == OK) {
      exec_args[1] = NULL;
      if (execute(exec_args) != SYSTEM_ERROR)
         result = OK;
   }
   LOG("Parent process: pid = %d\nGoodbye!\n", getpid());
   return result;
}

int test_linking(const bool_t valgrind) {
   FUNCTION_INFO(__FUNCTION__);
   static const char const * command = "LD_LIBRARY_PATH=. ./c_linking_test";
   int result = call_system(command);
   if (OK == result && valgrind) 
      result = call_system("LD_LIBRARY_PATH=. valgrind --leak-check=full --show-leak-kinds=all ./c_linking_test");
   return result;
}

int main(const int argc, const char * argv[]) {
   const char * program_name = strrchr(argv[0], '/');;
   LOG("%s\n", program_name ? ++program_name : argv[0]);
   FUNCTION_INFO(__FUNCTION__);
   bool_t valgrind = (argc == 2 && strcmp(argv[1], "test") == 0) ? 1 : 0;
   int result = test_linking (valgrind);
   assert_many(result != SYSTEM_ERROR, "assert failed: ", "s d", "result == ", result);
   if (result == OK)
      result = make_clean_make();
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   if (result == OK)
      result = test_linking(valgrind);
   assert_many(result != SYSTEM_ERROR, "assert failed: ", "s d", "result == ", result);
   LOG(" Program name = %s", program_name);
   FUNCTION_INFO(__FUNCTION__);
   LOG(" Final result = %d\n", result);
   return result;
}
