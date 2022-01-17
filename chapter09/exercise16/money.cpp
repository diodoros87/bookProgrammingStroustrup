#include "money.hpp"
#include "integer_parsing.hpp"

#include <cmath>
#include <limits>
#include <sstream>

using std::numeric_limits;
using std::enable_if_t;
//using std::isfinite;
using std::istringstream;
//using std::bad_cast;
//using std::trunc;

namespace money {
   
template <typename T>
const string Money<T>::TYPE_NAME = typeid(T).name();

template <typename T>
const T Money<T>::TYPE_DEFAULT_OBJECT = T{};

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
const Integer INTEGER_OBJECT = Integer{};
const string INTEGER_OBJECT_NAME = typeid(INTEGER_OBJECT).name();

inline void validate_cents(const long double & cents) {
   if (0 > cents || cents >= CENTS_PER_DOLLAR) {
      static const string ERROR = "cents must be 0 - " + to_string(CENTS_PER_DOLLAR);
      throw invalid_argument(ERROR);
   }
}

template <>
Integer Money<Integer>::get_amount(const string & STR) {
   return Integer::parse_create(STR);
}

#if defined(__clang__)
template <typename T>
T Money<T>::get_amount(const string & STR) {
   cerr << __func__ << " = " << STR << '\n';  
   if (is_same<T, long double>::value)   
      return stold(STR);
   else if (is_same<T, double>::value)  
      return stod(STR);
   else if (is_same<T, float>::value)   
      return stof(STR);
   else if (is_same<T, unsigned long long>::value) 
      return stoull(STR);
   else if (is_same<T, long long>::value)   
      return stoll(STR);
   else if (is_same<T, unsigned long>::value) 
      return stoul(STR);
   else if (is_same<T, long>::value)
      return stol(STR);
   else if (is_same<T, int>::value)   
      return stoi(STR);
   else if (is_same<T, unsigned int>::value 
         || is_same<T, unsigned short>::value) {
      unsigned long amount = stoul(STR);
      if (is_overflow<T, decltype(amount)>(amount))
         throw out_of_range("amount = " + std::to_string(amount) + " is overflow for type " + TYPE_NAME);
      return static_cast<T>(amount);
   }
   else if (is_same<T, short>::value
         || is_same<T, char>::value
         || is_same<T, int_fast8_t>::value) {
      int amount = stoi(STR);
      if (is_overflow<T, decltype(amount)>(amount))
         throw out_of_range("amount = " + std::to_string(amount) + " is overflow for type " + TYPE_NAME);
      return static_cast<T>(amount);
   }
   else
      throw invalid_argument("No implementation for type '" + TYPE_NAME + "'");  
}
#elif defined(__GNUG__)
template <typename T>
T Money<T>::get_amount(const string & STR) {
   cerr << __func__ << " = " << STR << '\n';
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
         throw invalid_argument("No implementation for type '" + TYPE_NAME + "'");  
   }
}
#endif

template <>
template <typename Greater>
Integer Money<Integer>::calculate(const Integer & dollars, const long double cents /*  = INCORRECT_CENTS */) const {
   static_assert(((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) &&
                        ! is_same<Greater, Integer>::value) && "(numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) && \
! is_same<Greater, Integer>::value" );
   Greater amount_in_cents = Greater(dollars) * Greater(CENTS_PER_DOLLAR);
   cerr << __func__ << " amount_in_cents = " << amount_in_cents << '\n';
   if (INCORRECT_CENTS == cents)
      amount_in_cents = Money<Greater>::round(amount_in_cents);
   else {
      Greater cents_round = dollars >= Integer::ZERO ? Money<Greater>::round(cents) : -Money<Greater>::round(cents);
      /*Greater cents_round;
      if (dollars > 0)
         cents_round = Money<Greater>::round(cents);
      else if (dollars < 0)
         cents_round = -Money<Greater>::round(cents);
      else {   // (dollars == 0)
         if (cents < 0)
            cents_round = -Money<Greater>::round(cents);
         else
            cents_round = Money<Greater>::round(cents);
      }*/
      amount_in_cents += Greater(cents_round);
   }
   
   if (is_overflow_for_Integer<Greater>(amount_in_cents))
   //if (is_overflow<T, Greater>(amount_in_cents))
      throw out_of_range("amount_in_cents = " + std::to_string(amount_in_cents) + " is overflow for type " + TYPE_NAME);
   Integer result = Integer::create_Integer(amount_in_cents);
   cerr << __func__ << " result = " << result << '\n';
   return result;
}

