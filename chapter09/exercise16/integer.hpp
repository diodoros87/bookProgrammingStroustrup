#ifndef INTEGER_HPP
#define INTEGER_HPP

#include<stdexcept>
#include<string>
#include<array>
#include<vector>

using std::array;
using std::vector;
using std::invalid_argument;
using std::string;
using std::to_string;
using std::size_t;

typedef short digit_type;

namespace integer_space {
   
class Arithmetic_Error : public std::exception { 
   string msg {"!!! Arithmetic error: "};
public:
   Arithmetic_Error(const string& message) { msg += message; }
   const char* what() const noexcept {
      return msg.c_str();
   }
};
   
class Integer {
public:
   static constexpr short MAX_ARRAY_LENGTH = 40;
   static constexpr char  NEUTRAL   = ' ';
   static constexpr char  PLUS      = '+';
   static constexpr char  MINUS     = '-';
private:
   array<digit_type, MAX_ARRAY_LENGTH> integer_array;
   char signum { NEUTRAL };   // less than 0 for integers < 0, more than 0 for integers > 0, 0 for 0   
   //Integer(const vector<digit_type> & vec);
   Integer(const array<digit_type, MAX_ARRAY_LENGTH> & ARRAY);
public:
   Integer() { }
   Integer(const array<digit_type, MAX_ARRAY_LENGTH> & table, const char signum) {
      validate_init(table, signum);
   }
   template <unsigned int N>
   Integer(const array<digit_type, N> & table, const char signum) {
      validate_size(table);
      //if (MAX_ARRAY_LENGTH < table.size())
      //   throw invalid_argument("Requirement: elements <= " + std::to_string(MAX_ARRAY_LENGTH));
      validate_init(table, signum);
   }
   Integer(const vector<digit_type> & vec, const char signum) {
      validate_size(vec);
      //if (MAX_ARRAY_LENGTH < vec.size())
      //   throw invalid_argument("Requirement: elements <= " + std::to_string(MAX_ARRAY_LENGTH));
      validate_init(vec, signum);
   }
private:
   template <typename Container>
   static void validate_size(const Container& obj) {
      if (MAX_ARRAY_LENGTH < obj.size())
         throw invalid_argument("Requirement: elements <= " + to_string(MAX_ARRAY_LENGTH));
   }
   array<digit_type, MAX_ARRAY_LENGTH> get_integer_array() const { return integer_array; }
   
   template <typename Container>
   void validate_init(const Container& obj, char signum);
   
   template <typename Container>
   void validate_set(const Container & TABLE);
   
   digit_type & operator[](const size_t i) { return integer_array[i]; }
   
   static void assign_remainder(const Integer & DIVIDEND, const Integer & MULTIPLE_OF_DIVISOR, 
                                       vector<digit_type> & dividend_array);
   static void multiply_divisor(array<digit_type, MAX_ARRAY_LENGTH> & result_array, const short current_dividend_index,
       const Integer & DIVIDEND, vector<digit_type> & dividend_array, const Integer & DIVISOR);
   static array<digit_type, MAX_ARRAY_LENGTH> calculate_dividing(const array<digit_type, MAX_ARRAY_LENGTH> & DIVIDEND_ARRAY, 
               short current_dividend_index, const Integer & DIVISOR);
   
   friend void set_signum_adding_Non_Zero_result(const Integer& first, const Integer& second,
                                       Integer & result, int_fast8_t comparing_signum_result);
   friend void set_opposite_signum(Integer & result, const char signum);
   friend void set_signum_subtracting_Non_Zero_result(const Integer& first, const Integer& second,
                                       Integer & result, int_fast8_t comparing_signum_result);
public:
   operator string() const;
   void set_integer_array(const vector<digit_type> & vec) {
      validate_size(vec);
      validate_set(vec);
   }
   void set_integer_array(const array<digit_type, MAX_ARRAY_LENGTH> & table) {
      validate_set(table);
   }
   
   template <unsigned int N>
   void set_integer_array(const array<digit_type, N> & table) {
      validate_size(table);
      //if (MAX_ARRAY_LENGTH < table.size())
      //   throw invalid_argument("Requirement: elements <= " + std::to_string(MAX_ARRAY_LENGTH));
      validate_set(table, signum);
   }
   //void set(const Integer& integer);
   void reset_number_to_zero() {
      signum = NEUTRAL;
      integer_array.fill(0);
   }
   array<digit_type, MAX_ARRAY_LENGTH> get_integer_array_copy() const {
      array<digit_type, MAX_ARRAY_LENGTH> copy(integer_array);
      return copy;
   }
   
   void validate_signum(const char signum) const {
      if (signum == NEUTRAL && false == is_zero()) 
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
   
   void parse(const string& STR);
   bool is_zero() const { return is_zero(*this); }
   static bool is_zero(const Integer& integer);
   int_fast8_t compare_signum(const Integer& integer) {
      return Integer::compare_signum(*this, integer);
   }
   static int_fast8_t compare_signum(const Integer& first, const Integer& second);
   static bool is_absolute_value_greater(const Integer& first, const Integer& second);
   static bool is_absolute_value_less(const Integer& first, const Integer& second);
   static Integer divide_absolute_values(const Integer& DIVIDEND, const Integer& DIVISOR);
   static Integer add_absolute_values(const Integer & first, const Integer & second);
   static Integer subtract_absolute_values(const Integer & first, const Integer & second);
   static Integer multiply_absolute_values(const Integer & first, const Integer & second);
   static void detect_multiplication_overflow(const Integer & first, const Integer & second);
   
   const digit_type & operator[](const size_t i) const { return integer_array[i]; }
   friend Integer operator-(const Integer& integer);
   friend Integer operator*(const Integer& first, const Integer& second);
   friend Integer operator%(const Integer& DIVIDEND, const Integer& DIVISOR);
   friend Integer operator/(const Integer& dividend, const Integer& divisor);
};

Integer operator-(const Integer& first, const Integer& second);
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

inline bool operator!=(const Integer& first, const Integer& second) {
   return !(first == second);
}

inline std::ostream& operator<<(std::ostream& os, const Integer & integer) {
   return os << string(integer);
}
   
}

#endif
