#include "integer.hpp"
#include "integer_parsing.hpp"

#include <cassert>
#include <string>
#include <cstdint>
#include <climits>
#include <vector>

#include<iostream>
using std::cerr;

using std::runtime_error;

namespace integer_space {
   
const string Integer::SIZE_INCORRECT = "Requirement: elements <= " + to_string(Integer::MAX_ARRAY_LENGTH);
const string Integer::SIGNUM_INCORRECT = "Accepted signum: '" + string(1, Integer::MINUS) + "', '" + string(1, Integer::NEUTRAL) 
         + "', " + string(1, Integer::PLUS) + "'\n";
/*
static inline void validate(const char signum) {
   if (Integer::PLUS != signum && Integer::MINUS != signum && Integer::NEUTRAL != signum )
      throw invalid_argument(SIGNUM_INCORRECT);
      //throw invalid_argument("Accepted signum: '" + string(1, Integer::MINUS) + "', '" + string(1, Integer::NEUTRAL) 
      //   + "', " + string(1, Integer::PLUS) + "'\n");
}
*/
/*
template <typename T>
static inline void validate_size(const vector<digit_type> & vec) {
   const short VEC_SIZE = vec.size();
   if (0 == VEC_SIZE)
      throw invalid_argument("Requirement: vector.length > 0");
   if (MAX_ARRAY_LENGTH < VEC_SIZE)
      throw invalid_argument("Requirement: vector.length <= " + to_string(MAX_ARRAY_LENGTH));
}

template <typename Container>
static inline void validate_size(const Container& TABLE) {
   const short SIZE = TABLE.size();
   //if (0 == SIZE)
   //   throw invalid_argument("Requirement: length > 0");
   if (MAX_ARRAY_LENGTH < SIZE)
      throw invalid_argument("Requirement: length <= " + to_string(MAX_ARRAY_LENGTH));
}
*/
/*
template <typename Container>   // private
void Integer::validate_init(const Container& TABLE, const char signum) {
   //validate_size(TABLE);
   const size_t SIZE = TABLE.size();
   validate(signum);
   digit_type number;
   int_fast8_t zeros = 0;
   for (short source_index = SIZE - 1; source_index >= 0 ; source_index--) {
      number = TABLE[source_index];
      if (number == 0) 
         zeros++;
      else if (number < 0 || number > 9)
         throw invalid_argument("Requirement: in array must be only integers from 0 to 9, but input number is: "
            + to_string(number));
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
*/
/*
Integer::Integer(const vector<digit_type> & vec, const char signum) {
   validate_size(vec);
   validate(vec, signum);
}
*/
// private constructor with automatically signum's inserting, using only in absolute values calculation
Integer::Integer(const array<digit_type, MAX_ARRAY_LENGTH> & ARRAY) {    // private
   digit_type number;
   int_fast8_t zeros = 0;
   short dest_index = MAX_ARRAY_LENGTH - 1;
   for (short source_index = MAX_ARRAY_LENGTH - 1; source_index >= 0 ; source_index--) {
      number = ARRAY[source_index];
      if (number == 0) 
         zeros++;
      else if (number < 0 || number > 9)
         throw invalid_argument("Requirement: in array must be only integers from 0 to 9, but input number is: "
            + to_string(number));
      integer_array[dest_index] = number;
      dest_index--;
   }
   this->signum = zeros == MAX_ARRAY_LENGTH ? NEUTRAL : PLUS;
}

/* own copy constructor or default  copy constructor ?
public Integer(Integer integer) {
   this(integer.integer_array, integer.signum);
}

void set(const Integer& integer) {
   if (null == integer) {
      throw new NullPointerException("Requirement: reference to Integer can not be null");
   }

   System.arraycopy(integer.integer_array, 0, this.integer_array, 0, integer_array.length);
   this.signum = integer.signum;
}
*/
/*
void Integer::set_integer_array(const vector<digit_type> & VEC) {  // assume that integer_array is positive number, change of signum is allowed by setSignum()
   if (MAX_ARRAY_LENGTH < VEC.size())
      throw invalid_argument("Requirement: elements <= " + to_string(MAX_ARRAY_LENGTH));
   this->integer_array->fill(0);  // not call reset_number_to_zero() due to change signum (in set_integer_array() save previous signum)
   digit_type number;
   short dest_index = MAX_ARRAY_LENGTH - 1;
   for (short source_index = VEC.size() - 1; source_index >= 0 ; source_index--) {
      number = VEC[source_index];
      if (number < 0 || number > 9) {
         reset_number_to_zero();    // similar to constructor, reset to zero
         throw invalid_argument("Requirement: in array must be only integers from 0 to 9, but input number is: "
            + to_string(number));
      }
      this->integer_array[dest_index] = number;
      dest_index--;
   }
   if (this->signum == NEUTRAL && false == is_zero())
      this->signum = PLUS;
   else if (this->signum != NEUTRAL && is_zero()) 
      this->signum = NEUTRAL;
}
*/
//using integer_parsing::validate_string
void Integer::parse(const string& STR) {
   integer_parsing::validate_string(STR);
   reset_number_to_zero();
   short iteration_stop_index = 0;
   if (STR[0] == PLUS || STR[0] == MINUS) {
      this->signum = STR[0];
      iteration_stop_index++;
   }
   for (short string_index = STR.length() - 1, array_index = MAX_ARRAY_LENGTH - 1;
                        string_index >= iteration_stop_index; string_index--, array_index--) 
      this->integer_array[array_index] = STR[string_index] - '0';

   if (is_zero()) 
      this->signum = NEUTRAL;
   else if (this->signum != MINUS)   // string without leading char '+' or '-' is '+'
      this->signum = PLUS;
}

bool Integer::is_zero(const Integer& integer) {
   for (digit_type digit : integer.integer_array)
      if (0 != digit) {
         //assert(integer.signum != 0);
         return false;
      }
   //assert(integer.signum == 0);
   return true;
}

int_fast8_t Integer::compare_signum(const Integer& first, const Integer& second) {
   if (first.signum == PLUS) {
      if (second.signum == PLUS)
         return 0;
      else   
         return +1;
   }
   else if (first.signum == NEUTRAL) {
      switch (second.signum) {
         case PLUS:
            return -1;
         case NEUTRAL:
            return 0;
         case MINUS:
            return +1;
      }
   }
   else {  // first->signum  == MINUS
      if (second.signum == MINUS) 
         return 0;
      else 
         return -1;
   }
}

bool Integer::is_absolute_value_greater(const Integer& first, const Integer& second) {
   digit_type first_digit;
   digit_type second_digit;
   for (short index = 0; index < first.integer_array.size(); index++) {
      first_digit  = first[index];
      second_digit = second[index];
      if (first_digit > second_digit) 
         return true;
      else if (first_digit < second_digit) 
         return false;
   }
   return false;
}

bool Integer::is_absolute_value_less(const Integer& first, const Integer& second) {
   digit_type first_digit;
   digit_type second_digit;
   for (short index = 0; index < first.integer_array.size(); index++) {
      first_digit  = first[index];
      second_digit = second[index];
      if (first_digit < second_digit) 
         return true;
      else if (first_digit > second_digit) 
         return false;
   }
   return false;
}

bool operator==(const Integer& first, const Integer& second) {
   for (short index = 0; index < Integer::MAX_ARRAY_LENGTH; index++) 
      if (first[index] != second[index]) 
         return false;

   int_fast8_t comparing_signum_result = Integer::compare_signum(first, second);
   if (0 != comparing_signum_result) 
      return false;
   //assert(0 == comparing_signum_result);
   return true;
}

bool operator>(const Integer& first, const Integer& second) {
   int_fast8_t comparing_signum_result = Integer::compare_signum(first, second);
   if (-1 == comparing_signum_result) 
      return false;
   else if (+1 == comparing_signum_result) 
      return true;
   // after calling Integer::compare_signum recognize both integers with identical nonzero signum

   assert(first.get_signum() == second.get_signum());
   assert(first.get_signum() != 0);
   int_fast8_t identical_integers_signum = first.get_signum();
   
   if (identical_integers_signum > 0) 
      return Integer::is_absolute_value_greater(first, second);
   else if (identical_integers_signum < 0) 
      return Integer::is_absolute_value_less(first, second);
   else  // identical_integers_signum == 0
      throw runtime_error("signum of integer = 0");
}

bool operator<(const Integer& first, const Integer& second) {
   int_fast8_t comparing_signum_result = Integer::compare_signum(first, second);
   if (-1 == comparing_signum_result) 
      return true;
   else if (+1 == comparing_signum_result) 
      return false;
   // after calling Integer.compare_signum recognize both integers with identical nonzero signum

   assert(first.get_signum() == second.get_signum());
   assert(first.get_signum() != 0);
   int_fast8_t identical_integers_signum = first.get_signum();

   if (identical_integers_signum < 0) 
      return Integer::is_absolute_value_greater(first, second);
   else if (identical_integers_signum > 0) 
      return Integer::is_absolute_value_less(first, second);
   else  // identical_integers_signum == 0
      throw runtime_error("signum of integer = 0");
}

void set_signum_adding_Non_Zero_result(const Integer& first, const Integer& second,
                                       Integer & result, int_fast8_t comparing_signum_result) {
   if (result.signum == 0)   // skip setSignum() for zero, zero has correct signum = 0 in private Integer constructor
      return;
   if (0 == comparing_signum_result)        // integers with identical signums
      result.signum = first.signum;
   else {  // integers with different signums
      if (true == Integer::is_absolute_value_greater(first, second))
         result.signum = first.signum;
      else 
         result.signum = second.signum;
   }
}

Integer operator+(const Integer& first, const Integer& second) {
   Integer result;
   int_fast8_t comparing_signum_result = Integer::compare_signum(first, second);
   if (0 == comparing_signum_result)     // integers with identical signums
      result = Integer::add_absolute_values(first, second);
   else  // integers with different signums
      result = Integer::subtract_absolute_values(first, second);

   set_signum_adding_Non_Zero_result(first, second, result, comparing_signum_result);

   return result;
}

static char get_opposite_signum(const char signum) {
   switch(signum) {
      case Integer::MINUS:
         return Integer::PLUS;
         break;
      case Integer::NEUTRAL:
         return Integer::NEUTRAL;
         break;
      case Integer::PLUS:
         return Integer::MINUS;
      default:
         throw runtime_error("Unrecognized signum");
   }
}

Integer operator-(const Integer& integer) {
   const char opposite = get_opposite_signum(integer.get_signum());
   return Integer(integer.get_integer_array(), opposite);
}

inline void set_opposite_signum(Integer & integer, const char signum) {
   const char opposite = get_opposite_signum(signum);
   integer.signum = opposite;
}

void set_signum_subtracting_Non_Zero_result(const Integer& first, const Integer& second,
                                       Integer & result, int_fast8_t comparing_signum_result) {
   if (result.signum == 0)   // skip setSignum() for zero, zero has correct signum = 0 in private Integer constructor
      return;
   if (0 == comparing_signum_result) {        // integers with identical signums
      if (true == Integer::is_absolute_value_greater(first, second)) 
         result.signum = first.signum;
      else 
         set_opposite_signum(result, second.signum);  
   }
   else {  // integers with different signums
      if (0 == first.signum)   // if minuend is zero (with signum zero), then signum's result is negate of subtrahend's signum
         set_opposite_signum(result, second.signum);
      else 
         result.signum = first.signum;
   }
}

Integer operator-(const Integer& first, const Integer& second) {
   Integer result;
   int_fast8_t comparing_signum_result = Integer::compare_signum(first, second);
   if (0 == comparing_signum_result)    // integers with identical sigmums
      result = Integer::subtract_absolute_values(first, second);
   else  // integers with different sigmums
      result = Integer::add_absolute_values(first, second);

   set_signum_subtracting_Non_Zero_result(first, second, result, comparing_signum_result);
   return result;
}

Integer operator*(const Integer& first, const Integer& second) {
   Integer result = Integer::multiply_absolute_values(first, second);
   if (result.signum == 0)   // skip set_signum() for zero, zero has correct signum = 0 in private Integer constructor
      return result;

   int_fast8_t comparing_signum_result = Integer::compare_signum(first, second);
   if (0 == comparing_signum_result)        // integers with identical signums
      result.signum = Integer::PLUS;
   else   // integers with different signums
      result.signum = Integer::MINUS;
   return result;
}

Integer operator%(const Integer& DIVIDEND, const Integer& DIVISOR) {
   if (true == DIVISOR.is_zero()) 
      throw Arithmetic_Error("Divisor can not be zero");
   Integer dividing_result     = Integer::divide_absolute_values(DIVIDEND, DIVISOR);
   Integer multiple_of_divisor = Integer::multiply_absolute_values(dividing_result, DIVISOR);
   Integer remainder           = Integer::subtract_absolute_values(DIVIDEND, multiple_of_divisor);
   if (remainder.signum == 0)   // skip set signum for zero, zero has correct signum = 0 in private Integer constructor
      return remainder;
   remainder.signum = DIVIDEND.signum;
   return remainder;
}

Integer operator/(const Integer& DIVIDEND, const Integer& DIVISOR) {
   if (true == DIVISOR.is_zero()) 
      throw Arithmetic_Error("Divisor can not be zero");   
   Integer result = Integer::divide_absolute_values(DIVIDEND, DIVISOR);
   if (result.signum == 0)    // skip setSignum() for zero, zero has correct signum = 0 in private Integer constructor
      return result;
   int_fast8_t comparing_signum_result = Integer::compare_signum(DIVIDEND, DIVISOR);
   if (0 == comparing_signum_result)        // integers with identical signums
      result.signum = Integer::PLUS;
   else   // integers with different signums
      result.signum = Integer::MINUS;
   return result;
}

constexpr short INCORRECT_INDEX = -99;

template<typename Container>
static short get_mismatch_value_index(const Container& ARRAY, const int fromIndex, const digit_type value) {
   for (short index = fromIndex; index < ARRAY.size(); index++)
      if (value != ARRAY[index]) 
         return index;
   return INCORRECT_INDEX;
}

array<digit_type, Integer::MAX_ARRAY_LENGTH> Integer::calculate_dividing(const array<digit_type, MAX_ARRAY_LENGTH> & DIVIDEND_ARRAY, 
               short current_dividend_index, const Integer & DIVISOR) {
   array<digit_type, MAX_ARRAY_LENGTH> result_array;
   //const short DIVISOR_DIGITS_LENGTH = DIVISOR_ARRAY.size() - DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX;
   vector<digit_type> temporary_dividend_array;
   Integer temporary_dividend;
   digit_type next_dividend_digit;
   for (; current_dividend_index < MAX_ARRAY_LENGTH; current_dividend_index++) {
      next_dividend_digit = DIVIDEND_ARRAY[current_dividend_index];
      temporary_dividend_array.push_back(next_dividend_digit);
      temporary_dividend.set_integer_array(temporary_dividend_array);
      
      if (true == is_absolute_value_less(temporary_dividend, DIVISOR))
         result_array[current_dividend_index] = 0;
      else
         multiply_divisor(result_array, current_dividend_index,
                          temporary_dividend, temporary_dividend_array, DIVISOR);
   }
   return result_array;                                                       
}

Integer Integer::divide_absolute_values(const Integer& DIVIDEND, const Integer& DIVISOR) {
   if (true == DIVISOR.is_zero()) 
      throw Arithmetic_Error("Divisor can not be zero");
   const array<digit_type, MAX_ARRAY_LENGTH> DIVIDEND_ARRAY = DIVIDEND.get_integer_array();
   short current_dividend_index = get_mismatch_value_index(DIVIDEND_ARRAY, 0, 0);
   const array<digit_type, MAX_ARRAY_LENGTH> DIVISOR_ARRAY  = DIVISOR.get_integer_array();
   const short DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX = get_mismatch_value_index(DIVISOR_ARRAY, 0, 0);
   
   if (0 > current_dividend_index || current_dividend_index > DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX) 
      return Integer();   // default constructor initialized by zero
   else {
      const array<digit_type, MAX_ARRAY_LENGTH> result_array = calculate_dividing(DIVIDEND_ARRAY, current_dividend_index, DIVISOR);
      return Integer(result_array);
   }
}
/*
static vector<digit_type> modify_dividend_array(const vector<digit_type> & temporary_dividend_array, const digit_type next_dividend_digit) {
   const short LENGTH = temporary_dividend_array.length + 1;
   temporary_dividend_array = Arrays.copyOf(temporary_dividend_array, LENGTH);
   temporary_dividend_array[LENGTH - 1] = next_dividend_digit;
   
   return temporary_dividend_array;
}
*/

void Integer::assign_remainder(const Integer & DIVIDEND, const Integer & MULTIPLE_OF_DIVISOR, 
                                       vector<digit_type> & dividend_array) {
   const Integer REMAINDER = subtract_absolute_values(DIVIDEND, MULTIPLE_OF_DIVISOR);
   const array<digit_type, MAX_ARRAY_LENGTH> REMAINDER_ARRAY  = REMAINDER.get_integer_array();
   const short REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX = get_mismatch_value_index(REMAINDER_ARRAY, 0, 0);
   //const short REMAINDER_DIGITS_LENGTH = MAX_ARRAY_LENGTH - REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX;
   
   //dividend_array = Arrays.copyOf(dividend_array, REMAINDER_DIGITS_LENGTH);
   //System.arraycopy(REMAINDER_ARRAY, REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX, 
   //                  dividend_array, 0, REMAINDER_DIGITS_LENGTH);
   dividend_array.assign(REMAINDER_ARRAY.begin() + REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX, REMAINDER_ARRAY.end());
}

void Integer::multiply_divisor(array<digit_type, MAX_ARRAY_LENGTH> & result_array, const short current_dividend_index,
       const Integer & DIVIDEND, vector<digit_type> & dividend_array, const Integer & DIVISOR) {
   Integer multiple_of_divisor = Integer(DIVISOR.get_integer_array());
   int_fast8_t multiple = 1;
   while (true == is_absolute_value_less(multiple_of_divisor, DIVIDEND)) {
      multiple_of_divisor = add_absolute_values(multiple_of_divisor, DIVISOR);
      multiple++;
   }
   if (true == is_absolute_value_greater(multiple_of_divisor, DIVIDEND)) {  // if true, remainder != 0
      multiple_of_divisor = subtract_absolute_values(multiple_of_divisor, DIVISOR);
      multiple--;
      assign_remainder(DIVIDEND, multiple_of_divisor, dividend_array);
   }
   else    // if multiple_of_divisor == DIVIDEND
      dividend_array = vector<digit_type>(); // empty vector
   
   if (multiple > 9) 
      throw runtime_error("Unexpected multiple greater than 9");
   result_array[current_dividend_index] = multiple;
}

Integer Integer::add_absolute_values(const Integer & first, const Integer & second) {
   array<digit_type, MAX_ARRAY_LENGTH> result_array;
   digit_type first_integer_digit;
   digit_type second_integer_digit;
   short sum_of_digits = 0;
   int_fast8_t carrying = 0;
   for (short index = first.integer_array.size() - 1; index >= 0; index--) {
      first_integer_digit  = first.integer_array[index];
      second_integer_digit = second.integer_array[index];
      sum_of_digits = first_integer_digit + second_integer_digit;
      sum_of_digits += carrying;
      result_array[index] = sum_of_digits % 10;
      carrying = sum_of_digits / 10 ;
   }
   if (carrying > 0) 
      throw Arithmetic_Error
            ("Arithmetic overflow while adding absolute values of " + string(first) + " and " + string(second));
   return Integer { result_array };
}

Integer Integer::subtract_absolute_values(const Integer & first, const Integer & second) {
   Integer minuend    = is_absolute_value_greater(first, second) ? first : second;
   Integer subtrahend = (&first == &minuend) ? second : first;
   array<digit_type, MAX_ARRAY_LENGTH> result_array;
   digit_type minuend_digit;
   digit_type subtrahend_digit;
   short difference_digits = 0;
   int_fast8_t carrying = 0;
   for (short index = first.integer_array.size() - 1; index >= 0; index--) {
      minuend_digit      = minuend.integer_array[index];
      subtrahend_digit   = subtrahend.integer_array[index];
      difference_digits  = minuend_digit - subtrahend_digit;
      difference_digits -= carrying;
      if (difference_digits < 0) {
         difference_digits += 10;
         carrying = 1;
      }
      else 
         carrying = 0;
      result_array[index] = difference_digits % 10;
   }
   return Integer{result_array};
}

inline static void check_multiplication_overflow(const int_fast8_t carrying, const Integer & first, const Integer & second) {
   if (carrying > 0) {
      throw Arithmetic_Error
            ("Arithmetic overflow while multiplying absolute values of " + string(first) + " and " + string(second));
   }
}

void Integer::detect_multiplication_overflow(const Integer & first, const Integer & second) {
   digit_type first_integer_digit;
   digit_type second_integer_digit;
   for (short first_index = 0; first_index < first.integer_array.size(); first_index++) {
      first_integer_digit = first[first_index];
      if (first_integer_digit > 0) {
         for (short second_index = 0; second_index < second.integer_array.size(); second_index++) {
            second_integer_digit = second[second_index];
            if (second_integer_digit > 0) {
               short first_magnitude_order  = first.integer_array.size() - 1 - first_index;
               short second_magnitude_order = second.integer_array.size() - 1 - second_index;
               if (first_magnitude_order + second_magnitude_order >= MAX_ARRAY_LENGTH) 
                  throw Arithmetic_Error
                     ("Arithmetic overflow while multiplying absolute value of " + string(first) + " and " + string(second));
            }
         }
      }
   }
}

static Integer sum_multiplication_results(array<Integer, Integer::MAX_ARRAY_LENGTH> & integers_array) {
   Integer result;
   for (short index = integers_array.size() - 1; index >= 0; index--)
      result = Integer::add_absolute_values(result, integers_array[index]);
   return result;
}

Integer Integer::multiply_absolute_values(const Integer & first, const Integer & second) {
   detect_multiplication_overflow(first, second);
   array< array<digit_type, MAX_ARRAY_LENGTH>, MAX_ARRAY_LENGTH > result_array;
   array<Integer, MAX_ARRAY_LENGTH> integers_array;
   digit_type first_integer_digit;
   digit_type second_integer_digit;
   short product = 0;
   int_fast8_t carrying = 0;
   digit_type magnitude_order = 0;
   short column;
   for (short first_index = MAX_ARRAY_LENGTH - 1; first_index >= 0; first_index--) {
      carrying = 0;
      first_integer_digit = first[first_index];
      for (short second_index = MAX_ARRAY_LENGTH - 1; second_index >= 0; second_index--) {
         column = second_index - magnitude_order;
         if (column < 0) {
            check_multiplication_overflow(carrying, first, second);
            break;
         }
         second_integer_digit = second[second_index];
         product = first_integer_digit * second_integer_digit;
         product += carrying;
         result_array[first_index][column] = product % 10;
         carrying = product / 10;
      }
      integers_array[first_index] = Integer(result_array[first_index]);
      magnitude_order++;
   }
   check_multiplication_overflow(carrying, first, second);
   return sum_multiplication_results(integers_array);
}

Integer::operator string () const {
   string result;
   if (signum != NEUTRAL)
      result += string(1, signum);
   digit_type number;
   digit_type skipped = 0;
   short index = integer_parsing::skip_leading_integers(this->integer_array, skipped);
   for (; index < (*this).integer_array.size(); index++) {
      number = integer_array[index];
      result += to_string(number);
   }
   return result.empty() ? "0" : result;
}

}
