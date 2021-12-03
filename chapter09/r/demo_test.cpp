#include "demo_test.hpp"
#include "print.hpp"
#include "utility.hpp"
#include "demo.hpp"

#include "connector.h"

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "demo_functions.h"
   #include "shared_lib_open.h"
   #define LIB_HUMAN_SO     "libhuman.so"
#else
   #include <functional>
   
   using namespace std::placeholders;
#endif
   
namespace tests {

using namespace demo;

#ifdef MANUAL_DLL_LOAD
static void load_demo(Demo_functions & demo_functions) {
   demo_functions.handle   = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   demo_functions.init     = reinterpret_cast<Result_codes (*)(const char * )> (get_symbol(demo_functions.handle, "demo_init"));
   demo_functions.set_name = reinterpret_cast<Result_codes (*)(const char * )> (get_symbol(demo_functions.handle, "demo_set_name"));
   demo_functions.get_name = reinterpret_cast<Result_codes (*)(char ** )> (get_symbol(demo_functions.handle, "demo_get_name"));
   demo_functions.destroy  = reinterpret_cast<Result_codes (*)()> (get_symbol(demo_functions.handle, "demo_destroy"));
}

static Result_codes test_demo_linking() {
   cerr << "\nMANUAL DLL LOAD\n";
   Demo_functions demo_functions;
   load_demo(demo_functions);
   Result_codes result = demo_functions.init("Marcus Aurelius"); 
   if (OK == result) {
      char * name = nullptr;
      result = demo_functions.get_name(&name);
      if (OK == result) {
         cerr << TIE( "C++", unmove(__cplusplus), __func__, " human name = ", name) << '\n';
         assert_many(string(name) == "Marcus Aurelius", "name == ", name);
         name = nullptr;
         result = demo_functions.set_name("Socrates");
         if (OK == result) {
            result = demo_functions.get_name(&name);
            if (OK == result) {
               cerr << TIE( "C++", unmove(__cplusplus), __func__, " human name = ", name) << '\n';
               assert_many(string(name) == "Socrates", "name == ", name);
               result = demo_functions.destroy();
               if (OK == result) {
                  result = static_cast<Result_codes> (close_handle(&(demo_functions.handle)));
                  assert_many(result == OK, "result == ", result);
                  return result;
               }
            }
         }
      }
   }
   close_handle(&(demo_functions.handle));
   demo_functions.destroy();
   assert_many(result == OK, "result == ", result);
   return result;
}
#else
static Result_codes test_demo_linking() {
   cerr << "\nAUTOMATIC DLL LOAD\n";
   Demo human = {"Leibniz"};
   string name = human.get_name();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, name) << '\n';
   assert_many(name == "Leibniz", "name == ", name);
   auto set = std::bind(&Demo::set_name, std::ref(human), _1);
   Result_codes result = call_catch_exception(set, "Descartes");
   name = human.get_name();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, name) << '\n';
   assert_many(name == "Descartes", "name == ", name);
   return result;
}
#endif

Result_codes test_demo() {
   Result_codes result = test_demo_linking();
   return result;
}

}
