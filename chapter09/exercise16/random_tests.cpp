#include "integer.hpp"
#include "integer_parsing.hpp"
#include "assertions.hpp"
#include "parsing_test.hpp"
#include "random_numbers.hpp"

#include<iostream>
#include<sstream>
#include<limits>
//#define NDEBUG
#include <cassert>

using namespace std;
using namespace integer_space;

namespace random_tests {
   
template <typename T>
class Random_Operations {
static_assert(is_integral<T>::value && "Integral required.");
private:
   Generator<T> generator;
public:
   explicit Random_Operations(const Generator<T> & gen) : generator(gen) { }
   
   ~ Random_Operations() = default;
   Random_Operations(const Random_Operations &) = default;
   Random_Operations(Random_Operations &&) = default;
   Random_Operations & operator=(const Random_Operations &) = default;
   Random_Operations & operator=(Random_Operations &&) = default;
   
   const T get_random_number() { return generator(); }
   Generator<T> get_generator() { return generator(); }
   void set_generator(const Generator<T> &  gen) { generator = gen; }
   
   Integer construct_Integer (const T& NUMBER) { 
      const string STR = to_string(NUMBER);
      Integer i = Integer::parse_create(STR);
      return i;
   }
   
   void check(const long long && NUMBER, const Integer && OBJECT) {
      cout << showpos << "\n number  = " << NUMBER << "\n integer = " << OBJECT << '\n';
      stringstream stream;
      if (0 != NUMBER)
         stream.setf(showpos);
      stream << NUMBER;
      const string number_string = stream.str();
      const string object_string = string(OBJECT);
      cout << "\n number  = " << number_string << "\n integer = " << object_string << '\n';
      assert(number_string == object_string);
   }
   
   void run (const unsigned long long REPETITIONS) {
      T number;
      Integer object;
      for (int i = 0; i < REPETITIONS; i++) {
         number = generator();
         object = construct_Integer(number);
         check(+number, +object);
         check(-number, -object);
         check(number + number, object + object);
         check(number - number, object - object);
         check(number * number, object * object);
         check(number / number, object / object);
      }
   }
};

template <typename T>
void run(const Generator<T> & gen) {
   Random_Operations<T> operations (gen); 
   static constexpr unsigned long long REPETITIONS = 10;
   operations.run(REPETITIONS);
}
   //inline void run
void run() {
   struct Generator_set { 
      Generator_MINMAX<long long> gen_long;
      Generator_MINMAX<int> gen_int;
      Generator_MINMAX<short>  gen_short;
      Generator_MINMAX<char> gen_char;
   };
   Generator_set generator_set;
   run<long long>(generator_set.gen_long.GENERATOR);
   run<int>(generator_set.gen_int.GENERATOR);
   run<short>(generator_set.gen_short.GENERATOR);
   run<char>(generator_set.gen_char.GENERATOR);
}
}

int main() {
   try {
      random_tests::run();
//       for (int i = 0; i < 10; i++) {
//          
//          random_tests::run<int>();
//          random_tests::run<short>();
//          random_tests::run<char>();
//       }
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
