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
using std::enable_if_t;

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
   
   template <typename Type, enable_if_t<is_integral<Type>::value, bool> = true>
   Type get_dollars(Type) const { return amount_in_cents / CENTS_PER_DOLLAR; }
   //T get_dollars() const { return amount_in_cents ; }
   /*
   T get_dollars() const { return amount_in_cents ; }
   T get_cents() const { return amount_in_cents; }
   */
   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_dollars(Type) const { return amount_in_cents - get_cents(Type(0)); }
   ///T get_dollars() const { return amount_in_cents ; }
   
   template <typename Type, enable_if_t<is_integral<Type>::value, bool> = true>
   Type get_cents(Type) const { return amount_in_cents % CENTS_PER_DOLLAR; }
   ///T get_cents() const { return amount_in_cents ; }
   
   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_cents(Type) const { return fmod(amount_in_cents, CENTS_PER_DOLLAR); }
   ///T get_cents() const { return amount_in_cents; }
   
   static T get_amount(const string & STR);
   /*
   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   ostream& operator<<(ostream& os) {
      return os << get_dollars() << "." 
                  << get_cents() << " \n";
   }
   */
private:
   T amount_in_cents {0};
};
/*
template <typename Number, template<typename> class Money, enable_if_t<is_integral<Type>::value, bool> = true>
inline ostream& operator<<(ostream& os, Money<Type>& money) {
   return os << static_cast<Type>(money.get_dollars()) << "." 
               << static_cast<Type>(money.get_cents()) << " \n";
}

template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   ostream& operator<<(ostream& os) {
      return os << get_dollars<Type>() << "." << get_cents<Type>() << " \n";
      //return os << std::showbase << std::put_money(money.amount_in_cents);
   }

template <typename Type>
inline ostream& operator<<(ostream& os, const Money<Type>& money) {
   return os << money.get_dollars<Type>() << "." 
               << money.get_cents() << " \n";
}


template <typename T>
inline std::ostream& operator<<(ostream& os, const Money<T>& money) {
   //return os << money.get_dollars() << std::use_facet<std::moneypunct<char>>(std::locale::classic()).decimal_point() 
   //            << money.get_cents() << " \n";
   return os << money.get_dollars() << "." 
               << money.get_cents() << " \n";
   //return os << std::showbase << std::put_money(money.amount_in_cents);
}

template <typename Number, template<typename> class Money_Template>
inline ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   return os << money.get_dollars() << "." 
               << money.get_cents() << " \n";
}
*/

template <class Number, template<typename> class Money_Template, enable_if_t<is_integral<Number>::value, bool> = true>
//typename std::enable_if<is_integral<Number>::value>::type 
//enable_if_t<is_integral<Number>::value>
inline ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   return os << money.get_dollars(Number(0)) << "." 
               << money.get_cents(Number(0)) << " \n";
}

template <class Number, template<typename> class Money_Template, enable_if_t<is_floating_point<Number>::value, bool> = true>
//typename std::enable_if<is_integral<Number>::value>::type 
//enable_if_t<is_integral<Number>::value>
inline ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   return os << money.get_dollars(Number(0)) << "." 
               << money.get_cents(Number(0)) << " \n";
}
/*
template<class T, class... Args>
std::enable_if_t<std::is_constructible<T, Args&&...>::value> // Using helper type
    construct(T* p, Args&&... args) 
{
    std::cout << "constructing T with operation\n";
    ::new(detail::voidify(p)) T(static_cast<Args&&>(args)...);
}

template <typename Type, enable_if_t<is_integral<Type>::value, bool> = true>
inline ostream& operator<<(ostream& os, const Money<Type>& money) {
   //return os << money.get_dollars() << std::use_facet<std::moneypunct<char>>(std::locale::classic()).decimal_point() 
   //            << money.get_cents() << " \n";
   return os << money.get_dollars() << "." 
               << money.get_cents() << " \n";
   //return os << std::showbase << std::put_money(money.amount_in_cents);
}

template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
inline ostream& operator<<(ostream& os, const Money<Type>& money) {
   //return os << money.get_dollars() << std::use_facet<std::moneypunct<char>>(std::locale::classic()).decimal_point() 
   //            << money.get_cents() << " \n";
   return os << money.get_dollars() << "." 
               << money.get_cents() << " \n";
   //return os << std::showbase << std::put_money(money.amount_in_cents);
}
*/
}

#include "money.cpp"

#endif
