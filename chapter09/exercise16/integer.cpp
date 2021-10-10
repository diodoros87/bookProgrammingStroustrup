#include "integer.hpp"

#include <cassert>
#include <string>
#include <cstdint>
#include <climits>
#include <vector>

namespace integer_space {
   
static inline void validate(const char signum) {
   if (PLUS_SIGNUM != signum && MINUS_SIGNUM != signum && NEUTRAL_SIGNUM != signum )
      throw invalid_argument("Accepted signum: '" + string(1, MINUS_SIGNUM) + "', '" + string(1, NEUTRAL_SIGNUM) 
         + "', " string(1, PLUS_SIGNUM) + "'\n");
}

/*
template <typename T>
static inline void validate_size(const vector<digit_type> & vec) {
   const size_t VEC_SIZE = vec.size();
   if (0 == VEC_SIZE)
      throw invalid_argument("Requirement: vector.length > 0");
   if (MAX_ARRAY_LENGTH < VEC_SIZE)
      throw invalid_argument("Requirement: vector.length <= " + to_string(MAX_ARRAY_LENGTH));
}

template <typename Container>
static inline void validate_size(const Container& TABLE) {
   const size_t SIZE = TABLE.size();
   //if (0 == SIZE)
   //   throw invalid_argument("Requirement: length > 0");
   if (MAX_ARRAY_LENGTH < SIZE)
      throw invalid_argument("Requirement: length <= " + to_string(MAX_ARRAY_LENGTH));
}
*/
template <typename Container>   // private
void Integer::validate(const Container& TABLE, const char signum) {
   //validate_size(TABLE);
   const size_t SIZE = TABLE.size();
   validate(signum);
   digit_type number;
   int_fast8_t zeros = 0;
   for (size_t source_index = SIZE - 1; source_index >= 0 ; source_index--) {
      number = vec[source_index];
      if (number == 0) 
         zeros++;
      else if (number < 0 || number > 9)
         throw invalid_argument("Requirement: in array must be only integers from 0 to 9, but input number is: "
            + to_string(number));
   }
   if (zeros == SIZE && signum != NEUTRAL_SIGNUM) 
      throw invalid_argument("Requirement: signum must be zero for array with only zeros");
   if (zeros < SIZE && signum == NEUTRAL_SIGNUM) 
      throw invalid_argument("Requirement: signum can not be zero for array with element other than zero");
   size_t dest_index = MAX_ARRAY_LENGTH - 1;
   for (size_t source_index = SIZE - 1; source_index >= 0 ; source_index--) {
      number = integer_array[source_index];
      this.integer_array[dest_index] = number;
      dest_index--;
   }
   this->signum = signum;
}
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
   size_t dest_index = MAX_ARRAY_LENGTH - 1;
   for (size_t source_index = MAX_ARRAY_LENGTH - 1; source_index >= 0 ; source_index--) {
      number = ARRAY[source_index];
      if (number == 0) 
         zeros++;
      else if (number < 0 || number > 9)
         throw invalid_argument("Requirement: in array must be only integers from 0 to 9, but input number is: "
            + to_string(number));
      integer_array[dest_index] = number;
      dest_index--;
   }
   this->signum = zeros == MAX_ARRAY_LENGTH ? NEUTRAL_SIGNUM : PLUS_SIGNUM;
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

void Integer::set_integer_array(const vector<digit_type> & VEC) {  // assume that integer_array is positive number, change of signum is allowed by setSignum()
   if (MAX_ARRAY_LENGTH < VEC.size())
      throw invalid_argument("Requirement: elements <= " + to_string(MAX_ARRAY_LENGTH));
   this->integer_array->fill(0);  // not call reset_number_to_zero() due to change signum (in set_integer_array() save previous signum)
   digit_type number;
   size_t dest_index = MAX_ARRAY_LENGTH - 1;
   for (size_t source_index = VEC.size() - 1; source_index >= 0 ; source_index--) {
      number = VEC[source_index];
      if (number < 0 || number > 9) {
         reset_number_to_zero();    // similar to constructor, reset to zero
         throw invalid_argument("Requirement: in array must be only integers from 0 to 9, but input number is: "
            + to_string(number));
      }
      this->integer_array[dest_index] = number;
      dest_index--;
   }
   if (this->signum == NEUTRAL_SIGNUM && false == is_zero())
      this->signum = PLUS_SIGNUM;
   else if (this->signum != NEUTRAL_SIGNUM && is_zero()) 
      this->signum = NEUTRAL_SIGNUM;
}

void Integer::parse(const string& STR) {
   Integer_Parsing::validateString(STR);
   reset_number_to_zero();
   short iteration_stop_index = 0;
   if (STR[0] == PLUS_SIGNUM || STR[0] == MINUS_SIGNUM) {
      this->signum = STR[0];
      iteration_stop_index++;
   }
   for (size_t string_index = STR.length() - 1, array_index = MAX_ARRAY_LENGTH - 1;
                        string_index >= iteration_stop_index; string_index--, array_index--) 
      this->integer_array[array_index] = STR[string_index] - '0';

   if (is_zero()) 
      this->signum = NEUTRAL_SIGNUM;
   else if (this->signum != MINUS_SIGNUM)   // string without leading char '+' or '-' is '+'
      this->signum = PLUS_SIGNUM;
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
   if (first.signum == PLUS_SIGNUM) {
      if (second.signum == PLUS_SIGNUM)
         return 0;
      else   
         return +1;
   }
   else if (first.signum == NEUTRAL_SIGNUM) {
      switch (second.signum) {
         case PLUS_SIGNUM:
            return -1;
         case NEUTRAL_SIGNUM:
            return 0;
         case MINUS_SIGNUM:
            return +1;
      }
   }
   else {  // first->signum  == MINUS_SIGNUM
      if (second.signum == MINUS_SIGNUM) 
         return 0;
      else 
         return -1;
   }
}

bool operator==(const Integer& first, const Integer& second) {
   for (short index = 0; index < first.integer_array.size(); index++) 
      if (first.integer_array[index] != second.integer_array[index]) 
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

   assert(first.signum == second.signum);
   assert(first.signum != 0);
   int_fast8_t identical_integers_signum = first.signum;
   
   if (identical_integers_signum > 0) 
      return is_absolute_value_greater(first, second);
   else if (identical_integers_signum < 0) 
      return is_absolute_value_less(first, second);
   else  // identical_integers_signum == 0
      throw runtime_error("signum of integer = 0");
}

bool Integer::is_absolute_value_greater(const Integer& first, const Integer& second) {
   digit_type first_digit;
   digit_type second_digit;
   for (byte index = 0; index < first.integer_array.length; index++) {
      first_digit  = first.integer_array[index];
      second_digit = second.integer_array[index];
      if (first_digit > second_digit) 
         return true;
      else if (first_digit < second_digit) 
         return false;
   }
   return false;
}

bool Integer:: is_absolute_value_less(const Integer& first, const Integer& second) {
   digit_type first_digit;
   digit_type second_digit;
   for (byte index = 0; index < first.integer_array.length; index++) {
      first_digit  = first.integer_array[index];
      second_digit = second.integer_array[index];
      if (first_digit < second_digit) 
         return true;
      else if (first_digit > second_digit) 
         return false;
   }
   return false;
}

bool operator<(const Integer& first, const Integer& second) {
   int_fast8_t comparing_signum_result = Integer::compare_signum(first, second);
   if (-1 == comparing_signum_result) 
      return true;
   else if (+1 == comparing_signum_result) 
      return false;
   // after calling Integer.compare_signum recognize both integers with identical nonzero signum

   assert(first.signum == second.signum);
   assert(first.signum != 0);
   int_fast8_t identical_integers_signum = first.signum;

   if (identical_integers_signum < 0) 
      return is_absolute_value_greater(first, second);
   else if (identical_integers_signum > 0) 
      return is_absolute_value_less(first, second);
   else {  // identical_integers_signum == 0
      throw runtime_error("signum of integer = 0");
}

static void set_signum_adding_Non_Zero_result(const Integer& first, const Integer& second,
                                       Integer & result, int_fast8_t comparing_signum_result) {
   if (result.signum == 0)   // skip setSignum() for zero, zero has correct signum = 0 in private Integer(byte[]) constructor
      return;
   if (0 == comparing_signum_result)        // integers with identical signums
      result.signum = first.signum;
   else {  // integers with different signums
      if (true == is_absolute_value_greater(first, second))
         result.signum = first.signum;
      else 
         result.signum = second.signum;
   }
}

Integer operator+(const Integer& first, const Integer& second) {
   Integer result;
   byte comparing_signum_result = Integer::compare_signum(first, second);
   if (0 == comparing_signum_result)     // integers with identical signums
      result = Integer::add_absolute_values(first, second);
   else  // integers with different signums
      result = Integer::subtract_absolute_values(first, second);

   set_signum_adding_Non_Zero_result(first, second, result, comparing_signum_result);

   return result;
}

static char get_opposite_signum(const char signum) {
   switch(signum) {
      case MINUS_SIGNUM:
         return PLUS_SIGNUM;
         break;
      case NEUTRAL_SIGNUM:
         return NEUTRAL_SIGNUM;
         break;
      case PLUS_SIGNUM:
         return MINUS_SIGNUM;
      default:
         throw runtime_error("Unrecognized signum");
   }
}

Integer operator-(const Integer& integer) {
   char signum = get_opposite_signum(integer.signum);
   return Integer(integer.integer_array, signum);
}

inline static void set_opposite_signum(Integer & result, const char signum) {
   const char opposite = get_opposite_signum(second.signum);
   result.signum = opposite;
}

static void set_signum_subtracting_Non_Zero_result(const Integer& first, const Integer& second,
                                       Integer & result, int_fast8_t comparing_signum_result) {
   if (result.signum == 0)   // skip setSignum() for zero, zero has correct signum = 0 in private Integer(byte[]) constructor
      return;
   if (0 == comparing_signum_result) {        // integers with identical signums
      if (true == is_absolute_value_greater(first, second)) 
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
   byte comparing_signum_result = Integer::compare_signum(first, second);
   if (0 == comparing_signum_result)    // integers with identical sigmums
      result = Integer::subtract_absolute_values(first, second);
   else  // integers with different sigmums
      result = Integer::add_absolute_values(first, second);

   set_signum_subtracting_Non_Zero_result(first, second, result, comparing_signum_result);
   return result;
}

Integer operator*(const Integer& first, const Integer& second) {
   Integer result = Integer::multiply_absolute_values(first, second);
   if (result.signum == 0)   // skip set_signum() for zero, zero has correct signum = 0 in private Integer(byte[]) constructor
      return result;

   int_fast8_t comparing_signum_result = Integer::compare_signum(first, second);
   if (0 == comparing_signum_result)        // integers with identical signums
      result.signum = PLUS_SIGNUM;
   else   // integers with different signums
      result.signum = MINUS_SIGNUM;
   return result;
}

Integer Integer::remainder(const Integer& DIVIDEND, const Integer& DIVISOR) {
   if (true == DIVISOR.is_zero()) 
      throw Arithmetic_Error("Divisor can not be zero");
   Integer dividing_result     = divide_absolute_values(DIVIDEND, DIVISOR);
   Integer multiple_of_divisor = multiply_absolute_values(dividing_result, DIVISOR);
   Integer remainder           = subtract_absolute_values(DIVIDEND, multiple_of_divisor);
   if (remainder.signum == 0) {   // skip setSignum() for zero, zero has correct signum = 0 in private Integer(byte[]) constructor
      return remainder;
   remainder.signum = DIVIDEND.signum;
   return remainder;
}

Integer operator/(const Integer& dividend, const Integer& divisor) {
   if (true == DIVISOR.is_zero()) 
      throw Arithmetic_Error("Divisor can not be zero");   
   Integer result = Integer::divide_absolute_values(dividend, divisor);
   if (result.signum == 0)    // skip setSignum() for zero, zero has correct signum = 0 in private Integer(byte[]) constructor
      return result;
   int_fast8_t comparing_signum_result = Integer::compare_signum(dividend, divisor);
   if (0 == comparing_signum_result)        // integers with identical signums
      result.signum = PLUS_SIGNUM;
   else   // integers with different signums
      result.signum = MINUS_SIGNUM;
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

Integer Integer::divide_absolute_values(const Integer& DIVIDEND, const Integer& DIVISOR) {
   if (true == DIVISOR.is_zero()) 
      throw Arithmetic_Error("Divisor can not be zero");
   const array<digit_type, MAX_ARRAY_LENGTH> DIVIDEND_ARRAY = DIVIDEND.get_integer_array();
   short current_dividend_index = get_mismatch_value_index(DIVIDEND_ARRAY, 0, (byte)0);
   const array<digit_type, MAX_ARRAY_LENGTH> DIVISOR_ARRAY  = DIVISOR.get_integer_array();
   const short DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX = get_mismatch_value_index(DIVISOR_ARRAY, 0, 0);
   
   if (0 > current_dividend_index || current_dividend_index > DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX) 
      return Integer();   // default constructor initialized by zero
   else {
      const array<digit_type, MAX_ARRAY_LENGTH> result_array = calculate_dividing(DIVIDEND_ARRAY, current_dividend_index, DIVISOR, DIVISOR_ARRAY, 
                                                DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX);
      return new Integer(result_array);
   }
}
/*
static vector<digit_type> modify_dividend_array(const vector<digit_type> & temporary_dividend_array, const digit_type next_dividend_digit) {
   const size_t LENGTH = temporary_dividend_array.length + 1;
   temporary_dividend_array = Arrays.copyOf(temporary_dividend_array, LENGTH);
   temporary_dividend_array[LENGTH - 1] = next_dividend_digit;
   
   return temporary_dividend_array;
}
*/

static void multiply_divisor(array<digit_type, MAX_ARRAY_LENGTH> & result_array, const short current_dividend_index,
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
      dividend_array = assign_remainder(DIVIDEND, multiple_of_divisor, dividend_array);
   }
   else    // if multiple_of_divisor == DIVIDEND
      dividend_array = vector<digit_type>; // empty vector
   
   if (multiple > 9) 
      throw runtime_error("Unexpected multiple greater than 9");
   result_array[current_dividend_index] = multiple;
}

static array<digit_type, MAX_ARRAY_LENGTH> calculate_dividing(const array<digit_type, MAX_ARRAY_LENGTH> & DIVIDEND_ARRAY, 
               const short current_dividend_index, const Integer & DIVISOR,
               const array<digit_type, MAX_ARRAY_LENGTH> & DIVISOR_ARRAY, const short DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX) {
   array<digit_type, MAX_ARRAY_LENGTH> result_array;
   const short DIVISOR_DIGITS_LENGTH = DIVISOR_ARRAY.size() - DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX;
   vector<digit_type> temporary_dividend_array;
   Integer temporary_dividend;
   digit_type next_dividend_digit;
   for (; current_dividend_index < MAX_ARRAY_LENGTH; current_dividend_index++) {
      next_dividend_digit = DIVIDEND_ARRAY[current_dividend_index];
      temporary_dividend_array.push_back(next_dividend_digit);
      temporary_dividend.set_integer_array(temporary_dividend_array);
      
      if (true == Integer::is_absolute_value_less(temporary_dividend, DIVISOR))
         result_array[current_dividend_index] = 0;
      else
         multiply_divisor(result_array, current_dividend_index,
                          temporary_dividend, temporary_dividend_array, DIVISOR);
   }
   return result_array;                                                       
}

static void assign_remainder(const Integer & DIVIDEND, const Integer & MULTIPLE_OF_DIVISOR, 
                                       vector<digit_type> & dividend_array) {
   const Integer REMAINDER = Integer::subtract_absolute_values(DIVIDEND, MULTIPLE_OF_DIVISOR);
   const array<digit_type, MAX_ARRAY_LENGTH> REMAINDER_ARRAY  = REMAINDER.get_integer_array();
   const short REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX = get_mismatch_value_index(REMAINDER_ARRAY, 0, 0);
   const short REMAINDER_DIGITS_LENGTH = MAX_ARRAY_LENGTH - REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX;
   
   //dividend_array = Arrays.copyOf(dividend_array, REMAINDER_DIGITS_LENGTH);
   //System.arraycopy(REMAINDER_ARRAY, REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX, 
   //                  dividend_array, 0, REMAINDER_DIGITS_LENGTH);
   dividend_array.assign(REMAINDER_ARRAY.begin() + REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX, REMAINDER_ARRAY.end());
}

Integer Integer::add_absolute_values(const Integer & first, const Integer & second) {
   array<digit_type, MAX_ARRAY_LENGTH> result_array;
   digit_type first_integer_digit;
   digit_type second_integer_digit;
   short sum_of_digits = 0;
   digit_type carrying = 0;
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
            ("Arithmetic overflow while adding absolute values of " + string(first) " and " + string(second));
   return Integer{result_array};
}

Integer Integer::subtract_absolute_values(const Integer & first, const Integer & second) {
   Integer minuend    = is_absolute_value_greater(first, second) ? first : second;
   Integer subtrahend = (&first == &minuend) ? second : first;
   array<digit_type, MAX_ARRAY_LENGTH> result_array;
   digit_type minuend_digit;
   digit_type subtrahend_digit;
   short difference_digits = 0;
   digit_type carrying = 0;
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

inline static void detect_multiplication_overflow(const int carrying, const Integer & first, const Integer & second) {
   if (carrying > 0) {
      throw Arithmetic_Error
            ("Arithmetic overflow while multiplying absolute values of " + string(first) " and " + string(second));
   }
}

static void Integer::detect_multiplication_overflow(const Integer & first, const Integer & second) {
   digit_type first_integer_digit;
   digit_type second_integer_digit;
   for (short first_index = 0; first_index < first.integer_array.size(); first_index++) {
      first_integer_digit = first.integer_array[first_index];
      if (first_integer_digit > 0) {
         for (short second_index = 0; second_index < second.integer_array.size(); second_index++) {
            second_integer_digit = second.integer_array[second_index];
            if (second_integer_digit > 0) {
               short first_magnitude_order  = first.integer_array.size() - 1 - first_index;
               short second_magnitude_order = second.integer_array.size() - 1 - second_index;
               if (first_magnitude_order + second_magnitude_order >= MAX_ARRAY_LENGTH) 
                  throw Arithmetic_Error
                     ("Arithmetic overflow while multiplying absolute value of " + string(first) " and " + string(second));
            }
         }
      }
   }
}

static Integer sum_multiplication_results(array<digit_type, MAX_ARRAY_LENGTH> & integers_array) {
   Integer result;
   for (short index = integers_array.size() - 1; index >= 0; index--)
      result = add_absolute_values(result, integers_array[index]);
   return result;
}

Integer Integer:: multiply_absolute_values(const Integer & first, const Integer & second) {
   detect_multiplication_overflow(first, second);
   array< array<digit_type, MAX_ARRAY_LENGTH> > result_array;
   array<digit_type, MAX_ARRAY_LENGTH> integers_array;
   digit_type first_integer_digit;
   digit_type second_integer_digit;
   short product = 0;
   digit_type carrying = 0;
   digit_type magnitude_order = 0;
   short column;
   for (int first_index = first.integer_array.length - 1; first_index >= 0; first_index--) {
      carrying = 0;
      first_integer_digit = first.integer_array[first_index];
      for (int second_index = second.integer_array.length - 1; second_index >= 0; second_index--) {
         column = second_index - magnitude_order;
         if (column < 0) {
            detect_multiplication_overflow(carrying, first, second);
            break;
         }
         second_integer_digit = second.integer_array[second_index];
         product = first_integer_digit * second_integer_digit;
         product += carrying;
         result_array[first_index][column] = product % 10;
         carrying = product / 10;
      }
      integers_array[first_index] = Integer(result_array[first_index]);
      magnitude_order++;
   }
   detect_multiplication_overflow(carrying, first, second);
   return sum_multiplication_results(integers_array);
}

operator string() const {
   string result;
   if (signum != NEUTRAL_SIGNUM)
      result += string(1, signum);
   digit_type number;
   digit_type skipped = 0;
   short index = Integer_Parsing::skip_leading_integers(this.integer_array, skipped);
   for (; index < MAX_ARRAY_LENGTH; index++) {
      number = *this.integer_array[index];
      result += to_string(number);
   }
   return result;
}

}
