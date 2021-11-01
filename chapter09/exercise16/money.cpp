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

const regex E_FLOAT_POINT_REGEX = regex { R"(^[+-]?(\d+([.]\d*)?([eE][+-]?\d+)?|[.]\d+([eE][+-]?\d+)?)$)" } ;
const regex FLOAT_POINT_REGEX   = regex { R"(^[+-]?(\d+([.]\d*)?|[.]\d+)$)" } ;
const regex INTEGER_REGEX       = regex { R"(^[+-]?(\d+)$)" } ;
const regex MINUS_ZERO_REGEX    = regex { R"(^-0([.][0-9]+)?$)" } ;

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

template <typename T>
T Money<T>::get_amount(const string & STR) {
   cerr << __func__ << " = " << STR << '\n';
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
      else if (TYPE_NAME == INTEGER_OBJECT_NAME) {
         //if (STR.size() > 1 && integer_parsing::string_contain_only_value(STR, '0'))
         //   return Integer::parse_create(STR.substr(1));
         //else
         return Integer::parse_create(STR);
      }
      else
         throw invalid_argument("No implementation for type '" + TYPE_NAME + "'");  
      
   }
}

template <typename T>
template<typename Greater>
T Money<T>::calculate(const T & dollars, const long double cents /*  = INCORRECT_CENTS */) const {
   static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) &&
                        ! is_same<Greater, Integer>::value );
   Greater amount_in_cents = Greater(dollars) * Greater(CENTS_PER_DOLLAR);
   cerr << __func__ << " amount_in_cents = " << amount_in_cents << '\n';
   if (INCORRECT_CENTS == cents)
      amount_in_cents = Money<Greater>::round(amount_in_cents);
   else {
      Greater cents_round = dollars >= 0 ? Money<Greater>::round(cents) : -Money<Greater>::round(cents);/*
      Greater cents_round;
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
    if (! is_same<T, Integer>::value)
      if (is_overflow<T, Greater>(amount_in_cents))
         throw out_of_range("amount_in_cents = " + std::to_string(amount_in_cents) + " is overflow for type " + TYPE_NAME);
   T result = T(amount_in_cents);
   cerr << __func__ << " result = " << static_cast<long long int>(result) << '\n';
   return result;
}

const Integer CENTS_PER_DOLLAR_INTEGER = Integer::create_Integer(CENTS_PER_DOLLAR);

template <typename T>
//template<typename Integer>
T Money<T>::calculate(const T & dollars, const long double cents /*  = INCORRECT_CENTS */) const {
   //static_assert((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) && "Number required.");
   cerr << __func__ << '\n';
   Integer amount_in_cents = Integer::create_Integer(dollars) * CENTS_PER_DOLLAR_INTEGER;
   cerr << __func__ << " amount_in_cents = " << amount_in_cents << '\n';
   /*
   if (numeric_limits<T>::is_integer)
      amount_in_cents = Greater(static_cast<long long>(dollars)) * Greater(static_cast<long double>(CENTS_PER_DOLLAR));
   else if (is_floating_point<T>::value)
      amount_in_cents = Greater(static_cast<long double>(dollars)) * Greater(static_cast<long double>(CENTS_PER_DOLLAR)); */
   if (INCORRECT_CENTS == cents)
      amount_in_cents = Money<Integer>::round(amount_in_cents);
   else {
      cerr << __func__ << " cents = " << cents << '\n';
      Integer cents_round = dollars >= 0 ? Money<Integer>::round(cents) : -Money<Integer>::round(cents);/*
      Integer cents_round;
      if (dollars > 0)
         cents_round = Money<Integer>::round(cents);
      else if (dollars < 0)
         cents_round = -Money<Integer>::round(cents);
      else {   // (dollars == 0)
         if (cents < 0)
            cents_round = -Money<Integer>::round(cents);
         else
            cents_round = Money<Integer>::round(cents);
      }*/
      amount_in_cents += cents_round; // Integer::create_Integer(cents_round);
   }
   if (! is_same<T, Integer>::value)
      if (is_overflow<T, Integer>(amount_in_cents))
         throw out_of_range("amount_in_cents = " + std::to_string(amount_in_cents) + " is overflow for type " + TYPE_NAME);
   T result = T(amount_in_cents);
   cerr << __func__ << " result = " << Integer::create_Integer(result) << '\n';
   return result;
}

