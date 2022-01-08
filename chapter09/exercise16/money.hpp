#ifndef MONEY_HPP
#define MONEY_HPP

#include <iostream>
#include <string>
#include <type_traits>
#include <iomanip>

using std::ostream;
using std::istream;
using std::string;
using std::is_integral;
using std::is_floating_point;
using std::enable_if_t;
using std::is_same;
using std::ostringstream;
using std::setprecision;
using std::fixed;
using std::setw;
using std::setfill;
using std::stringstream;
using std::istringstream;
using std::is_convertible;
using std::signbit;

using integer_space::Integer;

//extern "C" typedef void * callback(const char * DOLLARS, const double CENTS);

namespace money {
constexpr int_fast8_t CENTS_PER_DOLLAR = 100;
constexpr int_fast8_t INCORRECT_CENTS = -112;

//#if defined(__clang__)
/* template<typename Smaller>
inline bool is_overflow(const Integer & x) {
   return Integer::is_overflow<Smaller>(x);
   //return x < numeric_limits<Smaller>::lowest() || x > numeric_limits<Smaller>::max();
} */
//#elif defined(__GNUG__)
template<typename Smaller, typename Greater
#if defined(__clang__)
   , enable_if_t<(is_floating_point<Greater>::value || is_integral<Greater>::value), bool> = true
#endif
>
inline bool is_overflow(const Greater & x) {
//    static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) 
// #if defined(__clang__)
//    && ! is_same<Greater, Integer>::value
// #endif
//    && "Number required.");
   return x < numeric_limits<Smaller>::lowest() || x > numeric_limits<Smaller>::max();
}
//#endif

template<typename T>
constexpr bool is_resetting_stream() {
   return is_convertible<T, stringstream>::value || is_convertible<T, istringstream>::value
               || is_convertible<T, ostringstream>::value;
}

template<typename T>
void reset(T & stream) {
   static_assert(is_resetting_stream<T>());
   const static stringstream initial;
   stream.str("");
   stream.clear();
   stream.copyfmt(initial);
}
   
template <typename T>
class Money {
static_assert((numeric_limits<T>::is_integer || is_floating_point<T>::value) && "Number required.");
public:
   static const string TYPE_NAME;
   static const T TYPE_DEFAULT_OBJECT;
   
   static inline T round(long double x) {
      return static_cast<T>(floor(x + 0.5));
   }
   
   Money(const string & dollars, const long double cents);   
   Money(const string & dollars);            // constructors allow rounding of cents 
   // create methods disallow rounding of cents and accept only cents without fraction
   static Money create(const string & dollars, const long double cents);
   static Money create(const string & dollars);
   
   //static callback fp;
   
   Money& operator=(const Money& other) { 
      if (this != &other)
         amount_in_cents = other.amount_in_cents;
      return *this;
   };
   
   Money operator-() const { return Money(-amount_in_cents); }  // unsigned ???
   Money operator+() const { return *this; }
   
   Money operator+(const Money& other) const { return Money(-amount_in_cents); }
   Money operator-(const Money& other) const { return operator+(-other); }
   template<typename Greater>
   Money operator*(const T & n) const;
   Money operator*(const T & n) const;
   //Money operator/(const T & n) const;
   
   bool operator==(const Money& other) const { return amount_in_cents == other.amount_in_cents; }
   bool operator!=(const Money& other) const { return !(*this == other); } ;
   
   bool operator>(const Money& other) const { return amount_in_cents > other.amount_in_cents; }
   bool operator<=(const Money& other) const { return !operator>(other); };
   bool operator<(const Money& other) const { return amount_in_cents < other.amount_in_cents; }
   bool operator>=(const Money& other) const { return !operator<(other); };
   
   template <typename Type, enable_if_t<numeric_limits<Type>::is_integer, bool> = true>
   Type get_dollars(Type &) const { 
#if defined(__clang__)
      return Integer::create_Integer(amount_in_cents) / Integer::create_Integer(CENTS_PER_DOLLAR); 
#elif defined(__GNUG__)
      return amount_in_cents / CENTS_PER_DOLLAR;
#endif      
   }
   
   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_dollars(Type &) const { return trunc(amount_in_cents / CENTS_PER_DOLLAR); }

   template <typename Type, enable_if_t<numeric_limits<Type>::is_integer, bool> = true>
   Type get_cents(Type &) const { 
#if defined(__clang__)
      return Integer::create_Integer(amount_in_cents) % Integer::create_Integer(CENTS_PER_DOLLAR); 
#elif defined(__GNUG__)
      return amount_in_cents % CENTS_PER_DOLLAR;
#endif 
   }
   
   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_cents(Type &) const { return trunc(fmod(amount_in_cents, CENTS_PER_DOLLAR)); }
   
   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_floating_amount() const { return amount_in_cents / CENTS_PER_DOLLAR; }
      
   operator string() const;
   
   T get_amount_in_cents() const { return amount_in_cents; }

   static T get_amount(const string & STR);
   
   Money operator*(const Money & ) = delete;
   Money operator*(Money) = delete;
   Money operator*(Money&&) = delete;
   Money operator/(const Money & ) = delete;
   Money operator/(Money) = delete;
   Money operator/(Money&&) = delete;
   Money operator%(const Money & ) = delete;
   Money operator%(Money) = delete;
   Money operator%(Money&&) = delete;

private:
   template<typename Greater>
   T calculate(const T & dollars, const long double cents = INCORRECT_CENTS) const;
   T calculate(const T & dollars, const long double cents = INCORRECT_CENTS) const;

