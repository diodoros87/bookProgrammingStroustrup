#ifndef MONEY_HPP
#define MONEY_HPP

#include <iostream>
#include <string>
#include <type_traits>
#include <iomanip>
#include <regex>

#include <cmath>

#include "integer.hpp"

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
using std::regex;
using std::trunc;
using std::floor;

using integer_space::Integer;

namespace money {
constexpr int_fast8_t CENTS_PER_DOLLAR = 100;
constexpr int_fast8_t INCORRECT_CENTS = -112;

const regex E_FLOAT_POINT_REGEX = regex { R"(^[+-]?(\d+([.]\d*)?([eE][+-]?\d+)?|[.]\d+([eE][+-]?\d+)?)$)" } ;
const regex FLOAT_POINT_REGEX   = regex { R"(^[+-]?(\d+([.]\d*)?|[.]\d+)$)" } ;
const regex INTEGER_REGEX       = regex { R"(^[+-]?(\d+)$)" } ;
const regex MINUS_ZERO_REGEX    = regex { R"(^-0([.][0-9]+)?$)" } ;

extern const Integer CENTS_PER_DOLLAR_INTEGER;

template<typename Greater>
bool is_overflow_for_Integer(const Greater & x) {
   try {
      Integer::create_Integer(x);
      return false;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return true;
   }
} 

template<typename Smaller, typename Greater
#if defined(__clang__)
   , enable_if_t<(is_floating_point<Greater>::value || is_integral<Greater>::value), bool> = true
#endif
>
inline bool is_overflow(const Greater & x) {
   return x < numeric_limits<Smaller>::lowest() || x > numeric_limits<Smaller>::max();
}

template <typename Number, enable_if_t<
              is_floating_point<Number>::value || is_integral<Number>::value, bool> = true>
inline bool equal_integer(const Number x) {
   cerr << __func__ << " x = " << std::to_string(x) << '\n';
   cerr << " trunc(x) = " << std::to_string(trunc(x)) << '\n';
   cerr << std::boolalpha << " isfinite(x) = " << isfinite(x) << '\n';
   cerr << std::boolalpha << " x == trunc(x) = " << (x == trunc(x)) << '\n';
   return isfinite(x) && x == trunc(x);
}

struct bad_from_string : public std::bad_cast {
   const char * what() const noexcept override {
      return "Bad cast from string";
   }
};

template <typename Type>
Type from_string(const string & STR, bool eof_checking) {
   istringstream stream { STR };
   Type result;
   stream >> result;
   if (!stream)
      throw bad_from_string {};
   if (eof_checking && ! stream.eof()) {
      cerr << " Type = " << typeid(Type).name() << '\n';
      throw invalid_argument(__func__ + string(" entered string '" + STR + "'is not accepted format "));
   }
   return result;
}

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
   
   /*
   template<typename U, enable_if_t<numeric_limits<T>::is_integer, bool> = true>
   static inline U round(const string & x) {
      return static_cast<T>(floor(x + 0.5));
   }*/
   
   Money(const string & dollars, const long double cents);   
   Money(const string & dollars);            // constructors allow rounding of cents 
   // create methods disallow rounding of cents and accept only cents without fraction
   static Money create(const string & dollars, const long double cents);
   //static Money create(const string & dollars);
   
   template<typename U = T, enable_if_t<is_floating_point<U>::value, bool>  = true>
   static Money<U> create(const string & dollars) {
      const long double amount = from_string<long double>(dollars, true) * CENTS_PER_DOLLAR;
      if (! equal_integer<long double>(amount))
         throw invalid_argument("Not exact value dollars = " + to_string(amount));
      
      Money<U> money = Money<U>(dollars);
      return money;
   }
   
   template<typename U = T, enable_if_t<numeric_limits<U>::is_integer, bool>  = true>
   static Money<U> create(const string & dollars) {
      const size_t dot_position = dollars.find('.');
      if (dot_position != string::npos) {
         static const regex EXACT = regex { R"(^[+-]?(\d+).\d[\d]?[0]*$)" } ;
         if (! regex_match(dollars, EXACT))
            throw invalid_argument(string(__func__) +  " Regex: entered string '"
                  + dollars + "' is not exact format ");
      }
      Money<U> money = Money<U>(dollars);
      return money;
   }
   