template <typename T>
Money<T>::Money(const string & dollars, const long double cents) {
   cerr << __func__ << " TYPE_NAME = '" << TYPE_NAME << "' " << dollars << '\n';
   validate_cents(cents);
   if (! regex_match(dollars, INTEGER_REGEX)) 
      throw invalid_argument("Regex: dollars must be integer number ");
   string dollars_string = dollars;
   if (is_same<T, Integer>::value && regex_match(dollars, MINUS_ZERO_REGEX))
      dollars_string = dollars.substr(1);
   T amount = get_amount(dollars_string);
   if (is_floating_point<T>::value) 
      this->amount_in_cents = calculate<long double>(amount, cents);
   else if (numeric_limits<T>::is_integer)
      this->amount_in_cents = calculate(amount, cents);
   cerr << __func__ << " amount = '" << amount << "' std::signbit = '" << std::boolalpha << std::signbit(amount) << "' \n";  
   if (0 == amount && dollars[0] == '-')
      this->amount_in_cents = -this->amount_in_cents;
   if (is_same<T, char>::value || is_same<T, int_fast8_t>::value)
      cerr << __func__ << " this->amount_in_cents = '" << TYPE_NAME << "' " << static_cast<int>(this->amount_in_cents) << '\n';
   else
      cerr << __func__ << " this->amount_in_cents = '" << TYPE_NAME << "' " << this->amount_in_cents << '\n';   
}
 
struct bad_from_string : public bad_cast {
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
      throw invalid_argument(__func__ + string(" entered string is not accepted format "));
   }
   return result;
}

template <typename Type, enable_if_t<numeric_limits<Type>::is_integer, bool> = true>
Type get_amount_by_Integer(const string & STR) {
   Integer integer = Integer::parse_create(STR);
   if (! is_same<Type, Integer>::value)
      return Type(integer);
   return integer;
}

template <typename T>
//template <typename Number, enable_if_t<numeric_limits<Number>::is_integer, bool> = true>
T Money<T>::calculate_amount_in_cents(const string & dollars) {
   //static_assert(numeric_limits<T>::is_integer);
   T result;
   size_t dot_position = dollars.find('.');
   cerr << __func__ << " dot_position = " << dot_position <<  '\n';
   string dollars_string = dollars.substr(0, dot_position);
   if (is_same<T, Integer>::value && regex_match(dollars, MINUS_ZERO_REGEX)) {
      cerr << __func__ << " dollars_string = " << dollars_string <<  '\n';
      dollars_string = dollars_string.substr(1);
      cerr << __func__ << " dollars_string = " << dollars_string <<  '\n';
   }
   T dollars_part = get_amount(dollars_string);
   
   if (dot_position == string::npos) {
      //string dollars_string = dollars.substr(0, dot_position);
      //T dollars_part = get_amount(dollars_string);
      cerr << __func__ << " dot_position = string::npos " << dot_position <<  '\n';
      result = calculate(dollars_part);
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
      result = calculate(dollars_part, cents_part);
      if (dollars[0] == '-' && result > 0 && result <= 100)
         result = -result;
   }
   return result;
}

template <typename T>
Money<T>::Money(const string & dollars) {   // accept floating-point arguments
   //T amount = from_string<T>(dollars);
   cerr << __func__ << " TYPE_NAME = " << TYPE_NAME << ' ' << dollars << '\n';
   if (is_floating_point<T>::value) {
      if (! regex_match(dollars, E_FLOAT_POINT_REGEX)) 
         throw invalid_argument("Regex: entered string is not floating-point format ");
      T dollars_part = get_amount(dollars);
      this->amount_in_cents = calculate<long double>(dollars_part);
   }
   else if (numeric_limits<T>::is_integer) { 
      if (! regex_match(dollars, FLOAT_POINT_REGEX))
         throw invalid_argument("Regex: entered string is not floating-point format "); /*
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
}

template <typename Number, enable_if_t< is_floating_point<Number>::value> = true>
string formatted_string(const Number & dollars, const Number & cents) {
   ostringstream stream;
   stream << fixed << setprecision(0) << setw(0) << dollars << ",";
   stream.fill('0');
   stream << setw(2) << cents;
   string out = stream.str();
   return out;
}

template <typename Number, enable_if_t< is_integral<Number>::value > = true>
string formatted_string(const Number & dollars, const Number & cents) {
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
      cents_string = std::to_string(cents);
   }
   cents_string = cents_string.length() == 1 ? ("0" + cents_string) : cents_string;
   string out = dollars_string + "," + cents_string;
   return out;
}

string formatted_string(const Integer & dollars, const Integer & cents) {
   ostringstream stream;
   stream << dollars;
   string dollars_string = stream.str();
   //cerr << __func__ << " dollars_string " << dollars_string << "\n";
   if (dollars > Integer::ZERO)
      dollars_string = dollars_string.substr(1);
   //cerr << " dollars_string " << dollars_string << "\n";
   reset(stream);
   stream << cents;
   string cents_string = stream.str();
   if (! cents.is_zero())
      cents_string = cents_string.substr(1);
   cents_string = cents_string.length() == 1 ? ("0" + cents_string) : cents_string;
   string out = dollars_string + "," + cents_string;
   //cerr << __func__ << " out " << out << "\n";
   return out;
}

template <typename T>
Money<T>::operator string() const {
   T dollars = get_dollars(TYPE_DEFAULT_OBJECT);
   T cents = get_cents(TYPE_DEFAULT_OBJECT);
   if (cents < 0) 
      cents = -cents;
   string out = (dollars == 0 && get_amount_in_cents() < 0) ? "-" : "";
   out += formatted_string(dollars, cents);
   return out;
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
