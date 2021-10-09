#include "money.h"

namespace money {
   
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

void Money::reduce(long& numerator, long& denominator) {
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
   
Money::Money(long numerator, long denominator) {
   reduce(numerator, denominator);
   this->numerator = numerator;
   this->denominator = denominator;
}
   
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

}
