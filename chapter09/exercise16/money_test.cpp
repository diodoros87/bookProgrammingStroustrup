#include "integer.hpp"
#include "assertions.hpp"
#include "money.hpp"

using namespace std;
using namespace integer_space;
using namespace money;

template <typename T> 
void test() { 
   Money<T> money("20t", 34);
   cout << "money = " << money << '\n';
}

template <typename T> 
void incorrect_test(const string & DOLLARS, const double CENTS ) { 
   try {
      Money<T> money(DOLLARS, 34);
      assert(0);
      cerr << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   try {
      Money<T> money(DOLLARS, CENTS);
      assert(0);
      cout << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

int main() {
   try {
      test<int>();
      incorrect_test<int>();
      test<float>();
      incorrect_test<float>("20.8");
      incorrect_test<float>("20t");
      //test<string>();
      //incorrect_test<string>();
      return 0;
   } catch (const Arithmetic_Error & e) {
      cerr << __func__ << " exception : " << e.what() << endl;
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const runtime_error& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const exception& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (...) {
      cerr << __func__ << " unrecognized exception\n";
   }
   return 1;
}
