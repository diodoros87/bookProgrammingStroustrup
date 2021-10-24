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
   
template <typename T>
class Money {
static_assert((numeric_limits<T>::is_integer || is_floating_point<T>::value) && "Number required.");
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
   Money(const string & dollars);
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
   
   template <typename Type, enable_if_t<numeric_limits<Type>::is_integer, bool> = true>
   Type get_dollars(Type) const { return amount_in_cents / CENTS_PER_DOLLAR; }
   
   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_dollars(Type) const { return trunc(amount_in_cents / CENTS_PER_DOLLAR); }
   //- get_cents(Type(0)
   template <typename Type, enable_if_t<numeric_limits<Type>::is_integer, bool> = true>
   Type get_cents(Type) const { return amount_in_cents % CENTS_PER_DOLLAR; }
   
   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_cents(Type) const { return trunc(fmod(amount_in_cents, CENTS_PER_DOLLAR)); }
   
   static T get_amount(const string & STR);
   
   Money operator*(const Money & ) = delete;
   //Money operator*(const Money) = delete;
   Money operator*(Money) = delete;
   Money operator*(Money&&) = delete;
   Money operator/(const Money & ) = delete;
   Money operator/(Money) = delete;
   Money operator/(Money&&) = delete;
   Money operator%(const Money & ) = delete;
   Money operator%(Money) = delete;
   Money operator%(Money&&) = delete;
   /*
   template <typename Number, enable_if_t<is_floating_point<Number>::value, bool> = true>
   static Number get_amount(const string & STR);
   
   template <typename Number, enable_if_t<is_floating_point<Number>::value, bool> = true>
   static T get_amount(const string & STR) {
      if (TYPE_NAME == LONG_DOUBLE_NAME)   
         return stold(STR);
      else if (TYPE_NAME == DOUBLE_NAME)  
         return stod(STR);
      else if (TYPE_NAME == FLOAT_NAME)   
         return stof(STR);
   }
   
   template <class Number, template<typename> class Money_Template, enable_if_t<numeric_limits<Number>::is_integer, bool> = true>
   template <typename Number, enable_if_t<numeric_limits<Number>::is_integer, bool> = true>
   static T get_amount(const string & STR) {
      if (TYPE_NAME == UNSIGNED_LONG_LONG_NAME) 
            return stoull(STR);
      else if (TYPE_NAME == LONG_LONG_NAME)   
            return stoll(STR);
      else if (TYPE_NAME == UNSIGNED_LONG_NAME) 
         return stoul(STR);
      else if (TYPE_NAME == LONG_NAME)
         return stol(STR);
      else if (TYPE_NAME == INT_NAME)   
         return stoi(STR);
      else if (TYPE_NAME == UNSIGNED_INT_NAME 
            || TYPE_NAME == UNSIGNED_SHORT_NAME) {
         T amount = stoul(STR);
         if (is_overflow<T, unsigned long>(amount))
            throw invalid_argument("amount = " + to_string(amount) + " is overflow for type " + TYPE_NAME);
         return amount;
      }
      else if (TYPE_NAME == SHORT_NAME
            || TYPE_NAME == CHAR_NAME) {
         T amount = stoi(STR);
         if (is_overflow<T, int>(amount))
            throw invalid_argument("amount = " + to_string(amount) + " is overflow for type " + TYPE_NAME);
         return amount;
      }
      else if (TYPE_NAME == INTEGER_OBJECT_NAME)  
         return Integer::parse_create(STR);
      else
         throw invalid_argument("No implementation for type " + TYPE_NAME);
   }
   
   template <typename Number, enable_if_t<numeric_limits<Number>::is_integer, bool> = true>
   static Number get_amount(const string & STR);*/
private:
   T amount_in_cents {0};
};

template <class Number, template<typename> class Money_Template, enable_if_t<numeric_limits<Number>::is_integer, bool> = true>
inline ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   return os << money.get_dollars(Number(0)) << "," 
               << money.get_cents(Number(0)) << " \n";
}

template <class Number, template<typename> class Money_Template, enable_if_t<is_floating_point<Number>::value, bool> = true>
inline ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   return os << money.get_dollars(Number(0)) << "," 
               << money.get_cents(Number(0)) << " \n";
}

}

#include "money.cpp"

#endif
