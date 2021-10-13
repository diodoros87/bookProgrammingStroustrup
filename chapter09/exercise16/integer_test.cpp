#include "integer.hpp"
#include "integer_parsing.hpp"
#include "assertions.hpp"
#include "parsing_test.hpp"

#include<iostream>
//#define NDEBUG
#include <cassert>
//#include <algorithm>
#include <array>

using namespace std;
using namespace integer_space;

namespace integer_test {
   const array<string, 2> ALLOWED = {"vector", "array"};  // must be static to exist in program all time running
   const array<string, 2>::const_iterator BEGIN = ALLOWED.begin();
   const array<string, 2>::const_iterator END = ALLOWED.end();
   constexpr int INCORRECT_INDEX = -44;
   
   string get_allowed() {
      string result = " Only acceptable type: ";
      for (auto type : ALLOWED) 
         result += " " + type;
      return result;
   }
   
   template <typename Container>
   inline int get_type_index(const Container &) {
      const string TYPE = typeid(Container).name();
      for (int i = 0; i < ALLOWED.size(); i++) 
         if (TYPE.find(ALLOWED[i]) != string::npos)
            return i;
      //const array<string, 2>::iterator it = find (BEGIN, END, TYPE);
      //return it != ALLOWED.end();
      return INCORRECT_INDEX;
   }
   
   template <typename Container>
   string print_container(const string & title, const Container & C) {
      if (INCORRECT_INDEX == get_type_index(C))
         throw invalid_argument(__func__ + get_allowed());
      cout << title;
      for (auto elem : C)
         cout << " " << elem;
      cout << "\nEnd of container printing\n";
   }
   
namespace constructor_test {
   template <typename Container, const unsigned int SIZE>
   void test_incorrect_construct(const string & exception_info, const Container & ARRAY, const char signum) {
      const string TYPE = typeid(Container).name();
      if (TYPE.find("array") == string::npos)
         throw invalid_argument(string(__func__) + " only std::array is acceptable argument");
      try {
            Integer::create_Integer<SIZE>(ARRAY, signum);
            assert(false);
      } catch (const invalid_argument & e) {
         cerr << __func__ << " Exception while construct integer '" << exception_info << "' : " << e.what() << "\n";
      }
   }
   
   void test_incorrect_construct(const string & exception_info, const vector<digit_type> & vec, const char signum) {
      try {
         Integer(vec, signum);
         assert(false);
      } catch (const invalid_argument & e) {
         cerr << __func__ << " Exception while construct integer '" << exception_info << "' : " << e.what() << "\n";
      }
   }
   
   template <typename Container, const unsigned int SIZE>
   inline void array_to_vector_fail_construct(const string & exception_info, const Container & ARRAY, const char signum) {
      static_assert( SIZE <= Integer::MAX_ARRAY_LENGTH && SIZE > 0 && "create_Integer requires 0 < SIZE <= MAX_ARRAY_LENGTH");
      const string TYPE = typeid(Container).name();
      if (TYPE.find("array") == string::npos)
         throw invalid_argument(string(__func__) + " only std::array is acceptable argument");
      vector<digit_type> vec = { ARRAY.begin(), ARRAY.end() };
      test_incorrect_construct(exception_info, vec, signum);
   }
   
   void test_constructors() {
      cout << "\n CORRECT CONSTRUCT OF OBJECTS:\n";
      Integer first  = {};
      cout << "After construct by default constructor: " << static_cast<string>(first) << "\n";
      assert(static_cast<string>(first) == ("0"));

      array<digit_type, 5> numbers = {1, 2, 3, 0, 6};
      char signum = Integer::PLUS;
      Integer second  = Integer::create_Integer<5>(numbers, signum);
      cout << "After construct by array and signum's byte of '" << signum << "' : " << second << "\n";
      assert(static_cast<string>(second)==("+12306"));

      Integer third  = second;
      cout << "After construct by other integer: " << static_cast<string>(third) << "\n";
      assert(static_cast<string>(third)==("+12306"));

      cout << "\n INCORRECT ATTEMPTS TO CONSTRUCT OF OBJECTS:\n";

      vector<digit_type> too_large_vector_size(Integer::MAX_ARRAY_LENGTH + 3);
      assert(too_large_vector_size.size() == Integer::MAX_ARRAY_LENGTH + 3);
      test_incorrect_construct("with too large vector's size", too_large_vector_size, Integer::NEUTRAL);

      array<digit_type, 5> non_digits_array = {11, 2, 5, 0, 8};
      test_incorrect_construct<array<digit_type, 5>, 5>("with non digit element in array", non_digits_array, Integer::NEUTRAL);
      //vector<digit_type> non_digits_vec (non_digits_array.begin(), non_digits_array.end() );
      array_to_vector_fail_construct<array<digit_type, 5>, 5>("with non digit element in vector", non_digits_array, Integer::NEUTRAL);

      array<digit_type, 5> minus_array = {1, -2, 5, 0, 8};
      test_incorrect_construct<array<digit_type, 5>, 5>("with minus digit element in array", minus_array, Integer::NEUTRAL);

      array<digit_type, 5> zero_array = {0, 0, 0, 0, 0};
      signum = Integer::PLUS;
      test_incorrect_construct<array<digit_type, 5>, 5>("with incorrect signum " + string(1, signum) + " for zero array", zero_array, signum);
      signum = Integer::MINUS;
      test_incorrect_construct<array<digit_type, 5>, 5>("with incorrect signum " + string(1, signum) + " for zero array", zero_array, signum);

      array<digit_type, 5> non_zero_array = {0, 0, 0, 0, 1};
      signum = Integer::NEUTRAL;
      test_incorrect_construct<array<digit_type, 5>, 5>("with incorrect signum '" + string(1, signum) + "' for non-zero array", non_zero_array, signum);

      cout << "\n ------------------------\n";
   }
}
}
/*
{
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
   if (Integer::NEUTRAL == sign)
      sign = Integer::PLUS;
   array<digit_type, 5> a = { 7, 2, 7, 0};
   set(a, integer);

   integer_assert(integer, sign, "72700");
   vector<digit_type> vec = { 0, 4 , 5};
   set(vec, integer);
   integer_assert(integer, sign, "45");
}

void fail_set_integer_array_test(Integer & integer) {
   try {
      cout << __func__ << " tests FAIL of set_integer_array <vector> \n";
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
      cout << __func__ << " tests FAIL of set_integer_array <" << integer_array.size() <<  "> \n";
      set(integer_array, integer);
      assert(false);
   }
   catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

static void incorrect_parsing_test(const string & STR) {
   try {
      cout << __func__ << " tests FAIL of Integer::parse_create by: '" << STR << "' \n";
      Integer integer = Integer::parse_create(STR);
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
   
   parsing_test();
   Parsing_Test::test_parsing("i2", i2);
}

}
*/

using namespace integer_test;

int main() {
   try {
      constructor_test::test_constructors();
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
