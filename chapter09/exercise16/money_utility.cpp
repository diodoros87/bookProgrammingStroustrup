#include "integer.hpp"
#include "money.hpp"


namespace money {
   
const Integer CENTS_PER_DOLLAR_INTEGER = Integer::create_Integer(CENTS_PER_DOLLAR);

template<>
Integer Money<Integer>::get_amount(const string & STR) {
   return Integer::parse_create(STR);
}

template <>
//template<typename Integer>
Integer Money<Integer>::calculate_by_Integer(const Integer & dollars, const long double cents /*  = INCORRECT_CENTS */) const {
   cerr << __func__ << '\n';
   Integer amount_in_cents = dollars * CENTS_PER_DOLLAR_INTEGER;
   cerr << __func__ << " amount_in_cents = " << amount_in_cents << '\n';
   if (INCORRECT_CENTS != cents) {
      cerr << __func__ << " cents = " << cents << '\n';
      Integer cents_round = dollars >= Integer::ZERO ? Money<Integer>::round(cents) : -Money<Integer>::round(cents);
      amount_in_cents += cents_round;
   }
   cerr << __func__ << " result = " << amount_in_cents << '\n';
   return amount_in_cents;
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
   Integer amount = Money<Integer>::get_amount(dollars_string);
   //this->amount_in_cents = calculate<long double>(amount, cents);
   this->amount_in_cents = calculate_by_Integer(amount, cents);
   cerr << __func__ << " amount = '" << amount << "' std::signbit = '" << std::boolalpha << std::signbit(amount) << "' \n";  
   if (dollars[0] == '-' && Integer::ZERO == amount)
      this->amount_in_cents = -this->amount_in_cents;
   
   cerr << __func__ << " this->amount_in_cents = '" << TYPE_NAME << "' " << this->amount_in_cents << '\n';   
}

template<>
Money<Integer>::Money(const string & dollars) {   // accept floating-point arguments
   cerr << __func__ << " TYPE_NAME = " << TYPE_NAME << ' ' << dollars << '\n';
   if (! regex_match(dollars, FLOAT_POINT_REGEX))
      throw invalid_argument(string(__func__) +  " Regex: entered string '"
               + dollars + "' is not non-exponent floating-point format "); 
   this->amount_in_cents = calculate_amount_in_cents(dollars);
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

/*
template<>
Money<Integer>& Money<Integer>::operator+=(const Money<Integer>& other) {
   this->amount_in_cents += other.amount_in_cents;
   cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents << '\n';
   return *this;
}

template<>
Money<long double>& Money<long double>::operator+=(const Money<long double>& other) {
   this->amount_in_cents += other.amount_in_cents;
   cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents << '\n';
   return *this;
}
*/
/*
template<>
Money<Integer> Money<Integer>::operator+(const Money<Integer>& other) const {
   Integer sum = this->amount_in_cents + other.amount_in_cents;
   cerr << __func__ << " sum = " << sum << '\n';
   const Integer dollars = sum / CENTS_PER_DOLLAR_INTEGER;
   Integer cents = sum % CENTS_PER_DOLLAR_INTEGER;
   cents = std::move(std::abs(cents));
   Money<Integer> result = Money<Integer>(std::to_string(dollars), static_cast<long double>(cents));
   cerr << __func__ << " result = " << result << '\n';
   return result;
}


Money<Integer> operator+(const Money<Integer>& a, const Money<Integer>& b) {
   const Integer sum = a.amount_in_cents + b.amount_in_cents;
   cerr << __func__ << " sum = " << sum << '\n';
   const Constructor_Args args = constructor_args(sum);
   Money<Integer> result = Money<Integer>(args.DOLLARS, args.CENTS);
   cerr << __func__ << " result = " << result << '\n';
   return result;
}

Money<long long> operator+(const Money<long long>& a, const Money<long long>& b) {
   const long long sum = a.amount_in_cents + b.amount_in_cents;
   cerr << __func__ << " sum = " << sum << '\n';
   const Constructor_Args args = constructor_args<long long>(sum);
   Money<long long> result = Money<long long>(args.DOLLARS, args.CENTS);
   cerr << __func__ << " result = " << result << '\n';
   return result;
}
*/
Constructor_Args constructor_args(const Integer& AMOUNT) {
   const Integer dollars = AMOUNT / CENTS_PER_DOLLAR_INTEGER;
   Integer cents = AMOUNT % CENTS_PER_DOLLAR_INTEGER;
   cents = std::move(std::abs(cents));
   string dollars_string = std::to_string(dollars);
   if (dollars.is_zero() && AMOUNT < Integer::ZERO)
      dollars_string.insert(0, 1, Integer::MINUS);
   Constructor_Args RESULT {dollars_string, static_cast<long double>(cents)};
   return RESULT;
}
/*
Constructor_Args constructor_args(const long double AMOUNT) {
   const long double dollars = AMOUNT / CENTS_PER_DOLLAR;
   string dollars_string = std::to_string(dollars);
   Constructor_Args RESULT {dollars_string, 0};
   return RESULT;
}
*/

/*
template <typename Type, enable_if_t<is_same<Type, Integer>::value || is_same<Type, long double>::value, bool > = true>
Money<Type> operator+(const Money<Type>& a, const Money<Type>& b) {
   const Type sum = a.amount_in_cents + b.amount_in_cents;
   cerr << __func__ << " sum = " << sum << '\n';
   const Constructor_Args args = constructor_args(sum);
   if (is_same<Type, Integer>::value) {
      Money<Type> result = Money<Type>(args.DOLLARS, args.CENTS);
      cerr << __func__ << " result = " << result << '\n';
      return result;
   }
   else {
      Money<Type> result = Money<Type>(args.DOLLARS);
      cerr << __func__ << " result = " << result << '\n';
      return result;
   }
}
*/
Money<Integer> operator+(const Money<Integer>& a, const Money<Integer>& b) {
   const Integer sum = a.amount_in_cents + b.amount_in_cents;
   cerr << __func__ << " sum = " << sum << '\n';
   const Constructor_Args args = constructor_args(sum);
   Money<Integer> result = Money<Integer>::create(args.DOLLARS, args.CENTS);
   cerr << __func__ << " result = " << result << '\n';
   return result;
}

Money<long double> operator+(const Money<long double>& a, const Money<long double>& b) {
   const long double sum = a.amount_in_cents + b.amount_in_cents;
   cerr << __func__ << " sum = " << sum << '\n';
   const string dollars_string = dollars_from_amount(sum);
   Money<long double> result = Money<long double>::create(dollars_string);
   cerr << __func__ << " result = " << result << '\n';
   return result;
}
/*
Money<Integer> operator+(const Money<Integer>& a, const Money<Integer>& b) {
   return a.operator+(b);
}
*/
//template<typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
//            is_integral<Smaller>::value, bool> = true >
   
   /*
#ifdef DEBUG_OSTREAM
   stringstream& operator<<(ostringstream * os, const Money<Integer>& money) {
      validate_pointer(os);
      Integer dollars = money.get_dollars(Money<Integer>::TYPE_DEFAULT_OBJECT);
      Integer cents = money.get_cents(Money<Integer>::TYPE_DEFAULT_OBJECT);
      start_settings(os, money);
      *os << dollars.string_without_signum() << ",";
      *os << setw(2) << cents.string_without_signum();
      return *os;
   }
#endif
   */
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
}