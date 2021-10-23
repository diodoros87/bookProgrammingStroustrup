#ifndef MONEY_HPP
#define MONEY_HPP

#include <iostream>
#include <string>
#include <type_traits>

using std::ostream;
using std::istream;
using std::string;
using std::is_integral;
using std::is_floating_point;

namespace money {
/*
template <typename T,
              std::enable_if_t<
              is_floating_point<T>::value || is_integral<T>::value, bool> = true>*/
template <typename T>
class Money {
static_assert((is_integral<T>::value || is_floating_point<T>::value) && "Number required.");
public:
   static constexpr short CENTS_PER_DOLLAR = 100;
   static const string TYPE_NAME;
   
   static const string LONG_DOUBLE_NAME;
   static const string DOUBLE_NAME;
   static const string FLOAT_NAME;
   
   static const string CHAR_NAME;
   static const string SHORT_NAME;
   static const string UNSIGNED_SHORT_NAME;
   static const string INT_NAME;
   static const string UNSIGNED_INT_NAME;
   static const string LONG_NAME;
   static const string UNSIGNED_LONG_NAME;
   static const string LONG_LONG_NAME;
   static const string UNSIGNED_LONG_LONG_NAME;
   
   static const string INTEGER_OBJECT_NAME;
   
   static T round(double x) {
      return static_cast<T>(x + 0.5);
   }
   
   Money(const string & dollars, const double cents);
   /*
   void set(const string & dollars, const double cents = 0) { reduce(x, denominator); numerator = x; }
   void set_denominator(long x) { reduce(numerator, x); denominator = x; }
   */
   Money& operator=(const Money& other);
   
   Money operator-() const { return Money(-amount_in_cents); }  // unsigned ???
   Money operator+() const { return *this; }
   
   Money operator+(const Money& other) const;
   Money operator-(const Money& other) const { return operator+(-other); }
   Money operator*(const T n) const;
   Money operator/(const T n) const;
   
   bool operator==(const Money& other) const { return amount_in_cents == other.amount_in_cents; }
   bool operator!=(const Money& other) const { return !(*this == other); } ;
   
   bool operator>(const Money& other) const ;
   bool operator<=(const Money& other) const { return !operator>(other); };
   bool operator<(const Money& other) const ;
   bool operator>=(const Money& other) const { return !operator<(other); };
   
   T get_dollars() const { return amount_in_cents / CENTS_PER_DOLLAR; }
   T get_cents() const { return amount_in_cents % CENTS_PER_DOLLAR; }
   
   template <std::enable_if_t<is_integral<T>::value, bool> = true>
   T get_cents() const() { return amount_in_cents % CENTS_PER_DOLLAR; }
   
   template <typename T,
              std::enable_if_t<is_floating_point<T>::value, bool> = true>
   T get_cents() const() { return remainder(amount_in_cents, CENTS_PER_DOLLAR); }
   
   static T get_amount(const string & STR);
   
   //operator double() const { return static_cast<double>(numerator) / denominator; }
private:
   
   T amount_in_cents {0};
};

template <typename T>
inline std::ostream& operator<<(ostream& os, const Money<T>& money) {
   //return os << money.get_dollars() << std::use_facet<std::moneypunct<char>>(std::locale::classic()).decimal_point() 
   //            << money.get_cents() << " \n";
   return os << money.get_dollars() << "." 
               << money.get_cents() << " \n";
   //return os << std::showbase << std::put_money(money.amount_in_cents);
}

}

#include "money.cpp"

#endif