template <typename T>
template<typename Greater>
T Money<T>::calculate(const T & dollars, const long double cents /*  = INCORRECT_CENTS */) const {
   static_assert(((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) &&
                        ! is_same<Greater, Integer>::value) && "(numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) && \
! is_same<Greater, Integer>::value" );
   Greater amount_in_cents = Greater(dollars) * Greater(CENTS_PER_DOLLAR);
   cerr << __func__ << " amount_in_cents = " << amount_in_cents << '\n';
   if (INCORRECT_CENTS == cents)
      amount_in_cents = Money<Greater>::round(amount_in_cents);
   else {
      Greater cents_round = dollars >= 0 ? Money<Greater>::round(cents) : -Money<Greater>::round(cents);
      /*Greater cents_round;
      if (dollars > 0)
         cents_round = Money<Greater>::round(cents);
      else if (dollars < 0)
         cents_round = -Money<Greater>::round(cents);
      else {   // (dollars == 0)
         if (cents < 0)
            cents_round = -Money<Greater>::round(cents);
         else
            cents_round = Money<Greater>::round(cents);
      }*/
      amount_in_cents += Greater(cents_round);
   }
   
   if (is_overflow<T, Greater>(amount_in_cents))
      throw out_of_range("amount_in_cents = " + std::to_string(amount_in_cents) + " is overflow for type " + TYPE_NAME);
   T result = T { static_cast<T> (amount_in_cents) };
   //cerr << __func__ << " result = " << static_cast<long long int>(result) << '\n';
   cerr << __func__ << " result = " << std::to_string(result) << '\n';
   return result;
}

const Integer CENTS_PER_DOLLAR_INTEGER = Integer::create_Integer(CENTS_PER_DOLLAR);

template <>
//template<typename Integer>
Integer Money<Integer>::calculate_by_Integer(const Integer & dollars, const long double cents /*  = INCORRECT_CENTS */) const {
   cerr << __func__ << '\n';
   const Integer dollars_as_Integer = Integer::create_Integer(dollars);
   Integer amount_in_cents = dollars_as_Integer * CENTS_PER_DOLLAR_INTEGER;
   cerr << __func__ << " amount_in_cents = " << amount_in_cents << '\n';
   //if (INCORRECT_CENTS == cents)
   //   amount_in_cents = Money<Integer>::round(amount_in_cents);
   if (INCORRECT_CENTS != cents) {
      cerr << __func__ << " cents = " << cents << '\n';
      Integer cents_round = dollars_as_Integer >= Integer::ZERO ? Money<Integer>::round(cents) : -Money<Integer>::round(cents);
      amount_in_cents += cents_round;
   }
   //if (Integer::is_overflow<T>(amount_in_cents))
   //   throw out_of_range("amount_in_cents = " + std::to_string(amount_in_cents) + " is overflow for type " + TYPE_NAME);
   
   //T result = amount_in_cents.operator T();
   cerr << __func__ << " result = " << amount_in_cents << '\n';
   return amount_in_cents;
}

template <typename T>
//template<typename Integer>
T Money<T>::calculate_by_Integer(const T & dollars, const long double cents /*  = INCORRECT_CENTS */) const {
   cerr << __func__ << '\n';
   const Integer dollars_as_Integer = Integer::create_Integer(dollars);
   Integer amount_in_cents = dollars_as_Integer * CENTS_PER_DOLLAR_INTEGER;
   cerr << __func__ << " amount_in_cents = " << amount_in_cents << '\n';
   //if (INCORRECT_CENTS == cents)
   //   amount_in_cents = Money<Integer>::round(amount_in_cents);
   if (INCORRECT_CENTS != cents) {
      cerr << __func__ << " cents = " << cents << '\n';
      Integer cents_round = dollars_as_Integer >= Integer::ZERO ? Money<Integer>::round(cents) : -Money<Integer>::round(cents);
      amount_in_cents += cents_round;
   }
   if (Integer::is_overflow<T>(amount_in_cents))
      throw out_of_range("amount_in_cents = " + std::to_string(amount_in_cents) + " is overflow for type " + TYPE_NAME);
   
   T result = amount_in_cents.operator T();
   cerr << __func__ << " result = " << Integer::create_Integer(result) << '\n';
   return result;
}

