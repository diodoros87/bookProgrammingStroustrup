#include "integer.hpp"
#include "integer_parsing.hpp"
#include "assertions.hpp"

#include<iostream>
//#define NDEBUG
#include <cassert>

using namespace std;
using namespace integer_space;

template <size_t SIZE>
static Integer set(const array<digit_type, SIZE> & table, Integer & integer) {
   cout << __func__ << "\n";
   integer.set_integer_array<SIZE>(table);
   cout << integer << '\n';
   return integer;
}

static Integer set(const vector<digit_type> & vec, Integer & integer) {
   cout << __func__ << "\n";
   integer.set_integer_array(vec);
   cout << integer << '\n';
   return integer;
}

static Integer constructor_test(const array<digit_type, Integer::MAX_ARRAY_LENGTH> & table, const char signum) {
   cout << __func__ << '\n';
   Integer i = Integer(table, signum);
   cout << i << '\n';
   return i;
}

template <unsigned int N>
static Integer creation_test(const array<digit_type, N> & table, const char signum) {
   cout << __func__ << '\n';
   Integer i =  Integer::create_Integer<N>(table, signum);
   cout << i << '\n';
   return i;
}

static Integer constructor_test(const vector<digit_type> & vec, const char signum) {
   cout << __func__ << '\n';
   Integer i (vec, signum);
   cout << i << '\n';
   return i;
}

void set_integer_array_test(Integer & integer) {
   cout << __func__ << '\n';
   char sign = integer.get_signum();
   if (Integer::NEUTRAL == sign)
      sign = Integer::PLUS;
   array<digit_type, 5> a = { 7, 2, 7, 0};
   set(a, integer);
   //string integer_string = sign + "72700";
   integer_assert(integer, sign, "72700");
   //assert(static_cast<string>(integer) == integer_string);
   vector<digit_type> vec = { 0, 4 , 5};
   set(vec, integer);
   //integer_string = sign + "45";
   integer_assert(integer, sign, "45");
   //assert(static_cast<string>(integer) == integer_string);
}

void fail_set_integer_array_test(Integer & integer) {
   try {
      vector<digit_type> vec = { 0, 4 , 5};
      vec.resize(45, 9);
      set(vec, integer);
      assert(false);
   }
   catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   
   try {
      const array<digit_type, Integer::MAX_ARRAY_LENGTH + 1> integer_array = { 5, 0};
      set(integer_array, integer);
      assert(false);
   }
   catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

void test_set_and_fail_set(Integer & integer) {
   set_integer_array_test(integer);
   const string STR = static_cast<string>(integer);
   fail_set_integer_array_test(integer);
   assert( static_cast<string>(integer) == STR);
}

void constructor_test() {
   const array<digit_type, Integer::MAX_ARRAY_LENGTH> integer_array = { 7, 2};
   Integer ia = constructor_test(integer_array, Integer::PLUS);
   assert(string(ia) == "+7200000000000000000000000000000000000000");
   test_set_and_fail_set(ia);
   
   vector<digit_type> vec = { 7, 5, 8, 0};
   Integer iv = constructor_test(vec, Integer::PLUS);
   assert(string(iv) == "+7580");
   test_set_and_fail_set(iv);
   
   const array<digit_type, 1> a2 = { 2 };
   Integer i2 = creation_test<1>(a2, Integer::MINUS);
   assert(string(i2) == "-2");
   test_set_and_fail_set(i2);
   
   //const array<digit_type, 7> seven = { 0, 1, 3, 2};
   //Integer i7 = constructor_test<7>(seven, Integer::PLUS);
   //set_integer_array_test(i7);
}

void print(const array<digit_type, Integer::MAX_ARRAY_LENGTH> & integer_array) {
   cerr << " array<digit_type, Integer::MAX_ARRAY_LENGTH> \n";
   
   for (short index = 0; index < integer_array.size(); index++) {
      cerr << index << ". " << integer_array[index] << '\n';
      //cerr << number;
      //result += to_string(number);
   }
   cerr << " array<digit_type, Integer::MAX_ARRAY_LENGTH> END \n";
}

int main() {
   try {
      constructor_test();
//       array<digit_type, Integer::MAX_ARRAY_LENGTH> integer_array = { 7, 2};
//       constructor_test(integer_array, Integer::PLUS);
//       Integer i (integer_array, plus);
//       cout << i << '\n';
//       vector<digit_type> t = { 7, 2};
//       print (t);
//       Integer ti (t, plus);
//       cout << ti << '\n';
//       test();
      //incorrect_test();
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
