#include "demo_test.hpp"
#include "print.hpp"
#include "utility.hpp"

#include "connector.h"

#include <functional>

using namespace std::placeholders;
using std::function;

using std::bind;

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
*/
#ifdef MANUAL_DLL_LOAD


static void load_demo(Manual_DLL_interface & demo_functions) {
   demo_functions.handle   = get_handle("libdemo.so", RTLD_LAZY);
   demo_functions.create   = reinterpret_cast<Demo * (*)(const char * const )> (get_symbol(demo_functions.handle, "demo_create"));
   //demo_functions.set_name = reinterpret_cast<Result_codes (*)(const char * )> (get_symbol(demo_functions.handle, "demo_set_name"));
   //demo_functions.get_name = reinterpret_cast<Result_codes (*)(char ** )> (get_symbol(demo_functions.handle, "demo_get_name"));
   demo_functions.destroy  = reinterpret_cast<void (*)(Demo * & )> (get_symbol(demo_functions.handle, "demo_destroy"));
}

static Result_codes test_demo_linking() {
   cerr << "\nMANUAL DLL LOAD\n";
   Manual_DLL_interface demo_functions;
   load_demo(demo_functions);
   Demo * demo = demo_functions.create("Marcus Aurelius"); 
   if (nullptr == demo) {
      close_handle(&(demo_functions.handle));
      return INVALID_ARG;
   }
   string name = demo->get_name();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, " human name = ", name) << '\n';
   assert_many(string(name) == "Marcus Aurelius", "name == ", name);
   function<void(const string&)> set = bind(&Demo::set_name, demo, _1);
   Result_codes result = call_catch_exception(set, "Socrates");
   if (OK != result)
      return result;
   name = demo->get_name();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, " human name = ", name) << '\n';
   assert_many(string(name) == "Socrates", "name == ", name);
   demo_functions.destroy(demo);
   result = static_cast<Result_codes> (close_handle(&(demo_functions.handle)));
   assert_many(result == OK, "result == ", result);
   assert_many(demo == nullptr, "demo == ", demo);
   return result;
}
#else
static Result_codes test_demo_linking() {
   cerr << "\nAUTOMATIC DLL LOAD\n";
   Demo human = {"Leibniz"};
   string name = human.get_name();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, name) << '\n';
   assert_many(name == "Leibniz", "name == ", name);
   //auto set = bind(&Demo::set_name, std::ref(human), _1);
   function<void(const string&)> set = bind(&Demo::set_name, std::ref(human), _1);
   Result_codes result = call_catch_exception(set, "Descartes4");
   name = human.get_name();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, name) << '\n';
   assert_many(name == "Descartes", "name == ", name);
   return result;
}
#endif

Result_codes test_demo() {
   //auto test = bind(&test_demo_linking);
   function<Result_codes()> test = bind(&test_demo_linking);
   Result_codes result = call_catch_exception(test);
   return result; /*
   try {
      Result_codes result = test_demo_linking();
      
      return result;
   } catch (const invalid_argument& const_e) {
      cerr  << __func__ << " " << typeid(const_e).name() << " " << const_e.what() << '\n';
      invalid_argument &e = const_cast<invalid_argument &>(const_e);
      return get_error_code(reinterpret_cast<invalid_argument *>(&e));
   } catch (const out_of_range& const_e) {
      cerr  << __func__ << " " << typeid(const_e).name() << " " << const_e.what() << '\n';
      out_of_range &e = const_cast<out_of_range &>(const_e);
      return get_error_code(reinterpret_cast<out_of_range *>(&e));
   } catch (const bad_alloc & const_e) {
      cerr  << __func__ << " " << typeid(const_e).name() << " " << const_e.what() << '\n';
      bad_alloc &e = const_cast<bad_alloc &>(const_e);
      return get_error_code(reinterpret_cast<bad_alloc *>(&e));
   } catch (const exception & const_e) {
      cerr  << __func__ << " " << typeid(const_e).name() << " " << const_e.what() << '\n';
      exception &e = const_cast<exception &>(const_e);
      return get_error_code(reinterpret_cast<exception *>(&e));
   } catch (...) {
      cerr  << __func__ << " Unrecognized exception was catched " << '\n';
      return UNRECOGNIZED_ERROR;
   } */
}

/*
static Demo * demo_ptr = nullptr;

static void init_wrapper(const string & name) {
   if (::demo_ptr == nullptr)
      ::demo_ptr = new Demo(name);
}

static void set_name_wrapper(const string & name) {
   if (::demo_ptr) {
      demo_ptr->set_name(name);
}

static void get_name_wrapper() {
  if (::demo_ptr) {
      demo_ptr->get_name(name);
}

static void destroy_wrapper() {
   if (::demo_instance) {
      delete ::demo_ptr;
      ::demo_ptr = nullptr;
   }
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
*/

}