template<>
Money<Integer>::Money(const string & dollars, const long double cents) {
   cerr << __func__ << " TYPE_NAME = '" << TYPE_NAME << "' " << dollars << '\n';
   validate_cents(cents);
   if (! regex_match(dollars, INTEGER_REGEX)) 
      throw invalid_argument("Regex: dollars must be integer number ");
   string dollars_string = dollars;
   if (regex_match(dollars, MINUS_ZERO_REGEX))
      dollars_string = dollars.substr(1);
   Integer amount = get_amount(dollars_string);
   //this->amount_in_cents = calculate<long double>(amount, cents);
   this->amount_in_cents = calculate_by_Integer(amount, cents);
   cerr << __func__ << " amount = '" << amount << "' std::signbit = '" << std::boolalpha << std::signbit(amount) << "' \n";  
   if (dollars[0] == '-' && Integer::ZERO == amount)
      this->amount_in_cents = -this->amount_in_cents;
//    if (dollars[0] == '-') {
// #if defined(__clang__)
//       if ((is_same<T, Integer>::value && Integer::ZERO == Integer::create_Integer(amount)) ||
//          (! is_same<T, Integer>::value && 0 == amount) )
// #elif defined(__GNUG__)
//       if (0 == amount)
// #endif
      //if (dollars[0] == '-' && 0 == amount)   
         //this->amount_in_cents = -this->amount_in_cents;
   //}
   
   cerr << __func__ << " this->amount_in_cents = '" << TYPE_NAME << "' " << this->amount_in_cents << '\n';   
}

template <typename T>
Money<T>::Money(const string & dollars, const long double cents) {
   cerr << __func__ << " TYPE_NAME = '" << TYPE_NAME << "' " << dollars << '\n';
   validate_cents(cents);
   if (! regex_match(dollars, INTEGER_REGEX)) 
      throw invalid_argument("Regex: dollars must be integer number ");
   string dollars_string = dollars;
   T amount = get_amount(dollars_string);
   if (is_floating_point<T>::value) 
      this->amount_in_cents = calculate<long double>(amount, cents);
   else if (numeric_limits<T>::is_integer) 
      this->amount_in_cents = calculate_by_Integer(amount, cents);

   cerr << __func__ << " amount = '" << amount << "' std::signbit = '" << std::boolalpha << std::signbit(amount) << "' \n";  
   
//    if (dollars[0] == '-') {
// #if defined(__clang__)
//       if ((is_same<T, Integer>::value && Integer::ZERO == Integer::create_Integer(amount)) ||
//          (! is_same<T, Integer>::value && 0 == amount) )
// #elif defined(__GNUG__)
//       if (0 == amount)
// #endif
   if (dollars[0] == '-' && 0 == amount)   
      this->amount_in_cents = -this->amount_in_cents;
   //}
   
   if (is_same<T, char>::value || is_same<T, int_fast8_t>::value)
      cerr << __func__ << " this->amount_in_cents = '" << TYPE_NAME << "' " << static_cast<int>(this->amount_in_cents) << '\n';
   else
      cerr << __func__ << " this->amount_in_cents = '" << TYPE_NAME << "' " << this->amount_in_cents << '\n';   
}

template <typename Type, enable_if_t<numeric_limits<Type>::is_integer, bool> = true>
Type get_amount_by_Integer(const string & STR) {
   Integer integer = Integer::parse_create(STR);
   if (! is_same<Type, Integer>::value)
      return Type(integer);
   return integer;
}

//#if defined(__clang__)
template<>
Money<Integer>::Money(const string & dollars) {   // accept floating-point arguments
   //T amount = from_string<T>(dollars);
   cerr << __func__ << " TYPE_NAME = " << TYPE_NAME << ' ' << dollars << '\n';
   if (! regex_match(dollars, FLOAT_POINT_REGEX))
      throw invalid_argument(string(__func__) +  " Regex: entered string '"
               + dollars + "' is not non-exponent floating-point format "); /*
   string dollars_string = dollars;
   size_t dot_position = dollars_string.find('.');
   dollars_string = dollars_string.substr(0, dot_position);
   T dollars_part = get_amount(dollars_string);
   cerr << __func__ << " dot_position = " << dot_position <<  '\n';
   if (dot_position == string::npos) {
      //string dollars_string = dollars.substr(0, dot_position);
      //T dollars_part = get_amount(dollars_string);
      cerr << __func__ << " dot_position = string::npos " << dot_position <<  '\n';
      this->amount_in_cents = calculate(dollars_part);
      cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents <<  '\n';
   }
   else {
      string cents_string = dollars.substr(dot_position + 1);
      if (cents_string.size() > 2)
         cents_string.insert(2, ".");
      cerr << __func__ << " cents_string = " << cents_string <<  '\n';
      long double cents_part = stold(cents_string);
      this->amount_in_cents = calculate(dollars_part, cents_part);
      if (dollars[0] == '-' && this->amount_in_cents > 0 && this->amount_in_cents <= 100)
         this->amount_in_cents = -this->amount_in_cents;
   } */
   this->amount_in_cents = calculate_amount_in_cents(dollars);
}
//#endif

