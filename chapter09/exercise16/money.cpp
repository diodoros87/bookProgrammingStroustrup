#include "money.hpp"
#include "integer_parsing.hpp"

#include <cmath>
#include <limits>
#include <regex>
#include <sstream>

using std::numeric_limits;
using std::enable_if_t;
using std::isfinite;
using std::regex;
using std::istringstream;
using std::bad_cast;

namespace money {
   
template <typename T>
const string Money<T>::TYPE_NAME = typeid(T).name();

template <typename T>
const string Money<T>::LONG_DOUBLE_NAME = typeid(0.0L).name();
template <typename T>
const string Money<T>::DOUBLE_NAME = typeid(0.0).name();
template <typename T>
const string Money<T>::FLOAT_NAME = typeid(0.0f).name();
template <typename T>
const string Money<T>::CHAR_NAME = typeid('A').name();
template <typename T>
const string Money<T>::INT_FAST_8_T_NAME = typeid(((int_fast8_t)0)).name();
template <typename T>
const string Money<T>::SHORT_NAME = typeid(((short)0)).name();
template <typename T>
const string Money<T>::UNSIGNED_SHORT_NAME = typeid(((unsigned short)0)).name();
template <typename T>
const string Money<T>::INT_NAME = typeid(0).name();
template <typename T>
const string Money<T>::LONG_NAME = typeid(0L).name();
template <typename T>
const string Money<T>::LONG_LONG_NAME = typeid(numeric_limits<long long>::max()).name();
template <typename T>
const string Money<T>::UNSIGNED_INT_NAME = typeid(0u).name();
template <typename T>
const string Money<T>::UNSIGNED_LONG_NAME = typeid(0uL).name();
template <typename T>
const string Money<T>::UNSIGNED_LONG_LONG_NAME = typeid(numeric_limits<unsigned long long>::max()).name();

template <typename T>
const string Money<T>::INTEGER_OBJECT_NAME = typeid(Integer(0)).name();

template<typename T, typename U>
inline bool is_overflow(const U x) {
   static_assert(is_integral<U>::value && "Integral required.");
   return x < numeric_limits<T>::min() || x > numeric_limits<T>::max();
}

template <typename Number, enable_if_t<
              is_floating_point<Number>::value || is_integral<Number>::value, bool> = true>
inline bool equal_integer(const Number x) {
   return isfinite(x) && x == trunc(x);
}

/*
template <typename T, enable_if_t<
              is_same<T, char>::value || is_same<T, short>::value, bool> = true>>
T get_amount(const string & STR) {
   T amount = stoi(STR);
   if (is_overflow<T, int>(amount))
      throw invalid_argument("amount = " + to_string(amount) + " is overflow for type " + TYPE_NAME);
   return amount;
}

template <typename T, enable_if_t<
              is_same<T, unsigned short>::value || is_same<T, unsigned int>::value, bool> = true>>
T get_amount(const string & STR) {
   T amount = stoul(STR);
   if (is_overflow<T, unsigned long>(amount))
      throw invalid_argument("amount = " + to_string(amount) + " is overflow for type " + TYPE_NAME);
   return amount;
}

template <typename Number, enable_if_t<is_floating_point<Number>::value, bool> = true>
T Money<T>::get_amount(const string & STR) {
   if (TYPE_NAME == LONG_DOUBLE_NAME)   
      return stold(STR);
   else if (TYPE_NAME == DOUBLE_NAME)  
      return stod(STR);
   else if (TYPE_NAME == FLOAT_NAME)   
      return stof(STR);
}

inline double get_amount(const string & STR) {
   long double amount = stold(STR);
   return amount;
}

template <typename T, typename Number, enable_if_t<is_floating_point<Number>::value, bool> = true>
T Money<T>::get_amount(const string & STR) {
   if (TYPE_NAME == LONG_DOUBLE_NAME)   
      return stold(STR);
   else if (TYPE_NAME == DOUBLE_NAME)  
      return stod(STR);
   else if (TYPE_NAME == FLOAT_NAME)   
      return stof(STR);
}
*/
//template <typename T, enable_if_t<numeric_limits<T>::is_integer, bool> = true>
/*
template <typename T, typename Number, enable_if_t<numeric_limits<Number>::is_integer, bool> = true>
T Money<T>::get_amount(const string & STR) {
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
*/

template <typename T>
T Money<T>::get_amount(const string & STR) {
   //cerr << string(("amount = " + std::to_string(T(0)) + " is overflow for type " + TYPE_NAME)) << '\n';
   if (is_floating_point<T>::value ) {
      if (TYPE_NAME == LONG_DOUBLE_NAME)   
         return stold(STR);
      else if (TYPE_NAME == DOUBLE_NAME)  
         return stod(STR);
      else if (TYPE_NAME == FLOAT_NAME)   
         return stof(STR);
   } 
   else if (numeric_limits<T>::is_integer) {
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
            throw invalid_argument("amount = " + std::to_string(amount) + " is overflow for type " + TYPE_NAME);
         return amount;
      }
      else if (TYPE_NAME == SHORT_NAME
            || TYPE_NAME == CHAR_NAME
            || TYPE_NAME == INT_FAST_8_T_NAME) {
         T amount = stoi(STR);
         if (is_overflow<T, int>(amount))
            throw invalid_argument("amount = " + std::to_string(amount) + " is overflow for type " + TYPE_NAME);
         return amount;
      }
      else if (TYPE_NAME == INTEGER_OBJECT_NAME)  
         return Integer::parse_create(STR);
      else
         throw invalid_argument("No implementation for type " + TYPE_NAME);
   }
}

