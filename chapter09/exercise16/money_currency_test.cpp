//#include "integers_extremums_tests.hpp"
#include "money_currency_test.hpp"
//#include "money_init_test.hpp"
#include "money_utility_test.hpp"

#include "money_init_test.hpp"
#include "integers_extremums_tests.hpp"

//#include <functional>

using namespace std;
using namespace integer_space;
using namespace money;
using namespace money_init_test;


/*
template <typename T, typename Function, typename... Args>  
extern
void money_init_test::construct_incorrect(Function && f, Args&&... args );
*/

namespace money_init_test {

template <typename T> 
extern
Money<T> construct_cents(const string & DOLLARS, const long double CENTS, bool creating, const string & expected/* = ""*/, const string & currency/* = "PLN"*/, const bool NOT_assert_currency/* = true*/);

template <typename T> 
extern
Money<T> construct(const string & DOLLARS, bool creating, const string & expected/* = ""*/, const string & currency/* = "PLN"*/, const bool NOT_assert_currency/* = true*/);

template <typename T, typename Function, typename... Args>  
extern
void construct_incorrect(Function && f, Args&&... args );

}
/*
namespace money_currency_test {

template <typename T>
class Currency_test {
public:
   static void construct() {
      cerr << "\start of Currency_test::" << __func__ << '\n';
      money_init_test::construct_cents<T>("0", 99, true, "0,99 USD", "USD", false);
      money_init_test::construct_cents<T>("1", 1, true, "1,01 EUR", "EUR", false);
      money_init_test::construct_cents<T>("1", 0, true, "1,00 PLN", "PLN", false);
      money_init_test::construct_cents<T>("0", 99.9, false, "1,00 GBP", "GBP", false);
      money_init_test::construct_cents<T>("-0", 0, true, "0,00 CHF", "CHF", false);
      money_init_test::construct_cents<T>("0", 0, true, "0,00 JPY", "JPY", false);
      money_init_test::construct<T>("0.99999", false, "1,00 CAD", "CAD", false);
      money_init_test::construct<T>("-0", true, "0,00 JPY", "JPY", false);
      money_init_test::construct<T>("0.5", true, "0,50 JPY", "JPY", false);
      money_init_test::construct<T>("0.50", true, "0,50 JPY", "JPY", false);
      money_init_test::construct<T>("0.500", true, "0,50 JPY", "JPY", false);
      money_init_test::construct_cents<T>("1", 2.5, false, "1,03 GBP", "GBP", false);
      money_init_test::construct_cents<T>("0", 2.4, false, "0,02 GBP", "GBP", false);
      cerr << "\nEND of Currency_test::" << __func__ << '\n';
   }
};
}
*/

namespace money_currency_test {
   
template <typename T>
void construct() {
   cerr << "\nstart of money_currency_test::" << __func__ << '\n';
   money_init_test::construct_cents<T>("0", 99, true, "0,99 USD", "USD", false);
   money_init_test::construct_cents<T>("1", 1, true, "1,01 EUR", "EUR", false);
   money_init_test::construct_cents<T>("1", 0, true, "1,00 PLN", "PLN", false);
   money_init_test::construct_cents<T>("0", 99.9, false, "1,00 GBP", "GBP", false);
   money_init_test::construct_cents<T>("-0", 0, true, "0,00 CHF", "CHF", false);
   money_init_test::construct_cents<T>("0", 0, true, "0,00 JPY", "JPY", false);
   money_init_test::construct<T>("0.99999", false, "1,00 CAD", "CAD", false);
   money_init_test::construct<T>("-0", true, "0,00 JPY", "JPY", false);
   money_init_test::construct<T>("0.5", true, "0,50 JPY", "JPY", false);
   money_init_test::construct<T>("0.50", true, "0,50 JPY", "JPY", false);
   money_init_test::construct<T>("0.500", true, "0,50 JPY", "JPY", false);
   money_init_test::construct_cents<T>("1", 2.5, false, "1,03 GBP", "GBP", false);
   money_init_test::construct_cents<T>("0", 2.4, false, "0,02 GBP", "GBP", false);
   cerr << "\nEND of money_currency_test::" << __func__ << '\n';
}

template <typename T>
void incorrect_construct() {
   cerr << "\nstart of money_currency_test::" << __func__ << '\n';
   //money_init_test::construct_incorrect<Money<T>>(Constructor<T, Money>(), "20.8", 5.0L);
   //money_init_test::construct_incorrect<Money<T>>(Constructor<T, Money>(), "20.8", "VVV");
   money_init_test::construct_incorrect<Money<T>>(Constructor<T, Money>(), "20.8", "VVV");
   //////money_init_test::construct_incorrect<Money<T>>(Constructor<T, Money>(), "inf");
   //money_init_test::construct_incorrect<Money<T>, Money<T> (Constructor<T, Money>::*) (const string &, const long double, const string &) > (Constructor<T, Money>(), string("0"), 99.0L, string("WWW"));
   //money_init_test::construct_incorrect<Money<T>>(Constructor<T, Money>(), "20.8", "VVV");
   cerr << "\nEND of money_currency_test::" << __func__ << '\n';
}
   
template <typename T>
inline void perform() {
   //Currency_test<T>::construct();
   construct<T>();
   incorrect_construct<T>();
}

void perform() {
   perform<char>();
   perform<Integer>();
   perform<int_fast8_t>();
   perform<short>();
   perform<unsigned short>();
   perform<int>();
   perform<unsigned>();
   perform<long>();
   perform<unsigned long>();
   perform<long long>();
   perform<unsigned long long>();
   perform<float>();
   perform<double>();
   perform<long double>();
   
   cerr << '\n' << __func__ << ": END of ALL TESTS\n";
}

}
