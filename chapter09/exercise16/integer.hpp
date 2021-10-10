#ifndef INTEGER_HPP
#define INTEGER_HPP

#include<stdexcept>
#include<string>
#include<array>
#include<vector>

using std::array;
using std::vector;

typedef short digit_type;

namespace integer_space {
   
inline void validate_size(const)
   
class Arithmetic_Error : public std::exception { 
   std::string msg {"!!! Arithmetic error: "};
public:
   Arithmetic_Error(const std::string& message) { msg += message; }
   const char* what() {
      return msg.c_str();
   }
};
   
class Integer {
public:
   static constexpr short MAX_ARRAY_LENGTH = 40;
   static constexpr char NEUTRAL_SIGNUM   = ' ';
   static constexpr char PLUS_SIGNUM      = '+';
   static constexpr char MINUS_SIGNUM     = '-';
private:
   array<digit_type, MAX_ARRAY_LENGTH> integer_array;
   char signum {NEUTRAL_SIGNUM};   // less than 0 for integers < 0, more than 0 for integers > 0, 0 for 0   
   //Integer(const vector<digit_type> & vec);
   Integer(const array<digit_type, MAX_ARRAY_LENGTH> & ARRAY);
public:
   //Integer(const vector<digit_type> & vec, const char signum);
   Integer(const array<digit_type, MAX_ARRAY_LENGTH> & table, const char signum) {
      validate(table, signum);
   }
   template <unsigned int N>
   Integer(const array<digit_type, N> & table, const char signum) {
      if (MAX_ARRAY_LENGTH < table.size())
         throw invalid_argument("Requirement: elements <= " + std::to_string(MAX_ARRAY_LENGTH));
      validate(table, signum);
   }
   Integer(const vector<digit_type> & vec, const char signum) {
      if (MAX_ARRAY_LENGTH < vec.size())
         throw invalid_argument("Requirement: elements <= " + to_string(MAX_ARRAY_LENGTH));
      validate(vec, signum);
   }
private:
   array<digit_type, MAX_ARRAY_LENGTH> get_integer_array() const {
      return integer_array;
   }
   
   template <typename Container>
   void validate(const Container& obj, char signum);

public:
   operator std::string() const;
   void set_integer_array(const vector<digit_type> & integer_array);
   //void set(const Integer& integer);
   void reset_number_to_zero() {
      signum = NEUTRAL_SIGNUM;
      integer_array->fill(0);
   }
   array<digit_type, MAX_ARRAY_LENGTH> get_integer_array_copy() const {
      array<digit_type, MAX_ARRAY_LENGTH> copy(integer_array);
      return copy;
   }
   
   void validate_signum(const char signum) const {
      if (signum == NEUTRAL_SIGNUM && false == is_zero()) 
         throw invalid_argument("Requirement: can not set signum to zero for nonzero integer");
      else if (signum != 0 && true == is_zero()) {
         throw invalid_argument("Requirement: can not set signum to other than zero for zero integer");
      }
   }

   void set_signum(const char signum) {
      validate_signum(signum);
      this->signum = signum;
   }
   char get_signum() const { return signum; }
   bool is_zero() { return is_zero(*this); }
   static bool is_zero(const Integer& integer);
   int_fast8_t compare_signum(const Integer& integer) {
      return Integer::compare_signum(*this, integer);
   }
   static int_fast8_t compare_signum(const Integer& first, const Integer& second);
   
   bool operator==(const Integer& other) const { return operator==(*this, other); }
   bool operator!=(const Integer& other) const { return !(*this == other); } ;
   
   static bool is_absolute_value_greater(const Integer& first, const Integer& second);
   static bool is_absolute_value_less(const Integer& first, const Integer& second);
   
   static Integer remainder(const Integer& DIVIDEND, const Integer& DIVISOR);
   static Integer divide_absolute_values(const Integer& DIVIDEND, const Integer& DIVISOR);
   
   static Integer add_absolute_values(const Integer & first, const Integer & second);
   static Integer subtract_absolute_values(const Integer & first, const Integer & second);
   static Integer multiply_absolute_values(const Integer & first, const Integer & second);
   
   static void detect_multiplication_overflow(const Integer & first, const Integer & second);
}

Integer operator/(const Integer& dividend, const Integer& divisor);
Integer operator*(const Integer& first, const Integer& second) ;
Integer operator-(const Integer& first, const Integer& second);
Integer operator-(const Integer& integer);
Integer operator+(const Integer& first, const Integer& second);
bool operator<(const Integer& first, const Integer& second);
bool operator>(const Integer& first, const Integer& second);
bool operator==(const Integer& first, const Integer& second);

inline bool operator<=(const Integer& first, const Integer& second) {
   return ! (first > second);
}

inline bool operator>=(const Integer& first, const Integer& second) {
   return ! (first < second);
}

inline ostream& operator<<(ostream& os, const Integer & integer) {
   return os << std::string(integer);
}
   
}

#endif
