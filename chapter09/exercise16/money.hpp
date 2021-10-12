#include <iostream>

namespace money {

class Money {
public:
   static constexpr short ONE_CENT = 10;
   static constexpr short CENTS_PER_DOLLAR = 100;
   static constexpr short ONE_DOLLAR = CENTS_PER_DOLLAR * ONE_CENT;
   
   static long round(long x) {
      return x % 10 >= 5 ? x / 10 + 1 : x / 10;
   }
   
   Money(const long dollars, const double cents) : amount(ONE_DOLLAR * dollars + ONE_CENT * cents)
   //Money(const long cents);
   
   static Money reverse(const Money& number) { 
      return Money (number.denominator, number.numerator);}

   void set_numerator(long x) { reduce(x, denominator); numerator = x; }
   void set_denominator(long x) { reduce(numerator, x); denominator = x; }
   long get_numerator() const { return numerator; }
   long get_denominator() const { return denominator; }
   
   Money& operator=(const Money& other);
   
   Money operator-() const { return Money(-cents); }
   Money operator+() const { return *this; }
   
   Money operator+(const Money& other) const;
   Money operator-(const Money& other) const { return operator+(-other); }
   Money operator*(const long n) const;
   Money operator/(const long n) const { return operator*(reverse(other)); }
   
   bool operator==(const Money& other) const { return cents == other.cents; }
   bool operator!=(const Money& other) const { return !(*this == other); } ;
   
   bool operator>(const Money& other) const ;
   bool operator<=(const Money& other) const { return !operator>(other); };
   bool operator<(const Money& other) const ;
   bool operator>=(const Money& other) const { return !operator<(other); };
   
   long int dollars() const { return cents / ONE_DOLLAR; }
   
   operator double() const { return static_cast<double>(numerator) / denominator; }
private:
   
   long int amount {0};
};

inline std::ostream& operator<<(std::ostream& os, const Money& number) {
   return os << "(" << number.get_numerator() << " / " << number.get_denominator() << ")";
}

long gcd_algorithm(long a, long b);

}
