#include <iostream>

namespace rational {

class Rational {
public:
   Rational(long numerator, long denominator);
   Rational() { }
   
   static Rational reverse(const Rational& number) { 
      return Rational (number.denominator, number.numerator);}
   static Rational negate (const Rational& number) { 
      return Rational(-number.numerator, number.denominator); }

   void set_numerator(long x) { reduce(x, denominator); numerator = x; }
   void set_denominator(long x) { reduce(numerator, x); denominator = x; }
   long get_numerator() const { return numerator; }
   long get_denominator() const { return denominator; }
   
   Rational& operator=(const Rational& other);
   
   Rational operator-() const { return Rational(-numerator, denominator); }
   
   Rational operator+(const Rational& other) const;
   Rational operator-(const Rational& other) const { return operator+(-other); }
   Rational operator*(const Rational& other) const;
   Rational operator/(const Rational& other) const { return operator*(reverse(other)); }
   
   bool operator==(const Rational& other) const { 
      return numerator == other.numerator && denominator == other.denominator; }
   bool operator!=(const Rational& other) const { return !(*this == other); } ;
   operator double() const { return static_cast<double>(numerator) / denominator; }
   //bool operator>(const Rational& other) const ;
private:
   void reduce(long& numerator, long& denominator);
   
   long int numerator {1};
   long int denominator {1};
};

inline std::ostream& operator<<(std::ostream& os, const Rational& number) {
   return os << "(" << number.get_numerator() << " / " << number.get_denominator() << ")";
}

long gcd_algorithm(long a, long b);

}
