#include "utility.hpp"
#include "print.hpp"
#include "variadic_template.hpp"
#include "demo.hpp"

#include "connector.h"
#include "human.h"

#ifdef MANUAL_DLL_LOAD
   #include <dlfcn.h>
   #include "demo_functions.h"
   #include "shared_lib_open.h"
   #define LIB_HUMAN_SO     "libhuman.so"
#else
   #include <functional>
   
   using namespace std::placeholders;
#endif

using namespace demo;

void set_handler(void (*func)(void)) { 
   std::set_terminate(func); 
}

void handle_terminate(void) { 
   cerr << TIE( unmove(__cplusplus), __func__, " handler called - must've been some exception ?!\n");
}
   
#ifdef MANUAL_DLL_LOAD
void load_demo(Demo_functions & demo_functions) {
   demo_functions.handle   = get_handle(LIB_CONNECTOR_SO, RTLD_LAZY);
   demo_functions.init     = reinterpret_cast<Result_codes (*)(const char * )> (get_symbol(demo_functions.handle, "demo_init"));
   demo_functions.set_name = reinterpret_cast<Result_codes (*)(const char * )> (get_symbol(demo_functions.handle, "demo_set_name"));
   demo_functions.get_name = reinterpret_cast<Result_codes (*)(char ** )> (get_symbol(demo_functions.handle, "demo_get_name"));
   demo_functions.destroy  = reinterpret_cast<Result_codes (*)()> (get_symbol(demo_functions.handle, "demo_destroy"));
}
#endif

Result_codes test_demo() {
#ifdef MANUAL_DLL_LOAD
   cerr << "\nMANUAL DLL LOAD\n";
   Demo_functions demo_functions;
   load_demo(demo_functions);
   Result_codes result = demo_functions.init("Nicolaus Copernicus"); 
   if (OK == result) {
      char * name = nullptr;
      result = demo_functions.get_name(&name);
      if (OK == result) {
         cerr << __func__ << " human name = " << name << '\n';
         name = nullptr;
         demo_functions.set_name("Aristotle");
         result = demo_functions.get_name(&name);
         if (OK == result) {
            cerr << __func__ << " human name = " << name << '\n';
            result = demo_functions.destroy();
            if (OK == result) {
               result = static_cast<Result_codes> (close_handle(&(demo_functions.handle)));
               assert_many(result == OK, "result == ", result);
               return result;
            }
         }
      }
   }
   close_handle(&(demo_functions.handle));
   demo_functions.destroy();
   assert_many(result == OK, "result == ", result);
#else
   cerr << "\nAUTOMATIC DLL LOAD\n";
   Demo human = {"Leibniz"};
   cerr << TIE( unmove(__cplusplus), __func__, unmove(human.get_name())) << '\n';
   auto set = std::bind(&Demo::set_name, std::ref(human), _1);
   Result_codes result = call_catch_exception(set, "Spinoza");
   cerr << TIE( unmove(__cplusplus), __func__, unmove(human.get_name())) << '\n';
#endif
   return result;
}

Result_codes test_money() {
   Result_codes result = call_catch_exception(Constructor<int, Money>(), "-20", -80.0L);
   if (OK == result)
      call_catch_exception(Constructor<long long, Money>(), "78");
   if (OK == result)
      call_catch_exception(Creation<unsigned long long>(), "2", 5);
   if (OK == result)
      call_catch_exception(Creation<long double>(), "7");
   return result;
}

int main(const int argc, const char * argv[]) {
   cerr << "\n C++ " << __cplusplus << " function = " << __func__ << '\n';
   size_t pos = string(argv[0]).rfind('/');
   const string program_name = (pos != string::npos && pos + 1 < string(argv[0]).size()) ? string(argv[0]).substr(pos + 1) : argv[0];
   cerr << "\n  Program name = " << program_name << '\n';
   set_handler(handle_terminate);
   Result_codes result = test_money();
   assert_many(result == OK, "result == ", result);
   if (OK == result)
      result = test_demo();
   cerr << "\n  Program name = " << program_name;
   cerr << "\n C++ " << __cplusplus << " function = " << __func__ << '\n';
   cerr << " Final result = " << result << '\n';
   assert_many(result == OK, "result == ", result);
   return result;
}