   T calculate_amount_in_cents(const string & dollars);
   
   T amount_in_cents { };
};

#ifdef DEBUG_OSTREAM
   template <class Number, enable_if_t<numeric_limits<Number>::is_integer || is_floating_point<Number>::value, bool> = true>
   inline ostringstream& start_settings(ostringstream * os, const Money<Number>& money) {
      validate_pointer(os);
      if (signbit(money.get_amount_in_cents()))
         *os << '-';
      os->fill('0');
      return *os;
   }

   template <class Number, template<typename> class Money_Template, enable_if_t<
                     is_same<Number, char>::value || is_same<Number, int_fast8_t>::value, bool> = true>
   ostringstream& operator<<(ostringstream * os, const Money_Template<Number>& money) {
      validate_pointer(os);
      int dollars = static_cast<int>(money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT));
      int cents = static_cast<int>(money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT));
      start_settings(os, money);
      *os << abs(dollars) << ",";
      *os << setw(2) << abs(cents);
      return *os;
   }

   template <class Number, template<typename> class Money_Template, enable_if_t<is_integral<Number>::value &&
                        ! is_same<Number, char>::value && ! is_same<Number, int_fast8_t>::value, bool> = true>
   ostringstream& operator<<(ostringstream * os, const Money_Template<Number>& money) {
      validate_pointer(os);
      Number dollars = money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
      Number cents = money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
      start_settings(os, money);
      *os << (dollars < 0 ? -dollars : dollars) << ",";
      *os << setw(2) << (cents < 0 ? -cents : cents);
      return *os;
   }

   ostringstream& operator<<(ostringstream * os, const Money<Integer>& money) {
      validate_pointer(os);
      Integer dollars = money.get_dollars(Money<Integer>::TYPE_DEFAULT_OBJECT);
      Integer cents = money.get_cents(Money<Integer>::TYPE_DEFAULT_OBJECT);
      start_settings(os, money);
      *os << dollars.string_without_signum() << ",";
      *os << setw(2) << cents.string_without_signum();
      return *os;
   }

   template <class Number, template<typename> class Money_Template, enable_if_t<is_floating_point<Number>::value, bool> = true>
   ostringstream& operator<<(ostringstream * os, const Money_Template<Number>& money) {
      validate_pointer(os);
      std::streamsize os_precision = os->precision();
      Number dollars = money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
      Number cents = money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
      start_settings(os, money);
      *os << fixed << setprecision(0) << setw(0) << (signbit(dollars) ? -dollars : dollars) << ",";
      *os << setw(2) << (signbit(cents) ? -cents : cents);
      os->precision(os_precision);
      return *os;
   }


template <class Number, enable_if_t<is_floating_point<Number>::value || numeric_limits<Number>::is_integer, bool> = true>
ostream& operator<<(ostream& os, const Money<Number>& money) {
   ostringstream ostrs;
   string output = operator<<(&ostrs, money).str();
   return os << output;
}
#else

template <class Number, enable_if_t<numeric_limits<Number>::is_integer || is_floating_point<Number>::value, bool> = true>
inline ostream& start_settings(ostream& os, const Money<Number>& money) {
   if (signbit(money.get_amount_in_cents()))
      os << '-';
   os.fill('0');
   return os;
}

template <class Number, template<typename> class Money_Template, enable_if_t<
                  is_same<Number, char>::value || is_same<Number, int_fast8_t>::value, bool> = true>
ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   int dollars = static_cast<int>(money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT));
   int cents = static_cast<int>(money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT));
   start_settings(os, money);
   os << abs(dollars) << ",";
   os << setw(2) << abs(cents);
   return os;
}

template <class Number, template<typename> class Money_Template, enable_if_t<is_integral<Number>::value &&
                       ! is_same<Number, char>::value && ! is_same<Number, int_fast8_t>::value, bool> = true>
ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   Number dollars = money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
   Number cents = money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
   start_settings(os, money);
   os << (dollars < 0 ? -dollars : dollars) << ",";
   os << setw(2) << (cents < 0 ? -cents : cents);
   return os;
}

ostream& operator<<(ostream& os, const Money<Integer>& money) {
   Integer dollars = money.get_dollars(Money<Integer>::TYPE_DEFAULT_OBJECT);
   Integer cents = money.get_cents(Money<Integer>::TYPE_DEFAULT_OBJECT);
   start_settings(os, money);
   os << dollars.string_without_signum() << ",";
   os << setw(2) << cents.string_without_signum();
   return os;
}

template <class Number, template<typename> class Money_Template, enable_if_t<is_floating_point<Number>::value, bool> = true>
ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   std::streamsize os_precision = os.precision();
   Number dollars = money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
   Number cents = money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
   start_settings(os, money);
   os << fixed << setprecision(0) << setw(0) << (signbit(dollars) ? -dollars : dollars) << ",";
   os << setw(2) << (signbit(cents) ? -cents : cents);
   os.precision(os_precision);
   return os;
}
#endif

}

#include "money.cpp"

#endif
