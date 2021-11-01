#include "integer.hpp"
#include "assertions.hpp"
#include "money.hpp"

using namespace std;
using namespace integer_space;
using namespace money;

using std::is_signed;
using std::is_unsigned;


template <typename T> 
Money<T> construct(const string & DOLLARS, const double CENTS, const string & expected = "") { 
   Money<T> money(DOLLARS, CENTS);
   cout << "                                       money = '" << money << "'\n";
   auto t = std::make_tuple(static_cast<string>(money), " != ", expected); 
   assert_many(string(money) == expected, t);
   return money;
}

template <typename T> 
Money<T> construct(const string & DOLLARS, const string & expected = "") { 
   Money<T> money(DOLLARS);
   cout << "                                       money = '" << money << "'\n";;
   auto t = std::make_tuple(static_cast<string>(money), " != ", expected); 
   assert_many(static_cast<string>(money) == expected, t);
   //assert_many(static_cast<string>(money) == expected, static_cast<string>(money), " != ", expected);
   return money;
}

template <typename T> 
void construct_incorrect(const string & DOLLARS, const double CENTS ) { 
   try {
      Money<T> money(DOLLARS, 34);
      assert(0);
      cerr << __func__ << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   try {
      Money<T> money(DOLLARS, CENTS);
      assert(0);
      cout << __func__ << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

template <typename T> 
void incorrect() { 
   try {
      Money<T> money("6", -34);
      assert(0);
      cerr << __func__ << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   try {
      Money<T> money("6", 100);
      assert(0);
      cout << __func__ << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

template <typename T> 
void construct_incorrect(const string & DOLLARS) { 
   try {
      Money<T> money(DOLLARS);
      assert(0);
      cout << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

template <typename T> 
void construct() {
   cerr << __func__ << '\n';
   construct<T>("0", 99, "0,99");
   construct<T>("1", 1, "1,01");
   construct<T>("1", 0, "1,00");
   construct<T>("0", 99.9, "1,00");
   if (numeric_limits<T>::is_signed) {
      construct<T>("-1", 0, "-1,00");
      construct<T>("-1", 1, "-1,01");
      construct<T>("-1", 3, "-1,03");
      construct<T>("-0", 99.9, "-1,00");
      construct<T>("-0", 1, "-0,01");
      construct<T>("-0", 99, "-0,99");
      construct<T>("-0.99999", "-1,00");
      construct<T>("-0.5", "-0,50");
   }
   construct<T>("0.99999", "1,00");
   construct<T>("0.5", "0,50");
   if (! is_same<T, char>::value && ! is_same<T, int_fast8_t>::value) {
      construct<T>("1.99999", "2,00");
      
      construct<T>("20", 55, "20,55");
      construct<T>("577", 45.7, "577,46");
      if (numeric_limits<T>::is_signed) {
         construct<T>("-8577", 45.79, "-8577,46");   
         construct<T>("-1.99999", "-2,00");
         construct<T>("-1", 99.9, "-2,00");
         construct<T>("-0.5", "-0,50");
         construct<T>("-1", 99, "-1,99");
      }
      construct<T>("10", "10,00");
      if (is_same<T, float>::value)
         construct<T>("-8577e+03", "-8577000,68");
      else if (is_same<T, long double>::value)
         construct<T>("-8577e+03", "-8577000,00");
      construct<T>("10.67", "10,67");
      construct<T>("10.679", "10,68");
      construct<T>("10.6435", "10,64");
      construct<T>("10.099", "10,10");
   }
   cerr << "END OF " << __func__ << '\n';
}

template <typename T> 
void construct_incorrect() {
   cerr << __func__ << '\n';
   incorrect<T>();
   construct_incorrect<T>("20.8", 5);
   construct_incorrect<T>("20t", 7);
   if (is_same<T, char>::value || is_same<T, int_fast8_t>::value) {
      construct_incorrect<T>("-1.99999");
      construct_incorrect<T>("1.99999");
   }
   //construct_incorrect<T>("-0", 4);
   if (is_unsigned<T>::value)
      construct_incorrect<T>("-1", 4);
   //construct_incorrect<T>("+0", 4);
   if (numeric_limits<T>::is_integer)
      construct_incorrect<T>("-8577e+03");
   construct_incorrect<T>("inf", 8);
   construct_incorrect<T>("inf");
   //construct_incorrect<T>("57.7");
   cerr << "END OF " << __func__ << '\n';
}

template <typename T>
inline void test() {
   construct<T>();
   construct_incorrect<T>();
}

int main() {
   try {
      test<char>();
      test<int_fast8_t>();
      test<int>();
      test<long double>();
      test<double>();
      test<float>();
      test<Integer>();
      test<unsigned int>();
      
      //test<string>();
      return 0;
   } catch (const bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const Arithmetic_Error & e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const runtime_error& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const exception& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (...) {
      cerr << __func__ << " unrecognized exception\n";
   }
   return 1;
}
