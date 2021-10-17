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
   bool overflow = false;
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
      assert(static_cast<string>(i) == STR);
      return i;
   }
   
//    constexpr bool may_overflow() {
//       constexpr MIN = 
//    }
   
   void check(const long long & NUMBER, const Integer & OBJECT) {
      //cerr << showpos << "\n number  = " << NUMBER << "\n object = " << OBJECT;
      ostringstream stream;
      if (0 != NUMBER)
         stream.setf(ios_base::showpos);
      stream << NUMBER;
      const string number_string = stream.str();
      const string object_string = string(OBJECT);
      //cerr << "\n number  = " << number_string << "\n object = " << object_string << '\n';
      //assert(number_string == object_string);
      GENERAL_ASSERT(number_string == object_string, "\n number = " + number_string + "\n object = " + object_string);
      //return number_string == object_string;
   } 
   
   long long add_subtract_2(const long long & NUMBER, const Integer & OBJECT, const char SIGNUM) {
      number_result = 2 * NUMBER;
      cerr << showpos << "\n number:  2 * " << NUMBER << " = " << number_result;
      static OBJECT_TWO = Integer::parse_create("2");
      object_result   = OBJECT_TWO * OBJECT;
      if (SIGNUM == Integer::MINUS) {
         object_result = -object_result;
         OBJECT_TWO = -OBJECT_TWO;
      }
      cerr << "\n object: " << OBJECT_TWO << " * " << OBJECT << " = " << object_result << '\n';
      check(number_result, object_result);
      return number_result;
   }
   
   void add_subtract(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      long long number_result = N_1 + N_2;
      cerr << showpos << "\n number: " << N_1 << " + " << N_2 << " = " << number_result;
      Integer object_result   = O_1 + O_2;
      cerr << "\n object: " << O_1 << " + " << O_2 << " = " << object_result << '\n';
      check(number_result, object_result);
      
      number_result = - N_1 - N_2;
      cerr << showpos << "\n number: " << - N_1 << " - " << N_2 << " = " << number_result;
      object_result   = - O_1 - O_2;
      cerr << "\n object: " << - O_1 << " - " << O_2 << " = " << object_result << '\n';
      check(number_result, object_result);
      
      number_result = 2 * N_1;
      cerr << showpos << "\n number:  2 * " << NUMBER << " = " << number_result;
      static const OBJECT_TWO = Integer::parse_create("2");
      object_result   = OBJECT_TWO * OBJECT;
      cerr << "\n object: " << OBJECT_TWO << " * " << OBJECT << " = " << object_result << '\n';
      check(number_result, object_result);
      static const OBJECT_TWO = Integer::parse_create("+2");
      check(number_result, object_result);
   }
   
   void run_single_number (const unsigned long long REPETITIONS) {
      cout << "\n\n ----------- Test run for type: " << typeid(T).name() << '\n';
      T number;
      Integer object;
      for (int i = 0; i < REPETITIONS; i++) {
         number = generator();
         object = construct_Integer(number);
         check(+number, +object);
         check(-number, -object);
         check(static_cast<long long>(number) - number, object - object);
         check(static_cast<long long>(-number) + number, -object + object);
//          check(static_cast<long long>(number) + number, object + object);
//          check(static_cast<long long>(number) - number, object - object);
//          check(static_cast<long long>(number) * number, object * object);
//          check(static_cast<long long>(-number) + number, -object + object);
//          check(static_cast<long long>(-number) - number, -object - object);
//          check(static_cast<long long>(-number) * number, -object * object);
//          if (0 != number)
//             check(static_cast<long long>(number) / number, object / object);
      }
   }
};

template <typename T>
void run(const Generator<T> & gen) {
   Random_Operations<T> operations (gen); 
   static constexpr unsigned long long REPETITIONS = 100;
   operations.run_single_number(REPETITIONS);
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
