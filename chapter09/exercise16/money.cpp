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

const string LONG_DOUBLE_NAME = typeid(0.0L).name();
const string DOUBLE_NAME = typeid(0.0).name();
const string FLOAT_NAME = typeid(0.0f).name();
const string CHAR_NAME = typeid('A').name();
const string INT_FAST_8_T_NAME = typeid(((int_fast8_t)0)).name();
const string SHORT_NAME = typeid(((short)0)).name();
const string UNSIGNED_SHORT_NAME = typeid(((unsigned short)0)).name();
const string INT_NAME = typeid(0).name();
const string LONG_NAME = typeid(0L).name();
const string LONG_LONG_NAME = typeid(numeric_limits<long long>::max()).name();
const string UNSIGNED_INT_NAME = typeid(0u).name();
const string UNSIGNED_LONG_NAME = typeid(0uL).name();
const string UNSIGNED_LONG_LONG_NAME = typeid(numeric_limits<unsigned long long>::max()).name();
const string INTEGER_OBJECT_NAME = typeid(Integer(0)).name();

const regex FLOAT_POINT_REGEX = regex { R"(^[+-]?(\d+([.]\d*)?([eE][+-]?\d+)?|[.]\d+([eE][+-]?\d+)?)$)" } ;

template<typename Smaller, typename Greater>
inline bool is_overflow(const Greater & x) {
   static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) && "Number required.");
   //static_assert((numeric_limits<Smaller>::lowest() >= numeric_limits<Greater>::lowest() 
   //            && numeric_limits<Smaller>::max() <= numeric_limits<Greater>::max()) && "Number required.");
   //static_assert(is_same<U, Integer>::value || is_same<U, long double>::value) && "Number required.");
   return x < numeric_limits<Smaller>::lowest() || x > numeric_limits<Smaller>::max();
}

template <typename Number, enable_if_t<
              is_floating_point<Number>::value || is_integral<Number>::value, bool> = true>
inline bool equal_integer(const Number x) {
   return isfinite(x) && x == trunc(x);
}

inline void validate_cents(const long double & cents) {
   if (0 > cents || cents >= CENTS_PER_DOLLAR) {
      static const string ERROR = "cents must be 0 - " + to_string(CENTS_PER_DOLLAR);
      throw invalid_argument(ERROR);
   }
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
   if (is_floating_point<T>::value ) {  /*
      const long double number = stold(STR);
      if (is_overflow<T, decltype(number)>(number))
         throw invalid_argument("amount = " + std::to_string(number) + " is overflow for type " + TYPE_NAME);
      return static_cast<T>(number); 
      */
      if (TYPE_NAME == LONG_DOUBLE_NAME)   
         return stold(STR);
      else if (TYPE_NAME == DOUBLE_NAME)  
         return stod(STR);
      else if (TYPE_NAME == FLOAT_NAME)   
         return stof(STR);
   } 
   else if (numeric_limits<T>::is_integer) { /*
      if (regex_match(STR, FLOAT_POINT_REGEX)) {
         const long double number = stold(STR);
         if (is_overflow<T, decltype(number)>(number))
            throw invalid_argument("amount = " + std::to_string(number) + " is overflow for type " + TYPE_NAME);
         STR = is_overflow
      }
      Integer integer = Integer::parse_create(STR);
      if (! is_same<T, Integer>::value)
         return T(integer);
      return integer;
      */
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
         unsigned long amount = stoul(STR);
         if (is_overflow<T, decltype(amount)>(amount))
            throw out_of_range("amount = " + std::to_string(amount) + " is overflow for type " + TYPE_NAME);
         return static_cast<T>(amount);
      }
      else if (TYPE_NAME == SHORT_NAME
            || TYPE_NAME == CHAR_NAME
            || TYPE_NAME == INT_FAST_8_T_NAME) {
         int amount = stoi(STR);
         if (is_overflow<T, decltype(amount)>(amount))
            throw out_of_range("amount = " + std::to_string(amount) + " is overflow for type " + TYPE_NAME);
         return static_cast<T>(amount);
      }
      else if (TYPE_NAME == INTEGER_OBJECT_NAME)  
         return Integer::parse_create(STR);
      else
         throw invalid_argument("No implementation for type " + TYPE_NAME);  
      
   }
}
/*
template<typename Smaller, typename Greater>
Smaller calculate(const Smaller & dollars, const long double & cents) {
   static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) && "Number required.");
   static_assert((numeric_limits<Smaller>::lowest() >= numeric_limits<Greater>::lowest() 
               && numeric_limits<Smaller>::max() <= numeric_limits<Greater>::max()) && "Number required.");
   Greater amount_in_cents = Greater(dollars) * Greater(CENTS_PER_DOLLAR);
   Smaller cents_round = dollars > 0 ? round(cents) : -round(cents);
   amount_in_cents += Greater(cents_round);
   if (! is_same<Smaller, Integer>::value)
      if (is_overflow<Smaller, Greater>(amount_in_cents))
         throw out_of_range("amount_in_cents = " + std::to_string(amount_in_cents) + " is overflow for type " + TYPE_NAME);
   Smaller result = static_cast<Smaller>(amount_in_cents);
   return result;
}
*/
template <typename T>
template<typename Greater>
T Money<T>::calculate(const T & dollars, const long double & cents) {
   static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) && "Number required.");
   //static_assert((numeric_limits<T>::lowest() >= numeric_limits<Greater>::lowest() 
   //            && numeric_limits<T>::max() <= numeric_limits<Greater>::max()));
   cerr << __func__ << " TYPE_NAME = " << TYPE_NAME << ' ' << dollars << '\n';
   Greater amount_in_cents = Greater(dollars) * Greater(CENTS_PER_DOLLAR);
   T cents_round = dollars > 0 ? round(cents) : -round(cents);
   amount_in_cents += Greater(cents_round);
   cerr << __func__ << " amount_in_cents = " << amount_in_cents << '\n';
   if (! is_same<T, Integer>::value)
      if (is_overflow<T, Greater>(amount_in_cents))
         throw out_of_range("amount_in_cents = " + std::to_string(amount_in_cents) + " is overflow for type " + TYPE_NAME);
   T result = T(amount_in_cents);
   cerr << __func__ << " result = " << static_cast<int>(result) << '\n';
   return result;
}

