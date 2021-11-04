#include <dlfcn.h>

#include <stdlib.h>

#include "print.h"

void log_exit (const char * info) {
   LOG("%s\n", info);
   exit(EXIT_FAILURE);
}

void * get_handle (char * filepath, int flag) {
   void * result = dlopen(filepath, flag);
   if (NULL == result)
      log_exit(dlerror());
   return result;
}

void * get_symbol (void * handle, char * symbol) {
   void * result = dlsym(handle, symbol);
   if (NULL == result)
      log_exit(dlerror());
   return result;
}