template <typename T>
Money<T>::Money(const string & dollars, const long double cents) {
   try {
      integer_parsing::validate_string(dollars);
   } catch(const invalid_argument& e) {
		throw invalid_argument(string(e.what()) + "\ndollars can not be floating-point ");
	}
   
   T amount = get_amount(dollars);
   cerr << " TYPE_NAME = " << TYPE_NAME << ' ' << amount << '\n';
   //if (is_floating_point<T>::value && ! equal_integer<T>(amount)) 
   //if (! equal_integer<T>(amount)) 
   //  throw invalid_argument("dollars can not be floating-point");
   this->amount_in_cents = amount * CENTS_PER_DOLLAR;
   this->amount_in_cents += amount > 0 ? round(cents) : -round(cents);   
}

struct bad_from_string : public bad_cast {
   const char * what() const noexcept override {
      return "Bad cast from string";
   }
};
 
template <typename T, enable_if_t<is_floating_point<T>::value || numeric_limits<T>::is_integer, bool> = true>
T from_string(const string & STR) {
   istringstream stream { STR };
   T result;
   stream >> result;
   if (!stream)
      throw bad_from_string {};
   if (is_floating_point<T>::value && ! stream.eof()) {
      cerr << " T = " << typeid(T).name() << '\n';
      throw invalid_argument(__func__ + string(" entered string is not floating-point format "));
   }
   return result;
}
/*
template <typename T, enable_if_t<numeric_limits<T>::is_integer, bool> = true>
T from_string(const string & STR) {
   istringstream stream { STR };
   T result;
   stream >> result;
   if (!stream)
      throw bad_from_string {};
   return result;
}
*/
template <typename T>
Money<T>::Money(const string & dollars) {
   //T amount = from_string<T>(dollars);
   
   
   const regex float_point = regex { R"(^[+-]?(\d+([.]\d*)?([eE][+-]?\d+)?|[.]\d+([eE][+-]?\d+)?)$)" } ;
   if (! regex_match(dollars, float_point))
		throw invalid_argument("entered string is not floating-point format ");
   T amount = get_amount(dollars);
   
   cerr << " TYPE_NAME = " << TYPE_NAME << ' ' << amount << '\n';
   
   this->amount_in_cents = amount * CENTS_PER_DOLLAR;  
   if (is_floating_point<T>::value) 
      this->amount_in_cents = round(this->amount_in_cents);
}
   /*
Money& Money::operator=(const Money& other) { 
   if (*this != other) {
      numerator = other.numerator;
      denominator = other.denominator; 
   }
   return *this;
} 

Money Money::operator*(const Money& other) const { 
   long n = numerator * other.numerator;
   long d = denominator * other.denominator; 
   return Money(n, d);
} 

Money Money::operator+(const Money& other) const { 
   long n = numerator * other.denominator + denominator * other.numerator;
   long d = denominator * other.denominator; 
   return Money(n, d);
} 

bool Money::operator>(const Money& other) const {
   Money difference = operator-(other);
   return difference.numerator > 0;
}

bool Money::operator<(const Money& other) const {
   Money difference = other.operator-(*this);
   return difference.numerator > 0;
}
*/
}
