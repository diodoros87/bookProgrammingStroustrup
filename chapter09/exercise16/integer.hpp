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

typedef int_fast8_t digit_type;

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
   static const string SIZE_INCORRECT;
   static const string SIGNUM_INCORRECT;
private:
   array<digit_type, MAX_ARRAY_LENGTH> integer_array { 0 };
   char signum { NEUTRAL };   // less than 0 for integers < 0, more than 0 for integers > 0, 0 for 0   
   //Integer(const vector<digit_type> & vec);
   Integer(const array<digit_type, MAX_ARRAY_LENGTH> & ARRAY);
public:
   Integer() { }
   Integer(const array<digit_type, MAX_ARRAY_LENGTH> & table, const char signum) {
      validate_init(table, signum);
   }
   
   template <const unsigned int N>
   static Integer create_Integer(const array<digit_type, N> & table, const char signum) {
      validate_size(table);
      Integer integer;
      integer.validate_init(table, signum);
      return integer;
   }
   
   static Integer parse_create(const string& STR) {
      Integer integer;
      integer.parse(STR);
      return integer;
   }
   
   Integer(const vector<digit_type> & vec, const char signum) {
      validate_size(vec);
      validate_init(vec, signum);
   }
private:
   template <typename Container>
   static void validate_size(const Container& obj) {
      if (MAX_ARRAY_LENGTH < obj.size())
         throw invalid_argument(Integer::SIZE_INCORRECT);
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
   static void validate(const char signum) {
      if (Integer::PLUS != signum && Integer::MINUS != signum && Integer::NEUTRAL != signum )
         throw invalid_argument(Integer::SIGNUM_INCORRECT);
   }
   
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
   
   template <const unsigned int N>
   void set_integer_array(const array<digit_type, N> & table) {
      validate_size(table);
      validate_set(table);
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
   size_t size() const { return integer_array.size(); }
   
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

template <typename Container>   // private
void Integer::validate_init(const Container& TABLE, const char signum) {
   //validate_size(TABLE);
   const size_t SIZE = TABLE.size();
   validate(signum);
   digit_type number;
   unsigned short zeros = 0;
   for (short source_index = SIZE - 1; source_index >= 0 ; source_index--) {
      number = TABLE[source_index];
      if (number == 0) 
         zeros++;
      else if (number < 0 || number > 9)
         throw invalid_argument("Requirement: in array must be only integers from 0 to 9");
   }
   if (zeros == SIZE && signum != NEUTRAL) 
      throw invalid_argument("Requirement: signum must be zero for array with only zeros");
   if (zeros < SIZE && signum == NEUTRAL) 
      throw invalid_argument("Requirement: signum can not be zero for array with element other than zero");
   short dest_index = MAX_ARRAY_LENGTH - 1;
   for (short source_index = SIZE - 1; source_index >= 0 ; source_index--) {
      number = TABLE[source_index];
      (*this).integer_array[dest_index] = number;
      dest_index--;
   }
   this->signum = signum;
}

template <typename Container>   // private
void Integer::validate_set(const Container & TABLE) {  // assume that integer_array is positive number, change of signum is allowed by setSignum()
   this->integer_array.fill(0);  // not call reset_number_to_zero() due to change signum (in set_integer_array() save previous signum)
   digit_type number;
   short dest_index = MAX_ARRAY_LENGTH - 1;
   for (short source_index = TABLE.size() - 1; source_index >= 0 ; source_index--) {
      number = TABLE[source_index];
      if (number < 0 || number > 9) {
         reset_number_to_zero();    // similar to constructor, reset to zero
         throw invalid_argument("Requirement: in array must be only integers from 0 to 9");
      }
      this->integer_array[dest_index] = number;
      dest_index--;
   }
   if (this->signum == NEUTRAL && false == is_zero())
      this->signum = PLUS;
   else if (this->signum != NEUTRAL && is_zero()) 
      this->signum = NEUTRAL;
}
   
}

#endif
