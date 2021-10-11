#include "integer.hpp"
#include "integer_parsing.hpp"

#include<iostream>
#include <cassert>

using namespace std;
using namespace integer_space;

template <size_t SIZE>
static Integer set(const array<digit_type, SIZE> & table, Integer & integer) {
   cout << __func__ << "\n";
   integer.set_integer_array<SIZE>(table);
   cout << integer << '\n';
   assert(table.size()==integer.size());
   return integer;
}

template <unsigned int N>
static Integer constructor_test(const array<digit_type, N> & table, const char signum) {
   cout << __func__ << '\n';
   Integer i =  Integer  (table, signum) <N> ;
   cout << i << '\n';
   assert(table.size()==i.size());
   return i;
}

static Integer constructor_test(const vector<digit_type> & vec, const char signum) {
   cout << __func__ << '\n';
   Integer i (vec, signum);
   cout << i << '\n';
   assert(vec.size()==i.size());
   return i;
}
// void constructor_test(const array<digit_type, Integer::MAX_ARRAY_LENGTH> & integer_array) { 
//    Integer i;
//    cout << i << '\n';
// }

void set_integer_array_test(Integer & integer) {
   array<digit_type, 5> a = { 7, 2, 7, 0};
   set(a, integer);
   assert(a.size()==integer.size());
}

void constructor_test() {
   const array<digit_type, Integer::MAX_ARRAY_LENGTH> integer_array = { 7, 2};
   Integer a = constructor_test<Integer::MAX_ARRAY_LENGTH>(integer_array, Integer::PLUS);
   set_integer_array_test(a);
   
   vector<digit_type> vec = { 7, 5, 8, 0};
   Integer v = constructor_test(vec, Integer::PLUS);
   set_integer_array_test(v);
   
   const array<digit_type, 7> seven = { 0, 1, 3, 2};
   Integer i7 = constructor_test<7>(seven, Integer::PLUS);
   set_integer_array_test(i7);
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