template <typename T>
Money<T>::Money(const string & dollars) {   // accept floating-point arguments
   //T amount = from_string<T>(dollars);
   cerr << __func__ << " TYPE_NAME = " << TYPE_NAME << ' ' << dollars << '\n';
   if (is_floating_point<T>::value) {
      if (! regex_match(dollars, E_FLOAT_POINT_REGEX)) 
         throw invalid_argument(string(__func__) +  " Regex: entered string '"
               + dollars + "' is not floating-point format ");
   }
   else if (numeric_limits<T>::is_integer) { 
      if (! regex_match(dollars, FLOAT_POINT_REGEX))
         throw invalid_argument(string(__func__) +  " Regex: entered string '"
               + dollars + "' is not non-exponent floating-point format "); /*
      string dollars_string = dollars;
      size_t dot_position = dollars_string.find('.');
      dollars_string = dollars_string.substr(0, dot_position);
      T dollars_part = get_amount(dollars_string);
      cerr << __func__ << " dot_position = " << dot_position <<  '\n';
      if (dot_position == string::npos) {
         //string dollars_string = dollars.substr(0, dot_position);
         //T dollars_part = get_amount(dollars_string);
         cerr << __func__ << " dot_position = string::npos " << dot_position <<  '\n';
         this->amount_in_cents = calculate(dollars_part);
         cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents <<  '\n';
      }
      else {
         string cents_string = dollars.substr(dot_position + 1);
         if (cents_string.size() > 2)
            cents_string.insert(2, ".");
         cerr << __func__ << " cents_string = " << cents_string <<  '\n';
         long double cents_part = stold(cents_string);
         this->amount_in_cents = calculate(dollars_part, cents_part);
         if (dollars[0] == '-' && this->amount_in_cents > 0 && this->amount_in_cents <= 100)
            this->amount_in_cents = -this->amount_in_cents;
      } */
   }
   this->amount_in_cents = calculate_amount_in_cents(dollars);
}


bool is_exact(const string & number) {
   size_t dot_position = number.find('.');
   if (dot_position != string::npos) {
      
   }
   
   return true;
}
/*
template <>
Money<long double> Money<long double>::create(const string & dollars) {   
   const long double amount = from_string<long double>(dollars, true) * CENTS_PER_DOLLAR;
   if (! equal_integer<long double>(amount))
      throw invalid_argument("Not exact value dollars = " + to_string(amount));
   Money<T> money = Money<T>(dollars);
   const long double amount_2 = static_cast<long double>(money.get_amount_in_cents());
   if (amount_2 != amount)
      throw invalid_argument("Not exact value dollars = " + to_string(amount) " != " + to_string(amount_2));
   return money;
}
*/

template <typename T>
Money<T> Money<T>::create(const string & dollars, const long double cents) {   
   //long double amount = from_string<long double>(dollars, true) * CENTS_PER_DOLLAR + cents;
   //if (! equal_integer<long double>(amount))
   //   throw invalid_argument("Not exact value dollars = " + to_string(amount));
   if (! equal_integer<long double>(cents))
      throw invalid_argument("Not exact value cents = " + to_string(cents));
   Money<T> money = Money<T>(dollars, cents);
   //const long double amount_2 = static_cast<long double>(money.get_amount_in_cents());
   //if (amount_2 != amount)
   //   throw invalid_argument("Not exact value dollars = " + to_string(amount) + " != " + to_string(amount_2));
   return money;
}

template <typename Number, enable_if_t< is_floating_point<Number>::value, bool> = true> 
inline Number negate_minus_zero (const Number & n) {
   return n == 0 && signbit(n) ? -n : n;
}

template <typename Number, enable_if_t< is_floating_point<Number>::value, bool> = true>
string formatted_string(Number & dollars, Number & cents) {
   cerr << __func__ << " dollars = " << dollars <<  '\n';
   cerr << __func__ << " cents = " << cents <<  '\n';
   dollars = negate_minus_zero(dollars);
   if (signbit(cents)) 
      cents = -cents;
   cerr << __func__ << " dollars = " << dollars <<  '\n';
   cerr << __func__ << " cents = " << cents <<  '\n';
   ostringstream stream;
   stream << fixed << setprecision(0) << setw(0) << dollars << ",";
   stream.fill('0');
   stream << setw(2) << cents;
   string out = stream.str();
   return out;
}

