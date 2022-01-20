#define DEBUG_OSTREAM

#include "integer.hpp"
#include "assertions.hpp"
#include "money.hpp"

#include <cmath>

#include "integers_extremums_tests.hpp"
#include "money_operations_test.hpp"
#include "money_init_test.hpp"
#include "money_utility_test.hpp"

#include <functional>

using namespace std;
using namespace integer_space;
using namespace money;
using namespace money_init_test;

template <typename T> 
extern
Money<T> construct_cents(const string & DOLLARS, const double CENTS, bool creating, const string & expected = "");

template <typename T> 
extern
Money<T> construct(const string & DOLLARS, bool creating, const string & expected/* = ""*/);

namespace money_operations_test {
  /* 
template <typename T, typename Function, typename... Args>  
void operation_incorrect(Function && f, Args&&... args ) { 
   try {
      f(std::forward<Args>(args)...);
      T obj = f(std::forward<Args>(args)...);
      cerr << __func__ << " !!!!!!!!!!!!!!!!!!" << obj << endl;
      assert(0);
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}

template <typename T, typename Function, typename... Args>  
void construct_incorrect(Function && f, Args&&... args ) { 
   try {
      f(std::forward<Args>(args)...);
      T obj = f(std::forward<Args>(args)...);
      cerr << __func__ << " !!!!!!!!!!!!!!!!!!" << obj << endl;
      assert(0);
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}
*/
  
//typedef  int (Fred::*FredMemFn)(char x, float y);
//template <typename T, typename T (T::*oper)(const& T)> 
template <typename Type> 
void binary_operation(Type& (Type::*func)(const Type&) /*const*/, const string & A_DOLLARS, const double A_CENTS, const string & B_DOLLARS, 
                      const double B_CENTS, const string & expected = "") { 
   Type       A(A_DOLLARS, A_CENTS);
   const Type B(B_DOLLARS, B_CENTS);
   Type RESULT = (A.*func)(B);
   print_assert(RESULT, expected);
   A + B;
   print_assert(A, expected);
}

template <typename Type, typename Function> 
void binary_operation(Function && func, const string & A_DOLLARS, const double A_CENTS, const string & B_DOLLARS, 
                      const double B_CENTS, const string & expected = "") { 
   const Type A(A_DOLLARS, A_CENTS);
   const Type B(B_DOLLARS, B_CENTS);
   const Type RESULT = func(A, B);
   print_assert(RESULT, expected);
}

template <typename T, typename Function> 
void unary_operation(Function && func, const string & A_DOLLARS, const double A_CENTS, const string & expected = "") { 
   const Money<T> A(A_DOLLARS, A_CENTS);
   const Money<T> RESULT = func(A);
   print_assert(RESULT, expected);
}

/*
template <typename T>
Money<T> construct(const string & DOLLARS, bool creating, const string & expected = ") { 
   Money<T> money(DOLLARS);
   print_assert(money, expected);
   if (creating) {
      Money<T> money_creating = Money<T>::create(DOLLARS);
      print_assert(money_creating, expected);
      assert(money == money_creating);
   }
   else
      construct_incorrect<Money<T>>(Creation<T>(), DOLLARS);
   return money;
}

template <typename T> 
void incorrect() { 
   try {
      Money<T> money("6", -34);
      cerr << __func__ << "money = " << money << '\n';
      assert(0);
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
   try {
      Money<T> money("6", 100);
      cout << __func__ << "money = " << money << '\n';
      assert(0);
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}

template <typename T> 
void construct() {
   cerr << __func__ << '\n';
   construct_cents<T>("0", 99, true, "0,99");
   construct_cents<T>("1", 1, true, "1,01");
   construct_cents<T>("1", 0, true, "1,00");
   construct_cents<T>("0", 99.9, false, "1,00");
   construct_cents<T>("0", 0, true, "0,00");
   construct<T>("0.99999", false, "1,00");
   construct<T>("0.5", true, "0,50");
   construct<T>("0.50", true, "0,50");
   construct<T>("0.500", true, "0,50");
   construct_cents<T>("1", 2.5, false, "1,03");
   construct_cents<T>("0", 2.4, false, "0,02");
   if (numeric_limits<T>::is_signed) {
      construct_cents<T>("-1", 2.5, false, "-1,03");
      construct_cents<T>("-0", 2.4, false, "-0,02");
      construct_cents<T>("-1", 0, true, "-1,00");
      construct_cents<T>("-1", 1, true, "-1,01");
      construct_cents<T>("-1", 3, true, "-1,03");
      construct_cents<T>("-0", 99.9, false, "-1,00");
      construct_cents<T>("-0", 1, true, "-0,01");
      construct_cents<T>("-0", 99, true, "-0,99");
      construct<T>("-0.99999", false, "-1,00");
      construct<T>("-0.5", true, "-0,50");
      construct<T>("-0.50", true, "-0,50");
      construct<T>("-0.500", true, "-0,50");
      construct_cents<T>("-0", 5.5, false, "-0,06");
   }
   if (! is_same<T, char>::value && ! is_same<T, int_fast8_t>::value) {
      construct<T>("1.99999", false, "2,00");
      
      construct_cents<T>("20", 55, true, "20,55");
      if (! is_same<T, short>::value)
         construct_cents<T>("577", 45.7, false, "577,46");
      if (numeric_limits<T>::is_signed) {
         if (! is_same<T, short>::value)
            construct_cents<T>("-8577", 45.79, false, "-8577,46");   
         construct<T>("-1.99999", false, "-2,00");
         construct_cents<T>("-1", 99.9, false, "-2,00");
         construct_cents<T>("-1", 99, true, "-1,99");
         
         construct<T>("-10.67", true, "-10,67");
         construct<T>("-10.679", false, "-10,68");
         construct<T>("-10.6435", false, "-10,64");
         construct<T>("-10.099", false, "-10,10");
      }
      construct<T>("10", true, "10,00");
      
      if (is_same<T, Integer>::value) {
         construct_cents<T>("34028234663852885981782907897774079", 99,    true, "34028234663852885981782907897774079,99" );
         construct_cents<T>("-34028234663852885981782907897774079", 99,    true, "-34028234663852885981782907897774079,99" );
         construct<T>("34028234663852885981782907897774079.99",    true, "34028234663852885981782907897774079,99" );
         construct<T>("-34028234663852885981782907897774079.99",    true, "-34028234663852885981782907897774079,99" );
         construct<T>("34028234663852885981782907897774079.994",    false, "34028234663852885981782907897774079,99" );
         construct<T>("-34028234663852885981782907897774079.994",    false, "-34028234663852885981782907897774079,99" );
         construct<T>("34028234663852885981782907897774079.995",    false, "34028234663852885981782907897774080,00" );
         construct<T>("-34028234663852885981782907897774079.995",    false, "-34028234663852885981782907897774080,00" );
         construct<T>("34028234663852885981782907897774080.01",    true, "34028234663852885981782907897774080,01" );
         construct<T>("-34028234663852885981782907897774080.01",    true, "-34028234663852885981782907897774080,01" );
         construct<T>("34028234663852885981782907897774080.00",    true, "34028234663852885981782907897774080,00" );
         construct<T>("-34028234663852885981782907897774080.00",    true, "-34028234663852885981782907897774080,00" );
         construct<T>("34028234663852885981782907897774080.797",    false, "34028234663852885981782907897774080,80" );
         construct<T>("-34028234663852885981782907897774080.797",    false, "-34028234663852885981782907897774080,80" );
         construct<T>("34028234663852885981782907897774080.793333333", false, "34028234663852885981782907897774080,79" );
         construct<T>("-34028234663852885981782907897774080.793333333", false, "-34028234663852885981782907897774080,79" );
         construct<T>("99999999999999999999999999999999999999.99", true, "99999999999999999999999999999999999999,99" );
         construct<T>("-99999999999999999999999999999999999999.99", true, "-99999999999999999999999999999999999999,99" );
         construct<T>("99999999999999999999999999999999999999.990", true, "99999999999999999999999999999999999999,99" );
         construct<T>("-99999999999999999999999999999999999999.990", true, "-99999999999999999999999999999999999999,99" );
         construct<T>("99999999999999999999999999999999999999.991", false, "99999999999999999999999999999999999999,99" );
         construct<T>("-99999999999999999999999999999999999999.991", false, "-99999999999999999999999999999999999999,99" );
         construct<T>("99999999999999999999999999999999999999.994", false, "99999999999999999999999999999999999999,99" );
         construct<T>("-99999999999999999999999999999999999999.994", false, "-99999999999999999999999999999999999999,99" );

         construct<T>(std::to_string( numeric_limits<float>::max() / CONVERTER / 100 ),    true, "34028234663852885981782907897774080,00" );
         construct<T>(std::to_string( numeric_limits<float>::lowest() / CONVERTER / 100 ), true, "-34028234663852885981782907897774080,00");
         construct<T>(std::to_string( numeric_limits<float>::max() / CONVERTER ),    true, "3402823466385288598232333985305853952,00" );
         construct<T>(std::to_string( numeric_limits<float>::lowest() / CONVERTER ), true, "-3402823466385288598232333985305853952,00");
      }
      if (is_floating_point<T>::value) {
         construct<T>("8.577444e+02", false, "857,74");
         construct<T>("8.577555e+02", false, "857,76");
         construct<T>(".50", true, "0,50");
         construct<T>(".5", true, "0,50");
         construct<T>(".500", true, "0,50");
         construct<T>(".501", false, "0,50");
         construct<T>("+.501", false, "0,50");
         construct<T>("50.", true, "50,00");
         construct<T>("5.", true, "5,00");
         construct<T>("05.", true, "5,00");
         construct<T>("-8.577444e+02", false, "-857,74");
         construct<T>("-8.577555e+02", false, "-857,76");
         construct<T>("-.50", true, "-0,50");
         construct<T>("-.5", true, "-0,50");
         construct<T>("-.500", true, "-0,50");
         construct<T>("-.501", false, "-0,50");
         construct<T>("-50.", true, "-50,00");
         construct<T>("-5.", true, "-5,00");
         construct<T>("-05.", true, "-5,00");
         if (is_same<T, float>::value) {
            construct<T>(std::to_string( numeric_limits<float>::max() / CONVERTER / 100 ),    true, "34028235832468283066569397853224960,92" );
            construct<T>(std::to_string( numeric_limits<float>::lowest() /CONVERTER / 100 ), true, "-34028235832468283066569397853224960,92");
            construct<T>("-8577e+03", true, "-8577000,68");
         }
         if (is_same<T, double>::value || is_same<T, long double>::value) {
            construct<T>("-8577e+03", true, "-8577000,00");
         }
      }
      construct_cents<T>("20", 5.5, false, "20,06");
      construct<T>("10.67", true, "10,67");
      construct<T>("10.679", false, "10,68");
      construct<T>("10.6435", false, "10,64");
      construct<T>("10.099", false, "10,10");
   }
   cerr << "END OF " << __func__ << '\n';
}

template <typename T> 
void incorrect_construct() {
   cerr << __func__ << '\n';
   incorrect<T>();
   construct_incorrect<Money<T>>(Constructor<T, Money>(), "20.8", 5.0L);
   construct_incorrect<Money<T>>(Constructor<T, Money>(), "20.8", 5);
   construct_incorrect<Money<T>>(Constructor<T, Money>(), "20t", 7);
   if (is_same<T, char>::value || is_same<T, int_fast8_t>::value) {
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-1.99999");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "1.99999");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "189.99999");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(1 + numeric_limits<long double>::max() / CONVERTER)));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(-1 + numeric_limits<long double>::lowest() / CONVERTER)));
   }
   else if (is_same<T, Integer>::value || is_same<T, float>::value) {
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<long double>::max() / CONVERTER)));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<long double>::lowest() / CONVERTER)));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<double>::max() / CONVERTER)));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<double>::lowest() / CONVERTER)));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<float>::max())));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<float>::lowest())));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "34028234663852885981782907897774080793333.333");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-34028234663852885981782907897774080793333.333");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "99999999999999999999999999999999999999.999" );
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-99999999999999999999999999999999999999.999" );
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "99999999999999999999999999999999999999.995" );
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-99999999999999999999999999999999999999.995" );
      if (is_same<T, float>::value) {
         construct_incorrect<Money<float>>(Constructor<float, Money>(), "99999999999999999999999999999999999999.99" );
         construct_incorrect<Money<float>>(Constructor<float, Money>(), "-99999999999999999999999999999999999999.99" );
      }

   }
   
   if (is_unsigned<T>::value)
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-1", 4);

   if (numeric_limits<T>::is_integer) {
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-8577e+03");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-8.577444e+02");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "8.577555e+02");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), ".50");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "50.");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-.50");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-50.");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), ".5");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "05.");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-.5");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-50.");
   }
   
   construct_incorrect<Money<T>>(Constructor<T, Money>(), "inf", 8);
   construct_incorrect<Money<T>>(Constructor<T, Money>(), "inf");
   construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(std::nan(""))));
   construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<float>::infinity())));
   construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<double>::infinity())));
   construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<long double>::infinity())));

   cerr << "END OF " << __func__ << '\n';
}

template <typename T>
inline void test() {
   construct<T>();
   incorrect_construct<T>();
}

template <typename T>
void test_max_8bits_value() {
   constexpr T MAX_C = numeric_limits<T>::max();
   constexpr T MIN_C = numeric_limits<T>::lowest();
   construct<T>((std::to_string(MAX_C / CONVERTER)), true, "1,27");
   construct<T>((std::to_string(MIN_C / CONVERTER)), true, "-1,28");

}

void test_extremums() {
   Integers_extremums_test<Integer>::test();
   Integers_extremums_test<char>::test();
   Integers_extremums_test<int_fast8_t>::test();
   Integers_extremums_test<short>::test();
   Integers_extremums_test<unsigned short>::test();
   Integers_extremums_test<int>::test();
   Integers_extremums_test<unsigned>::test();
   Integers_extremums_test<long>::test();
   Integers_extremums_test<unsigned long>::test();
   Integers_extremums_test<long long>::test();
   Integers_extremums_test<unsigned long long>::test();
}

void test_Integer_overflow() {
   const array <Integer, 2> EXTREMUMS = { numeric_limits<Integer>::max(), numeric_limits<Integer>::lowest() };
   const Integer FACTOR = Integer(15);
   const Integer Integer_CONVERTER = Integer(CONVERTER);
   for (size_t i = 0; i < EXTREMUMS.size(); i++)
      try {
         const string MAX_STR = std::to_string((FACTOR * EXTREMUMS[i]) / Integer_CONVERTER);
         cout << __func__ << " MAX_STR for type = " << typeid(Integer).name() << " " << MAX_STR << '\n';
         assert(0);
      } catch (const out_of_range& e) {
         cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
      }
}
*/

template <typename Type, template<typename> class Template>
inline void test_adding(const string & A_DOLLARS, const double A_CENTS, const string & B_DOLLARS, 
                      const double B_CENTS, const string & expected = "") { 
   binary_operation<Template<Type>> (&Template<Type>::operator+, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
   binary_operation<Template<Type>, Template<Type>(const Template<Type>&, const Template<Type>&)> (operator+, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
}

void perform() {
   //binary_operation<char, Money<char> Money<char>::(const Money<char>& other)>(Money<char>::operator+, "23", 0, "34", 0, "57,00");
   //binary_operation<Money<char>>(&Money<char>::operator+, "0", 23, "0", 34, "0,57");
   test_adding<Integer, Money>("100", 23, "-450", 34, "-350,11");
   test_adding<char, Money>("0", 23, "0", 34, "0,57");
   
   ///binary_operation<Money<Integer>>(&Money<Integer>::operator+, "100", 23, "-450", 34, "-350,11");
   //binary_operation<Money<Integer>>(Money<Integer> (&operator+)(const Money<Integer> &, const Money<Integer> &), "100", 23, "-450", 34, "-350,11");
   ///binary_operation<Money<Integer>, Money<Integer>(const Money<Integer>&, const Money<Integer>&)>(operator+, "100", 23, "-450", 34, "-350,11");
   //binary_operation<Money<Integer>>(&Money<Integer>::operator*, "100", 23, "-450", 34, "-350,11");
   /*
   binary_operation<int_fast8_t>();
   binary_operation<short>();
   binary_operation<unsigned short>();
   binary_operation<int>();
   binary_operation<unsigned int>();
   binary_operation<long>();
   binary_operation<unsigned long>();
   binary_operation<long long>();
   binary_operation<unsigned long long>();
   binary_operation<long double>();
   binary_operation<double>();
   binary_operation<float>();
   binary_operation<Integer>();*/
   
   cerr << '\n' << __func__ << ": END of ALL TESTS\n";
}

}
