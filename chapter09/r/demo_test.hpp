#ifndef DEMO_TEST_HPP
#define DEMO_TEST_HPP

#include "result_codes.h"

#include "demo.hpp"

using demo::Demo;

typedef struct Manual_DLL_interface {
   Demo * (*create)(const char * const);
   void (*destroy)(Demo * & );
   void * handle;
} Manual_DLL_interface;

namespace tests {

Result_codes test_demo();

}

#endif
