#include "rational.h"

namespace rational {
   
long gcd_algorithm(long a, long b) {
   if (a == 0 && b == 0)
      throw "can not calculate gcd for two numbers equals zero!";
   long modulo_result;
   while(b != 0) {
      modulo_result = a % b;
      a = b;
      b = modulo_result;
   }
   return a;
}

void Rational::reduce(long& numerator, long& denominator) {
   if (0 == denominator)
      throw "denominator can not be zero!";
   long gcd = gcd_algorithm(numerator, denominator);
   denominator /= gcd;
   numerator /= gcd;
   if (denominator < 0) {
      denominator = -denominator;
      numerator = -numerator;
   }
}
   
Rational::Rational(long numerator, long denominator) {
   reduce(numerator, denominator);
   this->numerator = numerator;
   this->denominator = denominator;
}
   
Rational& Rational::operator=(const Rational& other) { 
   if (*this != other) {
      numerator = other.numerator;
      denominator = other.denominator; 
   }
   return *this;
} 

Rational Rational::operator*(const Rational& other) const { 
   long n = numerator * other.numerator;
   long d = denominator * other.denominator; 
   return Rational(n, d);
} 

Rational Rational::operator+(const Rational& other) const { 
   long n = numerator * other.denominator + denominator * other.numerator;
   long d = denominator * other.denominator; 
   return Rational(n, d);
} 

bool Rational::operator>(const Rational& other) const {
   Rational difference = operator-(other);
   return difference.numerator > 0;
}

bool Rational::operator<(const Rational& other) const {
   Rational difference = other.operator-(*this);
   return difference.numerator > 0;
}

}
