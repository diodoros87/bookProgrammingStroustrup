#include "integer.hpp"
#include "integer_parsing.hpp"
#include "assertions.hpp"
#include "parsing_test.hpp"

#include<iostream>
//#define NDEBUG
#include <cassert>

using namespace std;
using namespace integer_space;

namespace main_integer_test {

template <const size_t SIZE>
static Integer set(const array<digit_type, SIZE> & table, Integer & integer) {
   cout << __func__ << " tests set_integer_array <" << SIZE <<  "> \n";
   integer.set_integer_array<SIZE>(table);
   cout << integer << '\n';
   return integer;
}

static Integer set(const vector<digit_type> & vec, Integer & integer) {
   cout << __func__ << " tests set_integer_array <vector> \n";
   integer.set_integer_array(vec);
   cout << integer << '\n';
   return integer;
}

static Integer constructor_test(const array<digit_type, Integer::MAX_ARRAY_LENGTH> & table, const char signum) {
   cout << __func__ << " tests construct Integer by array<digit_type, Integer::MAX_ARRAY_LENGTH> \n";
   Integer i = Integer(table, signum);
   cout << i << '\n';
   return i;
}

template <const unsigned int N>
static Integer creation_test(const array<digit_type, N> & table, const char signum) {
   cout << __func__ << " tests Integer::create_Integer <" << N <<  "> \n";
   Integer i =  Integer::create_Integer<N>(table, signum);
   cout << i << '\n';
   return i;
}

static Integer constructor_test(const vector<digit_type> & vec, const char signum) {
   cout << __func__ << " tests construct Integer by <vector> \n";
   Integer i (vec, signum);
   cout << i << '\n';
   return i;
}

static Integer default_constructor_test() {
   cout << __func__ << " tests construct Integer by default constructor \n";
   Integer i ;
   cout << i << '\n';
   assert( static_cast<string>(i) == "0");
   return i;
}

void set_integer_array_test(Integer & integer) {
   cout << __func__ << "\n";
   char sign = integer.get_signum();
   //if (Integer::NEUTRAL == sign)
   //   sign = Integer::PLUS;
   constexpr array<digit_type, 5> a = { 7, 2, 7, 0};
   set(a, integer);
   integer_assert(integer, sign, "72700");
   
   constexpr array<digit_type, Integer::MAX_ARRAY_LENGTH> i40 = { 7, 2, 9};
   set(i40, integer);
   integer_assert(integer, sign, "7290000000000000000000000000000000000000");
   
   constexpr array<digit_type, 0> empty = { };
   set<0>(empty, integer);
   integer_assert(integer, Integer::NEUTRAL, "0");
   
   vector<digit_type> vec = { 0, 4 , 5};
   set(vec, integer);
   integer_assert(integer, Integer::PLUS, "45");
   
   vec = { };
   set(vec, integer);
   integer_assert(integer, Integer::NEUTRAL, "0");
}

void fail_set_integer_array_test(Integer & integer) {
   const string STR = static_cast<string>(integer);
   try {
      cout << __func__ << " tests FAIL of set_integer_array <vector> \n";
      vector<digit_type> vec = { 0, 4 , 5};
      vec.resize(45, 9);
      set(vec, integer);
      assert(false);
   }
   catch (const invalid_argument& e) {
      assert( static_cast<string>(integer) == STR);
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   
   try {
      constexpr array<digit_type, 2> integer_array = { 55, 0};
      cout << __func__ << " tests FAIL of set_integer_array <" << integer_array.size() <<  "> \n";
      set(integer_array, integer);
      assert(false);
   }
   catch (const invalid_argument& e) {
      assert( static_cast<string>(integer) == "0");
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   
   try {
      constexpr array<digit_type, 2> integer_array = { -5, 0};
      cout << __func__ << " tests FAIL of set_integer_array <" << integer_array.size() <<  "> \n";
      set(integer_array, integer);
      assert(false);
   }
   catch (const invalid_argument& e) {
      assert( static_cast<string>(integer) == "0");
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

static void incorrect_parsing_test(const string & STR) {
   try {
      cout << __func__ << " tests FAIL of Integer::parse_create by: '" << STR << "' \n";
      Integer::parse_create(STR);
      assert(false);
   }
   catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

inline void test_set_and_fail_set(Integer & integer) {
   set_integer_array_test(integer);
   fail_set_integer_array_test(integer);
}

static Integer parsing_test() {
   cout << __func__ << " tests Integer::parse_create \n";
   Integer integer = Integer::parse_create("999");
   cout << integer << '\n'; 
   assert(static_cast<string>(integer) == ("+999"));
   
   integer = Integer::parse_create("-78999");
   cout << integer << '\n'; 
   assert(static_cast<string>(integer) == ("-78999"));
   
   integer = Integer::parse_create("0");
   cout << integer << '\n'; 
   assert(static_cast<string>(integer) == ("0"));
   
   integer = Integer::parse_create("7");
   cout << integer << '\n'; 
   assert(static_cast<string>(integer) == ("+7"));
   
   incorrect_parsing_test("888888888888888888888888888888888888888888888888888888888888888");
   incorrect_parsing_test("-99999eee");
   incorrect_parsing_test("-99999edf");
   incorrect_parsing_test("99999 ");
   incorrect_parsing_test("t");
   incorrect_parsing_test(" 5");
   
   assert(static_cast<string>(integer) == ("+7"));
   return integer;
}

void constructor_test() {
   default_constructor_test();
   constexpr array<digit_type, Integer::MAX_ARRAY_LENGTH> integer_array = { 7, 2};
   Integer ia = constructor_test(integer_array, Integer::PLUS);
   assert(string(ia) == "+7200000000000000000000000000000000000000");
   test_set_and_fail_set(ia);
   
   vector<digit_type> vec = { 7, 5, 8, 0};
   Integer iv = constructor_test(vec, Integer::PLUS);
   assert(string(iv) == "+7580");
   test_set_and_fail_set(iv);
   
   constexpr array<digit_type, 1> a2 = { 2 };
   Integer i2 = creation_test<1>(a2, Integer::MINUS);
   assert(string(i2) == "-2");
   test_set_and_fail_set(i2);
   
   parsing_test();
   Parsing_Test::test_parsing("i2", i2);
}

}

using namespace main_integer_test;

int main() {
   try {
      //cout << typeid(array<digit_type, Integer::MAX_ARRAY_LENGTH>()).name() << '\n';
      constructor_test();
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
