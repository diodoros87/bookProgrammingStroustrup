#include "utility.hpp"
#include "print.hpp"
#include "variadic_template_test.hpp"
#include "demo_test.hpp"
#include "human_test.hpp"

using namespace tests;

void set_handler(void (*func)(void)) { 
   std::set_terminate(func); 
}

void handle_terminate(void) { 
   cerr << TIE( "C++", unmove(__cplusplus), __func__, " handler called - must've been some exception ?!\n");
}

static Result_codes main_linking_tests() {
   Result_codes result = test_demo();
   assert_many(result == OK, "result == ", result);
   if (OK == result)
      result = test_human();
   assert_many(result == OK, "result == ", result);
   if (OK == result)
      result = test_money();
   assert_many(result == OK, "result == ", result);
   return result;
}

int main(const int argc, const char * argv[]) {
   try {
      cerr << "\n C++ " << __cplusplus << " function = " << __func__ << '\n';
      size_t pos = string(argv[0]).rfind('/');
      const string program_name = (pos != string::npos && pos + 1 < string(argv[0]).size()) ? string(argv[0]).substr(pos + 1) : argv[0];
      cerr << "\n  Program name = " << program_name << '\n';
      set_handler(handle_terminate);
      Result_codes result = main_linking_tests();
      cerr << "\n  Program name = " << program_name;
      cerr << "\n C++ " << __cplusplus << " function = " << __func__ << '\n';
      cerr << " Final result = " << result << '\n';
      assert_many(result == OK, "result == ", result);
      return result;
   } 
   catch (const std::invalid_argument & e) {
      cerr << "Exception: " << e.what() << '\n';
   } catch (const std::out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << '\n';
   }
   catch (const std::bad_alloc & e) {
      cerr << "Exception: " << e.what() << '\n';
   }
   catch (const std::exception & e) {
      cerr << "Exception: " << e.what() << '\n';
   }
   catch (...) {
      cerr << "Unrecognized Exception: " << '\n';
   }
   return EXIT_FAILURE;
}