template <typename Number, enable_if_t< is_integral<Number>::value, bool> = true>
string formatted_string(const Number dollars, const Number cents) {
   string dollars_string;
   string cents_string;
   if (is_same<Number, char>::value || is_same<Number, int_fast8_t>::value) {
      dollars_string = std::to_string(static_cast<int>(dollars));
      cerr << __func__ << " dollars_string = " << dollars_string <<  '\n';
      cents_string = std::to_string(static_cast<int>(cents));
      cerr << __func__ << " cents_string = " << cents_string <<  '\n';
   }
   else {
      dollars_string = std::to_string(dollars);
      cerr << __func__ << " dollars_string = " << dollars_string <<  '\n';
      cents_string = std::to_string(cents);
      cerr << __func__ << " cents_string = " << cents_string <<  '\n';
   }
   cents_string = cents_string.length() == 1 ? ("0" + cents_string) : cents_string;
   //string out = (dollars == 0 && get_amount_in_cents() < static_cast<T>(0)) ? "-" : "";
   string out = dollars_string + "," + cents_string;
   return out;
}

string formatted_string(const Integer & dollars, const Integer & cents) {
   ostringstream stream;
   stream << dollars;
   string dollars_string = stream.str();
   if (dollars > Integer::ZERO)
      dollars_string = dollars_string.substr(1);
   reset(stream);
   stream << cents;
   string cents_string = stream.str();
   if (! cents.is_zero())
      cents_string = cents_string.substr(1);
   cents_string = cents_string.length() == 1 ? ("0" + cents_string) : cents_string;
   string out = dollars_string + "," + cents_string;
   return out;
}

template<>
Money<Integer>::operator string() const {
   Integer dollars = get_dollars(TYPE_DEFAULT_OBJECT);
   Integer cents = get_cents(TYPE_DEFAULT_OBJECT);
   if (cents < Integer::ZERO) 
      cents = -cents;
   string out = (dollars == Integer::ZERO && signbit(get_amount_in_cents())) ? "-" : "";
   out += formatted_string(dollars, cents);
   return out;
}

template <typename T>
Money<T>::operator string() const {
   T dollars = get_dollars(TYPE_DEFAULT_OBJECT);
   T cents = get_cents(TYPE_DEFAULT_OBJECT);
   cerr << __func__ << " get_cents " << cents << '\n';
   if (is_integral<T>::value && signbit(cents)) {
      cerr << __func__ << "\n --------------------------- cents < static_cast<T>(0)) " << '\n';
      cents = -cents;
   }
   cerr << __func__ << " cents " << cents << '\n';
   cerr << __func__ << " get_amount_in_cents() " << get_amount_in_cents() << '\n';
   string out = (dollars == 0 && get_amount_in_cents() < static_cast<T>(0)) ? "-" : "";
   out += formatted_string(dollars, cents);
   return out;
}

template <typename T>
template<typename Greater>
Money<T> Money<T>::operator*(const T & FACTOR) const {
   static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) &&
                        ! is_same<Greater, Integer>::value );
   Greater product = Greater(this->amount_in_cents) * Greater(FACTOR);
   product = Money<Greater>::round(product);
   cerr << __func__ << " product = " << product << '\n';
   if (is_overflow<T, Greater>(product))
      throw out_of_range(__func__ + "amount = " + std::to_string(product) + " is overflow for type " + TYPE_NAME);
   const string dollars = std::to_string(product);
   Money<T> result = Money<T>(dollars);
   cerr << __func__ << " result = " << result << '\n';
   return result;
}

template <typename T>
//template<typename Integer>
Money<T> Money<T>::operator*(const T & FACTOR) const {
   //static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) && "Number required.");
   cerr << __func__ << '\n';
   Integer product = Integer::create_Integer(this->amount_in_cents) * Integer::create_Integer(FACTOR);
   product = Money<Integer>::round(product);
   cerr << __func__ << " product = " << product << '\n';
   if (! is_same<T, Integer>::value)
      if (is_overflow<T, Integer>(amount_in_cents))
         throw out_of_range("amount_in_cents = " + std::to_string(amount_in_cents) + " is overflow for type " + TYPE_NAME);
   const string dollars = std::to_string(product);
   Money<T> result = Money<T>(dollars);
   cerr << __func__ << " result = " << result << '\n';
   return result;
}

/*
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