/*
template<typename Smaller, typename Greater>
Money<T>::Money(const Smaller & dollars) {
   static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) && "Number required.");
   static_assert((numeric_limits<Smaller>::lowest() >= numeric_limits<Greater>::lowest() 
               && numeric_limits<Smaller>::max() <= numeric_limits<Greater>::max()) && "Number required.");
   Greater amount_in_cents = Greater(dollars) * Greater(CENTS_PER_DOLLAR);
   T cents_round = dollars > 0 ? round(cents) : -round(cents);
   amount_in_cents += Greater(cents_round);
   if (! is_same<T, Integer>::value)
      if (is_overflow<T, decltype(amount_in_cents)>(amount_in_cents))
         throw out_of_range("amount_in_cents = " + std::to_string(amount_in_cents) + " is overflow for type " + TYPE_NAME);
   this->amount_in_cents = amount_in_cents;
}
*/
template <typename T>
Money<T>::Money(const string & dollars, const long double cents) {
   validate_cents(cents);
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
   if (is_floating_point<T>::value) 
      this->amount_in_cents = calculate<long double>(amount, cents);
   //Money<T, long double>(amount);
   else if (numeric_limits<T>::is_integer)
      this->amount_in_cents = calculate<Integer>(amount, cents);
   /*
   else if (numeric_limits<T>::is_integer) {
      Integer amount_in_cents = Integer(amount) * Integer(CENTS_PER_DOLLAR);
      T cents_round = amount > 0 ? round(cents) : -round(cents);
      Integer amount_in_cents += Integer(cents_round);
      amount_in_cents += amount > 0 ? Integer(round(cents)) : -round(cents);
      if (! is_same<T, Integer>::value)
         if (is_overflow<T, decltype(amount_in_cents)>(amount_in_cents))
            throw out_of_range("amount_in_cents = " + std::to_string(amount_in_cents) + " is overflow for type " + TYPE_NAME);
   }
   */
   //this->amount_in_cents = amount_in_cents;   
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
   
   if (! regex_match(dollars, FLOAT_POINT_REGEX))
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
