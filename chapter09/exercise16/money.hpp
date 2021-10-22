#include <iostream>

namespace money {

template <typename T>
class Money {
public:
   static constexpr short CENTS_PER_DOLLAR = 100;
   static const string TYPE_NAME;
   
   static const string LONG_DOUBLE_NAME;
   static const string DOUBLE_NAME;
   static const string FLOAT_NAME;
   
   static const string CHAR_NAME;
   static const string SIGNED_CHAR_NAME;
   static const string UNSIGNED_CHAR_NAME;
   static const string SHORT_NAME;
   static const string UNSIGNED_SHORT_NAME;
   static const string INT_NAME;
   static const string UNSIGNED_INT_NAME;
   static const string LONG_NAME;
   static const string UNSIGNED_LONG_NAME;
   static const string LONG_LONG_NAME;
   static const string UNSIGNED_LONG_LONG_NAME;
   static const string SIZE_T_NAME;
   
   static T round(double x) {
      return static_cast<T>(x + 0.5);
   }
   
   Money(const string & dollars, const double cents = 0) : amount(CENTS_PER_DOLLAR * dollars + round(cents))
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
   
   T dollars() const { return cents / CENTS_PER_DOLLAR; }
   
   operator double() const { return static_cast<double>(numerator) / denominator; }
private:
   
   T cents {0};
};

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

inline std::ostream& operator<<(std::ostream& os, const Money& number) {
   return os << "(" << number.get_numerator() << " / " << number.get_denominator() << ")";
}

long gcd_algorithm(long a, long b);

}
