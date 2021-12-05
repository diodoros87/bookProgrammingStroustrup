#include "demo_test.hpp"
#include "print.hpp"
#include "utility.hpp"
#include "demo.hpp"

#include "connector.h"

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "demo_functions.h"
   #include "shared_lib_open.h"
#else
   #include <functional>
   
   using namespace std::placeholders;
#endif
   
namespace tests {

using namespace demo;
/*
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
*/

void init_wrapper(const string & name) {
  Constructor<Demo>(name);
}

typedef struct Functions {
   Demo (*init)(const string & );
   void (*set_name)(const string & );
   string (*get_name) ();
   void (*destroy)();
   void * handle;
} Functions; 

static void load_demo(Functions & functions) {
#ifdef MANUAL_DLL_LOAD
   cerr << "\nMANUAL DLL LOAD\n";
   functions.handle   = get_handle("libdemo.so", RTLD_LAZY);
   functions.init     = reinterpret_cast<Demo (*)(const string & )> (get_symbol(functions.handle, "Human_init"));
   functions.set_name = reinterpret_cast<Result_codes (*)(Human_t * const, const char * const)> (get_symbol(functions.handle, "Human_set"));
   functions.get_name = reinterpret_cast<Result_codes (*)(const Human_t * const, char ** const)> (get_symbol(functions.handle, "Human_get_name"));
   functions.destroy  = reinterpret_cast<void (*)(Human_t ** const)> (get_symbol(functions.handle, "Human_destroy"));
#else
   cerr << "\nAUTOMATIC DLL LOAD\n";
   functions.handle = nullptr;
   functions.init = reinterpret_cast<Demo (*)(const string & )> (Constructor<Demo>);
   functions.set_name = reinterpret_cast<Result_codes (*)(Human_t * const, const char * const)> (Human_set);
   functions.get_name = reinterpret_cast<Result_codes (*)(const Human_t * const, char ** const)> (Human_get_name);
   functions.destroy = reinterpret_cast<void (*)(Human_t ** const)> (Human_destroy);
#endif
}

#ifdef MANUAL_DLL_LOAD
static void load_demo(Demo_functions & demo_functions) {
   demo_functions.handle   = get_handle("libdemo.so", RTLD_LAZY);
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
