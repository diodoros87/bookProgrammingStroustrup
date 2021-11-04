#include <cstdlib>

#include "variadic_template.hpp"
#include "connector.h"

#ifdef __cplusplus
extern "C" {
#endif

Money_int Money_int__init_1(const char * dollars) {
   return Money<int>(dollars);
}

Money_int Money_int__create_1(const char * dollars) {
   return Money<int>::create(dollars);
}

Money_int Money_int__init_2(const char * dollars, const long double cents) {
   return Money<int>(dollars, cents);
}

Money_int Money_int__create_2(const char * dollars, const long double cents) {
   return Money<int>::create(dollars, cents);
}

static Demo * demo_instance = NULL;

void demo_init(const char * name) {
   if (demo_instance == NULL)
      demo_instance = new Demo(name);
}

void demo_set_name(const char * name) {
   if (demo_instance == NULL)
      exit(EXIT_FAILURE);
   demo_instance->sayHi(name);
}

#ifdef __cplusplus
}
#endif