   ~ Money() = default;
   Money(const Money &) = default;
   Money(Money &&) = default;
   Money & operator=(const Money &) = default;
   Money & operator=(Money &&) = default;
   /*
   Money& operator=(const Money& other) { 
      if (this != &other)
         amount_in_cents = other.amount_in_cents;
      return *this;
   };
   */
   Money& operator-() const { return Money(-amount_in_cents); }  // unsigned ???
   Money& operator+() const { return *this; }
   
   //Money operator+(const Money& other) const;
   Money operator-(const Money& other) const { return operator+(-other); }
   
   //Money<Integer> operator+(const Money<Integer>& other) const;
   Money<Integer>& operator+(const Money<Integer>& other);
   template<typename U = T, enable_if_t<is_integral<U>::value, bool>  = true>
   Money<U>& operator+(const Money<U>& other) const {
      //static_assert((numeric_limits<U>::is_integer);
      return operator+<Integer, U>(other);
   }
   
   template<typename U = T, enable_if_t<is_floating_point<U>::value, bool>  = true>
   Money<U>& operator+(const Money<U>& other) const {
      //static_assert((numeric_limits<U>::is_integer);
      //                     ! is_same<Greater, Integer>::value );
      return operator+<long double, U>(other);
   }
   
   
   template<typename Greater>
   Money operator*(const T & n) const;
   Money operator*(const T & n) const;
   Money operator/(const T & n) const;
   
   bool operator==(const Money& other) const { return amount_in_cents == other.amount_in_cents; }
   bool operator!=(const Money& other) const { return !(*this == other); } ;
   
   bool operator>(const Money& other) const { return amount_in_cents > other.amount_in_cents; }
   bool operator<=(const Money& other) const { return !operator>(other); };
   bool operator<(const Money& other) const { return amount_in_cents < other.amount_in_cents; }
   bool operator>=(const Money& other) const { return !operator<(other); };
   
   template <typename Type, enable_if_t<is_integral<Type>::value, bool> = true>
   Type get_dollars(Type &) const { 
      return amount_in_cents / CENTS_PER_DOLLAR;   
   }
   
   //template <typename Type, enable_if_t<is_integral<Type>::value, bool> = true>
   Integer get_dollars(const Integer &) const { 
      return amount_in_cents / CENTS_PER_DOLLAR_INTEGER;   
   }

   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_dollars(Type &) const { return trunc(amount_in_cents / CENTS_PER_DOLLAR); }
   
   template <typename Type, enable_if_t<is_integral<Type>::value, bool> = true>
   Type get_cents(Type &) const { 
      return amount_in_cents % CENTS_PER_DOLLAR;
   }
   
