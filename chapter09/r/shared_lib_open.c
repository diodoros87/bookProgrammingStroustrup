#include <dlfcn.h>

#include <stdlib.h>

#include "print.h"

void * get_handle (char * filepath, int flag) {
   void * result = dlopen(filepath, flag);
   if (NULL == result) {
      LOG_EXIT(__FUNCTION__, dlerror(), EXIT_FAILURE);
   }
   return result;
}

void * get_symbol (void * handle, char * symbol) {
   void * result = dlsym(handle, symbol);
   if (NULL == result) {
      LOG_EXIT(__FUNCTION__, dlerror(), EXIT_FAILURE);
   }
   return result;
}
