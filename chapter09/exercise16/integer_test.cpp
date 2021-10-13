#include "integer.hpp"
#include "integer_parsing.hpp"
#include "assertions.hpp"
#include "parsing_test.hpp"

#include<iostream>
//#define NDEBUG
#include <cassert>
#include <algorithm>

using namespace std;
using namespace integer_space;

namespace integer_test {
   
namespace constructor_test {
   const array<string, 2> ALLOWED = {"vector", "array"};  // must be static to exist in program all time running
   const array<string, 2>::iterator BEGIN = ALLOWED.begin();
   const array<string, 2>::iterator END = ALLOWED.end();
   template <typename Container>
   void test_incorrect_construct(const string & exception_info, Container & c, const char signum) {
      const string TYPE = typeid(Container).name();
      if (find (BEGIN, END) == ALLOWED.end())
         throw invalid_argument(__func__ + " acceptable type");
      try {
         HugeInteger hugeInteger = new HugeInteger(integerArray, signum);
         System.out.printf("???  number after construct %s: %s%n", exceptionInfo, hugeInteger);
         assert(false);
      } catch (NullPointerException exception) {
         System.out.printf("%nException while construct integer \'%s\': %s%n", exceptionInfo, exception.getMessage());
         exception.printStackTrace();
      } catch (IllegalArgumentException exception) {
         System.out.printf("%nException while construct integer \'%s\': %s%n", exceptionInfo, exception.getMessage());
         exception.printStackTrace();
      }
   }
   
   void test_constructors() {
      cout << "\n CORRECT CONSTRUCT OF OBJECTS:\n";
      Integer first  = {};
      cout << "After construct by default constructor: " + static_cast<string>(first) + "\n";
      assert(static_cast<string>(first) == ("0"));

      array<digit_type, 5> numbers = {1, 2, 3, 0, 6};
      char signum = Integer::PLUS;
      Integer second  = {numbers, signum};
      cout << "After construct by array and signum's byte of '" + signum + "' : " + second + "\n";
      assert(static_cast<string>(second)==("+12306"));

      Integer third  = second;
      cout << "After construct by other integer: " + static_cast<string>(third) + "\n";
      assert(static_cast<string>(third)==("+12306"));

      cout << "\n INCORRECT ATTEMPTS TO CONSTRUCT OF OBJECTS:\n";
      //test_incorrect_construct("with null array", null, (byte)0);

      byte[] tooLargeArraySize = new byte[HugeInteger.MAX_ARRAY_LENGTH + 3];
      testIncorrectConstruct("with too large array's size", tooLargeArraySize, (byte)0);

      byte[] nonDigitsArray = {11, 2, 5, 0, 8};
      testIncorrectConstruct("with non digit element in array", nonDigitsArray, (byte)0);

      byte[] minusArray = {1, -2, 5, 0, 8};
      testIncorrectConstruct("with minus digit element in array", minusArray, (byte)0);

      byte[] zeroArray = {0, 0, 0, 0, 0};
      signum = +1;
      testIncorrectConstruct(String.format("with incorrect signum %+d for zero array", signum), zeroArray, signum);

      byte[] nonZeroArray = {0, 0, 0, 0, 1};
      signum = 0;
      testIncorrectConstruct(String.format("with incorrect signum %+d for non-zero array", signum), nonZeroArray, signum);

      System.out.printf("%n ------------------------%n");
   }
}

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

using namespace integer_test;

static const vector<string>& get_allowed_types() {
   
   return types;
}

template<class T>
void print_type_name(const T & x) {
   const string NAME = typeid(T).name();
   cout << NAME << '\n';
   const size_t CODE = typeid(T).hash_code();
   cout << CODE << '\n';
}

int main() {
   try {
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