   Integer get_cents(const Integer &) const { 
      return amount_in_cents % CENTS_PER_DOLLAR_INTEGER;
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
   
   friend Money<Integer> operator+(const Money<Integer>& a, const Money<Integer>& b);
   
   template<typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            is_integral<Smaller>::value, bool> >
   friend Money<Smaller> operator+(const Money<Smaller>& a, const Money<Smaller>& b);
   
   template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool> >
   friend Money<Smaller> operator+(const Money<Smaller>& a, const Money<Smaller>& b);

private:
   template <typename Greater>
   T calculate(const T & dollars, const long double cents = INCORRECT_CENTS   ) const;
   
   T calculate_by_Integer(const T & dollars, const long double cents = INCORRECT_CENTS) const;
   
   template<typename Greater>
   Money operator+(const Money& other) const;
   
   template<typename Greater, typename U = T, enable_if_t<is_floating_point<U>::value ||
            (is_integral<U>::value && ! is_same<Greater, Integer>::value), bool>  = true>
   Money operator+(const Money<U>& other) const {
      //static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) &&
      //                     ! is_same<Greater, Integer>::value );
      Greater sum = Greater(this->amount_in_cents) + Greater(other->amount_in_cents);
      sum = Money<Greater>::round(sum);
      cerr << __func__ << " sum = " << sum << '\n';
      if (is_overflow<T, Greater>(sum))
         throw out_of_range(__func__ + "amount = " + std::to_string(sum) + " is overflow for type " + TYPE_NAME);
      const string dollars = std::to_string(sum);
      Money<T> result = Money<T>(dollars);
      cerr << __func__ << " result = " << result << '\n';
      return result;
   }
   
   template<typename Greater, typename U = T, enable_if_t<is_integral<U>::value && is_same<Greater, Integer>::value, bool>  = true>
   Money operator+(const Money<U>& other) const {
      //static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) &&
      //                     ! is_same<Greater, Integer>::value );
      Integer sum = Integer::create_Integer(this->amount_in_cents) + Integer::create_Integer(other.amount_in_cents);
      //sum = Money<Greater>::round(sum);
      cerr << __func__ << " sum = " << sum << '\n';
      if (Integer::is_overflow<T>(sum))
         throw out_of_range(string(__func__) + " amount = " + std::to_string(sum) + " is overflow for type " + TYPE_NAME);
      const Integer dollars = sum / CENTS_PER_DOLLAR_INTEGER;
      const Integer cents = sum % CENTS_PER_DOLLAR_INTEGER;
      //const Money<U> addition = static_cast<U>(sum);
      //const U dollars = addition.get_dollars(Money<U>::TYPE_DEFAULT_OBJECT);
      //const U cents = addition.get_cents(Money<U>::TYPE_DEFAULT_OBJECT);
      Money<T> result = Money<T>(std::to_string(dollars), static_cast<long double>(cents));
      cerr << __func__ << " result = " << result << '\n';
      return result;
   }
   
   //T calculate_amount_in_cents(const string & dollars);

   template<typename U = T, enable_if_t<is_same<U, Integer>::value, bool> = true>
   Integer calculate_amount_in_cents(const string & dollars) const {
      Integer result;
      size_t dot_position = dollars.find('.');
      cerr << __func__ << " dot_position = " << dot_position <<  '\n';
      string dollars_string = dollars.substr(0, dot_position);
      if (regex_match(dollars, MINUS_ZERO_REGEX)) {
         cerr << __func__ << " dollars_string = " << dollars_string <<  '\n';
         dollars_string = dollars_string.substr(1);
         cerr << __func__ << " dollars_string = " << dollars_string <<  '\n';
      }
      Integer dollars_part = Money<T>::get_amount(dollars_string);
      
      if (dot_position == string::npos) {
         //string dollars_string = dollars.substr(0, dot_position);
         //T dollars_part = get_amount(dollars_string);
         cerr << __func__ << " dot_position = string::npos " << dot_position <<  '\n';
         //result = calculate<long double>(dollars_part);
         result = calculate_by_Integer(dollars_part);
         cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents <<  '\n';
      }
      else {
         string cents_string = dollars.substr(dot_position + 1);
         if (cents_string.size() > 2)
            cents_string.insert(2, ".");
         cerr << __func__ << " cents_string = " << cents_string <<  '\n';
         long double cents_part = stold(cents_string);
         if (cents_string.size() == 1)
            cents_part *= 10;
         //result = calculate<long double>(dollars_part, cents_part);
         result = calculate_by_Integer(dollars_part, cents_part);
         if (dollars[0] == '-' && result > Integer::ZERO && result <= Integer::create_Integer(100))
            result = -result;
      }
      return result;
   }
   
   template<typename U = T, enable_if_t<is_floating_point<U>::value, bool>  = true>
   T calculate_amount_in_cents(const string & dollars) const {
      U dollars_part = Money<T>::get_amount(dollars);
      U result  = calculate<long double>(dollars_part);
      return result;
   }
   
   template<typename U = T, enable_if_t<is_integral<U>::value, bool>  = true>
   U calculate_amount_in_cents(const string & dollars) const {
      U result;
      size_t dot_position = dollars.find('.');
      cerr << __func__ << " dot_position = " << dot_position <<  '\n';
      string dollars_string = dollars.substr(0, dot_position);
      U dollars_part = Money<T>::get_amount(dollars_string);
      
      if (dot_position == string::npos) {
         cerr << __func__ << " dot_position = string::npos " << dot_position <<  '\n';
         result = calculate_by_Integer(dollars_part);
         cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents <<  '\n';
      }
      else {
         string cents_string = dollars.substr(dot_position + 1);
         if (cents_string.size() > 2)
            cents_string.insert(2, ".");
         cerr << __func__ << " cents_string = " << cents_string <<  '\n';
         long double cents_part = stold(cents_string);
         if (cents_string.size() == 1)
            cents_part *= 10;
         
         result = calculate_by_Integer(dollars_part, cents_part);
         if (dollars[0] == '-' && result > 0 && result <= 100)
            result = -result;
      }
      return result;
   }

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
   
   template <class Number, template<typename> class Money_Template, enable_if_t<is_same<Number, Integer>::value, bool> = true>
   ostringstream& operator<<(ostringstream * os, const Money_Template<Number>& money) {
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

Money<Integer> operator+(const Money<Integer>& a, const Money<Integer>& b);
/*
template<typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            is_integral<Smaller>::value, bool> = true >
Money<Smaller> operator+(const Money<Smaller>& a, const Money<Smaller>& b);
*/
template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool> = true >
Money<Smaller> operator+(const Money<Smaller>& a, const Money<Smaller>& b);

template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && is_same<Greater, Integer>::value), bool> = true >
Money<Smaller> operator+(const Money<Smaller>& a, const Money<Smaller>& b);

template<typename Smaller, enable_if_t<is_integral<Smaller>::value, bool>  = true>
Money<Smaller>& operator+(const Money<Smaller>& a, const Money<Smaller>& b) {
   //static_assert((numeric_limits<U>::is_integer);
   return operator+<Integer, Smaller>(a, b);
}

template<typename Smaller, enable_if_t<is_floating_point<Smaller>::value, bool>  = true>
Money<Smaller>& operator+(const Money<Smaller>& a, const Money<Smaller>& b) {
   //static_assert((numeric_limits<U>::is_integer);
   //                     ! is_same<Greater, Integer>::value );
   return operator+<long double, Smaller>(a, b);
}

template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool> /*= true */>
Money<Smaller> operator+(const Money<Smaller>& a, const Money<Smaller>& b) {
   //static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) &&
   //                     ! is_same<Greater, Integer>::value );
   static const string TYPE_NAME = typeid(Smaller).name();
   Greater sum = Greater(a.amount_in_cents) + Greater(b.amount_in_cents);
   sum = Money<Greater>::round(sum);
   cerr << __func__ << " sum = " << sum << '\n';
   if (is_overflow<Smaller, Greater>(sum))
      throw out_of_range(__func__ + "amount = " + std::to_string(sum) + " is overflow for type " + TYPE_NAME);
   const string dollars = std::to_string(sum);
   Money<Smaller> result = Money<Smaller>(dollars);
   cerr << __func__ << " result = " << result << '\n';
   return result;
}

template<typename Greater, typename Smaller, enable_if_t<is_integral<Smaller>::value && is_same<Greater, Integer>::value, bool>  = true>
Money operator+(const Money<Smaller>& a, const Money<Smaller>& b) {
   //static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) &&
   //                     ! is_same<Greater, Integer>::value );
   static const string TYPE_NAME = typeid(Smaller).name();
   Integer sum = Integer::create_Integer(a.amount_in_cents) + Integer::create_Integer(b.amount_in_cents);
   //sum = Money<Greater>::round(sum);
   cerr << __func__ << " sum = " << sum << '\n';
   if (Integer::is_overflow<Smaller>(sum))
      throw out_of_range(string(__func__) + " amount = " + std::to_string(sum) + " is overflow for type " + TYPE_NAME);
   const Integer dollars = sum / CENTS_PER_DOLLAR_INTEGER;
   const Integer cents = sum % CENTS_PER_DOLLAR_INTEGER;
   //const Money<U> addition = static_cast<U>(sum);
   //const U dollars = addition.get_dollars(Money<U>::TYPE_DEFAULT_OBJECT);
   //const U cents = addition.get_cents(Money<U>::TYPE_DEFAULT_OBJECT);
   Money<Smaller> result = Money<Smaller>(std::to_string(dollars), static_cast<long double>(cents));
   cerr << __func__ << " result = " << result << '\n';
   return result;
}

}



#include "money.cpp"

#endif
