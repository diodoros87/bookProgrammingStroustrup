#define DEBUG_OSTREAM

#include "integer.hpp"
#include "assertions.hpp"
#include "money.hpp"
#include "money_init_test.hpp"
#include "money_operations_test.hpp"

#include <functional>

using namespace std;
using namespace integer_space;
using namespace money;

template <typename Function, typename... Args>  
bool execute_function(Function && func, Args&&... args ) {
   try {
      func(forward<Args>(args)...);
      
      return true;
   } catch (const money::bad_from_string& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const integer_space::Arithmetic_Error & e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const std::invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const std::out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const std::bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const std::bad_alloc & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
   }catch (const std::runtime_error& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const std::exception& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (...) {
      cerr << __func__ << " unrecognized exception\n";
   }
   
   return false;
}

template <typename Function, typename... Args>  
static inline  bool call_function(Function && func, Args&&... args) {
   bool result = execute_function(func, forward<Args>(args)...);
   assert_many(result == true, "result == ", result);
   return result;
}

bool perform_tests() {
   initializer_list < function <void()> > vec = { //bind(money_init_test::perform), 
      bind(money_operations_test::perform) 
      
      
   };
   bool result = false;
   for (auto fun : vec) {
      result = call_function(fun);
      if (false == result)
         break;
   }
   return result;   
}

int main() {
   const bool result = perform_tests();
   cerr << "\n" << __func__ << " END of ALL TESTS\n";
   if (result)
      return EXIT_SUCCESS;
   else
      return EXIT_FAILURE;
}
