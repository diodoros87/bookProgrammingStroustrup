#include "integer.hpp"
#include "integer_parsing.hpp"
#include "assertions.hpp"
#include "parsing_test.hpp"
#include "random_numbers.hpp"

#include<iostream>
#include<limits>
//#define NDEBUG
#include <cassert>

using namespace std;
using namespace integer_space;

namespace random_tests {
   template <typename T>
   void run() {
      static_assert(is_integral<T>::value && "Integral required.");
      constexpr T MIN = numeric_limits<T>::min();
      constexpr T MAX = numeric_limits<T>::max();
      Int_Generator<T> generator = { MIN, MAX };
      cerr << " random = " << static_cast<T>(generator()) << '\n';
   }
}

int main() {
   try {
      random_tests::run<long long>();
      random_tests::run<int>();
      random_tests::run<short>();
      random_tests::run<unsigned>();
      return 0;
   }
   catch (const Arithmetic_Error & e) {
      cerr << __func__ << " exception : " << e.what() << endl;
   }
   catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   catch (const runtime_error& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   catch (const exception& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   catch (...) {
      cerr << __func__ << " unrecognized exception\n";
   }
   return 1;
}
