#include "money.h"

namespace money {
   
Money::Money(const string & dollars, const double cents) {
   T amount = get_amount(dollars);
   if (is_floating_point<T>::value && amount > T(amount)) 
      throw invalid_argument("dollars can not be floating-point");
   this->cents = amount * CENTS_PER_DOLLAR + round(cents);   
}

Money::get_amount(const string & STR) {
   T amount;
   if (is_floating_point<T>::value ) {
      switch(TYPE_NAME) {
         case LONG_DOUBLE_NAME:   
            amount = stold(dollars);
            break;
         case DOUBLE_NAME:  
            amount = stod(dollars);
            break;
         case FLOAT_NAME:   
            amount = stof(dollars);
            break;
      }
   } 
   else if (numeric_limits<T>::is_integer) {
      switch(TYPE_NAME) {
         case UNSIGNED_LONG_LONG_NAME:   
            amount = stoull(dollars);
            break;
         case LONG_LONG_NAME:   
            amount = stoll(dollars);
            break;
         case UNSIGNED_LONG_NAME:   
            amount = stoul(dollars);
            break;
         case LONG_NAME:   
            amount = stol(dollars);
            break;
         case INT_NAME:   
            amount = stoi(dollars);
            break;
         case CHAR_NAME:   
            int tmp = stoi(dollars);
            if (tmp )
            break;
      }
   }
}

   
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
