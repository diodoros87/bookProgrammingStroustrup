#include "integers_extremums_tests.hpp"
#include "money_currency_test.hpp"
#include "money_init_test.hpp"
#include "money_utility_test.hpp"

#include <functional>

using namespace std;
using namespace integer_space;
using namespace money;
using namespace money_init_test;

template <typename T> 
extern
Money<T> construct_cents(const string & DOLLARS, const long double CENTS, bool creating, const string & expected/* = ""*/, const string & currency/* = "PLN"*/, const bool NOT_assert_currency/* = true*/);

template <typename T> 
extern
Money<T> construct(const string & DOLLARS, bool creating, const string & expected/* = ""*/, const string & currency/* = "PLN"*/, const bool NOT_assert_currency/* = true*/);

namespace money_currency_test {
   
template <typename T> 
void construct() {
   cerr << __func__ << '\n';
   construct_cents<T>("0", 99, true, "0,99 USD", "USD", false);
   construct_cents<T>("1", 1, true, "1,01 EUR", "EUR", false);
   construct_cents<T>("1", 0, true, "1,00 PLN", "PLN", false);
   construct_cents<T>("0", 99.9, false, "1,00 GBP", "GBP", false);
   construct_cents<T>("-0", 0, true, "0,00 CHF", "CHF", false);
   construct_cents<T>("0", 0, true, "0,00 JPY", "JPY", false);
   construct<T>("0.99999", false, "1,00 CAD", "CAD", false);
   construct<T>("-0", true, "0,00 JPY", "JPY", false);
   construct<T>("0.5", true, "0,50 JPY", "JPY", false);
   construct<T>("0.50", true, "0,50 JPY", "JPY", false);
   construct<T>("0.500", true, "0,50 JPY", "JPY", false);
   construct_cents<T>("1", 2.5, false, "1,03 GBP", "GBP", false);
   construct_cents<T>("0", 2.4, false, "0,02 GBP", "GBP", false);
}

template <typename T> 
void incorrect_construct() {
   cerr << __func__ << '\n';
   construct_incorrect<Money<T>>(Constructor<T, Money>(), "20", 5.0L, "WWW");
   construct_incorrect<Money<T>>(Constructor<T, Money>(), "20.8", 5, "AAA");
}
   
template <typename T>
inline void perform() {
   construct<T>();
   incorrect_construct<T>();
}

void perform() {
   perform<char, Money>();
   perform<Integer, Money>();
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
