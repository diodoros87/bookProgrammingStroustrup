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

using integer_space::Integer;

namespace money {
constexpr int_fast8_t CENTS_PER_DOLLAR = 100;
constexpr int_fast8_t INCORRECT_CENTS = -99;

template<typename Smaller, typename Greater>
inline bool is_overflow(const Greater & x) {
   static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) && "Number required.");
   return x < numeric_limits<Smaller>::lowest() || x > numeric_limits<Smaller>::max();
}
   
template <typename T>
class Money {
static_assert((numeric_limits<T>::is_integer || is_floating_point<T>::value) && "Number required.");
public:
   static const string TYPE_NAME;
   static const T TYPE_DEFAULT_OBJECT;
   
   static inline T round(long double x) {
      return static_cast<T>(x + 0.5);
      //return T(x + 0.5);
   }
   
   Money(const string & dollars, const long double cents);   
   Money(const string & dollars);            // constructors allow rounding of cents 
   // create methods disallow rounding of cents and accept only cents without fraction
   static Money create(const string & dollars, const long double cents);
   static Money create(const string & dollars);
   /*
   void set(const string & dollars, const double cents = 0) { reduce(x, denominator); numerator = x; }
   void set_denominator(long x) { reduce(numerator, x); denominator = x; }
   */
   Money& operator=(const Money& other) = default;
   
   Money operator-() const { return Money(-amount_in_cents); }  // unsigned ???
   Money operator+() const { return *this; }
   
   Money operator+(const Money& other) const;
   Money operator-(const Money& other) const { return operator+(-other); }
   Money operator*(const T n) const;
   Money operator/(const T n) const;
   
   bool operator==(const Money& other) const { return amount_in_cents == other.amount_in_cents; }
   bool operator!=(const Money& other) const { return !(*this == other); } ;
   
   bool operator>(const Money& other) const { return amount_in_cents > other.amount_in_cents; }
   bool operator<=(const Money& other) const { return !operator>(other); };
   bool operator<(const Money& other) const { return amount_in_cents < other.amount_in_cents; }
   bool operator>=(const Money& other) const { return !operator<(other); };
   
   template <typename Type, enable_if_t<numeric_limits<Type>::is_integer, bool> = true>
   Type get_dollars(Type &) const { return amount_in_cents / CENTS_PER_DOLLAR; }
   
   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_dollars(Type &) const { return trunc(amount_in_cents / CENTS_PER_DOLLAR); }

   template <typename Type, enable_if_t<numeric_limits<Type>::is_integer, bool> = true>
   Type get_cents(Type &) const { return amount_in_cents % CENTS_PER_DOLLAR; }
   
   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_cents(Type &) const { return trunc(fmod(amount_in_cents, CENTS_PER_DOLLAR)); }
   
   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_floating_amount() const { return amount_in_cents / CENTS_PER_DOLLAR; }
   /*
   template <typename Type, enable_if_t<numeric_limits<Type>::is_integer, bool> = true>
   inline operator string(Type &&) {
      ostringstream stream;
      stream << money.get_dollars(Type{}) << "," << money.get_cents(Type{});
      return stream.str();
   }
   */
      
   operator string() const {
      ostringstream stream;
      T dollars = get_dollars(TYPE_DEFAULT_OBJECT);
      T cents = get_cents(TYPE_DEFAULT_OBJECT);
      if (cents < 0)
         cents = -cents;
      if (is_floating_point<T>::value) {
         stream << fixed << setprecision(0) << setw(0) << dollars << ",";
         stream.fill('0');
         stream << setw(2) << cents;
         return stream.str();
      }
      string dollars_string;
      string cents_string;
      if (is_same<T, char>::value || is_same<T, int_fast8_t>::value) {
         dollars_string = std::to_string(static_cast<int>(dollars));
         cents_string = std::to_string(static_cast<int>(cents));
      } 
      else if (is_same<T, Integer>::value) {
         stream << dollars;
         dollars_string = stream.str();
         if (dollars > 0)
            dollars_string = dollars_string.substr(1);
         stream.flush();
         stream.clear();
         stream << cents;
         cents_string = stream.str();
         if (cents != 0)
            cents_string = cents_string.substr(1);
      }
      else /* if (is_integral<T>::value) */ {
         dollars_string = std::to_string(dollars);
         cents_string = std::to_string(cents);
      }
      cents_string = cents_string.length() == 1 ? ("0" + cents_string) : cents_string;
      //cents_string = cents_string.length() == 1 ? ("0" + cents_string) : cents_string.substr(0, 2);
      //stream << fixed << setprecision(0) << dollars_string << "," << setprecision(2) << (cents < 0 ? -cents : cents);
      string out = dollars_string + "," + cents_string;
      return out;
   }

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

   T amount_in_cents { };
};

template <class Number, template<typename> class Money_Template, enable_if_t<
                  is_same<Number, char>::value || is_same<Number, int_fast8_t>::value, bool> = true>
ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   int dollars = static_cast<int>(money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT));
   int cents = static_cast<int>(money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT));
   return os << dollars << "," << (cents < 0 ? -cents : cents);
}

template <class Number, template<typename> class Money_Template, enable_if_t<is_integral<Number>::value &&
                       ! is_same<Number, char>::value && ! is_same<Number, int_fast8_t>::value, bool> = true>
ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   Number dollars = money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
   Number cents = money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
   return os << dollars << "," << (cents < 0 ? -cents : cents);
}

ostream& operator<<(ostream& os, const Money<Integer>& money) {
   Integer dollars = money.get_dollars(Money<Integer>::TYPE_DEFAULT_OBJECT);
   Integer cents = money.get_cents(Money<Integer>::TYPE_DEFAULT_OBJECT);
   os << (dollars > 0 ? dollars.string_without_signum() : dollars) << ",";
   return os << cents.string_without_signum();
}

template <class Number, template<typename> class Money_Template, enable_if_t<is_floating_point<Number>::value, bool> = true>
ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   std::streamsize os_precision = os.precision();
   Number dollars = money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
   Number cents = money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
   /*
   Number amount = get_floating_amount();
   return os << fixed << setprecision(0) << dollars << "," 
      << setprecision(0) << (cents < 0 ? -cents : cents);
   return os << fixed << setprecision(0) << dollars << "," << get_floating_amount(2) << (cents < 0 ? -cents : cents);
   */
   os << fixed << setprecision(0) << setw(0) << dollars << ",";
   os.fill('0');
   return os << setw(2) << (cents < 0 ? -cents : cents);
   //return os << setprecision(os_precision) << std::defaultfloat;
}

}

#include "money.cpp"

#endif
