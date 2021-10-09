#include "integer.h"

#include <cassert>
#include <string>
#include <cstdint>
#include <climits>

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
*/
template <typename Container>
static inline void validate_size(const Container& TABLE) {
   const size_t SIZE = TABLE.size();
   if (0 == SIZE)
      throw invalid_argument("Requirement: length > 0");
   if (MAX_ARRAY_LENGTH < SIZE)
      throw invalid_argument("Requirement: length <= " + to_string(MAX_ARRAY_LENGTH));
}

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
Integer::Integer(const vector<digit_type> & vec) {    // private
   validate_size(vec);
   const size_t VEC_SIZE = vec.size();
   digit_type number;
   int_fast8_t zeros = 0;
   size_t dest_index = MAX_ARRAY_LENGTH - 1;
   for (size_t source_index = VEC_SIZE - 1; source_index >= 0 ; source_index--) {
      number = vec[source_index];
      if (number == 0) 
         zeros++;
      else if (number < 0 || number > 9)
         throw invalid_argument("Requirement: in array must be only integers from 0 to 9, but input number is: "
            + to_string(number));
      integer_array[dest_index] = number;
      dest_index--;
   }
   this->signum = zeros == VEC_SIZE ? NEUTRAL_SIGNUM : PLUS_SIGNUM;
}

/* own copy constructor or default  copy constructor ?
public Integer(Integer integer) {
   this(integer.integerArray, integer.signum);
}

void set(const Integer& integer) {
   if (null == integer) {
      throw new NullPointerException("Requirement: reference to Integer can not be null");
   }

   System.arraycopy(integer.integerArray, 0, this.integerArray, 0, integerArray.length);
   this.signum = integer.signum;
}
*/

void Integer::set_integer_array(const vector<digit_type> & VEC) {  // assume that integerArray is positive number, change of signum is allowed by setSignum()
   validate_size(VEC);
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
      this->integerArray[dest_index] = number;
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

static void get_opposite_signum(const char signum) {
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
      throw invalid_argument("Divisor can not be zero");
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
      throw invalid_argument("Divisor can not be zero");   
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
      throw invalid_argument("Divisor can not be zero");
   const array<digit_type, MAX_ARRAY_LENGTH> DIVIDEND_ARRAY = DIVIDEND.get_integer_array();
   short current_dividend_index = get_mismatch_value_index(DIVIDEND_ARRAY, 0, (byte)0);
   const array<digit_type, MAX_ARRAY_LENGTH> DIVISOR_ARRAY  = DIVISOR.get_integer_array();
   final int DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX = get_mismatch_value_index(DIVISOR_ARRAY, 0, (byte)0);
   
   if (0 > current_dividend_index || current_dividend_index > DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX) 
      return Integer();   // default constructor initialized by zero
   else {
      const array<digit_type, MAX_ARRAY_LENGTH> resultArray = calculate_dividing(DIVIDEND_ARRAY, current_dividend_index, DIVISOR, DIVISOR_ARRAY, 
                                                DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX);
      return new Integer(resultArray);
   }
}

static array<digit_type, MAX_ARRAY_LENGTH> calculate_dividing(const array<digit_type, MAX_ARRAY_LENGTH> & DIVIDEND_ARRAY, 
               const short current_dividend_index, const Integer & DIVISOR,
               const array<digit_type, MAX_ARRAY_LENGTH> & DIVISOR_ARRAY, const short DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX) {
   byte[] resultArray            = array<digit_type, MAX_ARRAY_LENGTH>;
   const short DIVISOR_DIGITS_LENGTH = DIVISOR_ARRAY.size() - DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX;
   vector<digit_type> temporary_dividend_array;
   Integer temporaryDividend;
   digit_type next_dividend_digit;
                     
   while (current_dividend_index < MAX_ARRAY_LENGTH) {
      next_dividend_digit = DIVIDEND_ARRAY[current_dividend_index];
      temporary_dividend_array = modifyDividendArray(temporary_dividend_array, next_dividend_digit);
      temporaryDividend.set_integer_array(temporary_dividend_array);
      
      if (true == Integer.is_absolute_value_less(temporaryDividend, DIVISOR)) {
         resultArray[current_dividend_index] = 0;
      }
      else {
         temporary_dividend_array = multiplyDivisor(resultArray, current_dividend_index,
                                             temporaryDividend, temporary_dividend_array, DIVISOR);
      }
      
      current_dividend_index++;
   }
                                                         
                                                         
   return resultArray;                                                       
}

private static byte[] modifyDividendArray(byte[] temporary_dividend_array, byte next_dividend_digit) {
   final int LENGTH = temporary_dividend_array.length + 1;
   temporary_dividend_array = Arrays.copyOf(temporary_dividend_array, LENGTH);
   temporary_dividend_array[LENGTH - 1] = next_dividend_digit;
   
   return temporary_dividend_array;
}

private static byte[] multiplyDivisor(byte[] resultArray, int current_dividend_index,
                                       final Integer DIVIDEND, byte[] dividendArray,
                                       final Integer DIVISOR) throws Exception {
   Integer multiple_of_divisor = new Integer(DIVISOR.get_integer_array());
   
   int multiple = 1;
   
   while (true == Integer.is_absolute_value_less(multiple_of_divisor, DIVIDEND)) {
      multiple_of_divisor = add_absolute_values(multiple_of_divisor, DIVISOR);
      multiple++;
   }
   
   if (true == Integer.is_absolute_value_greater(multiple_of_divisor, DIVIDEND)) {  // if true, remainder != 0
      multiple_of_divisor = subtract_absolute_values(multiple_of_divisor, DIVISOR);
      multiple--;
      dividendArray = assignRemainder(DIVIDEND, multiple_of_divisor, dividendArray);
   }
   else {    // if Integer.isEqualTo(multiple_of_divisor, DIVIDEND)
      final byte[] EMPTY_ARRAY = new byte[0];
      dividendArray = Arrays.copyOf(EMPTY_ARRAY, 0);
   }
   
   if (multiple > 9) {
      throw new ArithmeticException("Unexpected multiple greater than 9");
   }
   
   resultArray[current_dividend_index] = (byte)multiple;
   
   return dividendArray;
}

private static byte[] assignRemainder(final Integer DIVIDEND, final Integer MULTIPLE_OF_DIVISOR, 
                                       byte[] dividendArray) {
   final Integer REMAINDER = Integer.subtract_absolute_values(DIVIDEND, MULTIPLE_OF_DIVISOR);
   final byte[] REMAINDER_ARRAY  = REMAINDER.get_integer_array();
   final int REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX = ArraysOperations.get_mismatch_value_index(REMAINDER_ARRAY, 0, (byte)0);
   final int REMAINDER_DIGITS_LENGTH = REMAINDER_ARRAY.length - REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX;
   
   dividendArray = Arrays.copyOf(dividendArray, REMAINDER_DIGITS_LENGTH);
   System.arraycopy(REMAINDER_ARRAY, REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX, 
                     dividendArray, 0, REMAINDER_DIGITS_LENGTH);
                     
   return dividendArray;
}

public static Integer add_absolute_values(Integer first, Integer second) {
   byte[] resultArray = new byte[MAX_ARRAY_LENGTH];
   int firstIntegerDigit;
   int secondIntegerDigit;
   int sumOfDigits = 0;
   int carrying = 0;
   for (int index = first.integerArray.length - 1; index >= 0; index--) {
      firstIntegerDigit  = first.integerArray[index];
      secondIntegerDigit = second.integerArray[index];
      sumOfDigits = firstIntegerDigit + secondIntegerDigit;
      sumOfDigits += carrying;
      
      resultArray[index] = (byte)(sumOfDigits % 10);
      carrying = sumOfDigits / 10 ;
   }

   if (carrying > 0) {
      throw new ArithmeticException
               (String.format("Arithmetic overflow while adding absolute value of %s and %s", first , second));
   }

   Integer result = new Integer(resultArray);
   return result;
}

public static Integer subtract_absolute_values(Integer first, Integer second) {
   Integer minuend    = getMaxOfAbsoluteValues(first, second);
   Integer subtrahend = (first == minuend) ? second : first;

   byte[] resultArray = new byte[MAX_ARRAY_LENGTH];
   int minuendDigit;
   int subtrahendDigit;
   int differenceOfDigits = 0;
   int carrying = 0;
   for (int index = first.integerArray.length - 1; index >= 0; index--) {
      minuendDigit    = minuend.integerArray[index];
      subtrahendDigit = subtrahend.integerArray[index];
      differenceOfDigits  = minuendDigit - subtrahendDigit;
      differenceOfDigits -= carrying;
      
      if (differenceOfDigits < 0) {
         differenceOfDigits += 10;
         carrying = 1;
      }
      else {
         carrying = 0;
      }
      
      resultArray[index] = (byte)(differenceOfDigits % 10);
   }

   Integer result = new Integer(resultArray);
   return result;
}

public static Integer multiply_absolute_values(Integer first, Integer second) {
   detectMultiplicationArithmeticOverflow(first, second);

   byte[][] resultArray = new byte[MAX_ARRAY_LENGTH][MAX_ARRAY_LENGTH];
   Integer[] hugeIntegersArray = new Integer[MAX_ARRAY_LENGTH];
   int firstIntegerDigit;
   int secondIntegerDigit;
   int productOfDigits = 0;
   int carrying = 0;
   int orderOfMagnitude = 0;
   int columnInResultArray;

   for (int firstIndex = first.integerArray.length - 1; firstIndex >= 0; firstIndex--) {
      carrying = 0;
      firstIntegerDigit = first.integerArray[firstIndex];

      for (int secondIndex = second.integerArray.length - 1; secondIndex >= 0; secondIndex--) {
         columnInResultArray = secondIndex - orderOfMagnitude;
         if (columnInResultArray < 0) {
            detectMultiplicationArithmeticOverflow(carrying, first, second);
            break;
         }

         secondIntegerDigit = second.integerArray[secondIndex];
         productOfDigits = firstIntegerDigit * secondIntegerDigit;
         productOfDigits += carrying;

         resultArray[firstIndex][columnInResultArray] = (byte)(productOfDigits % 10);
         carrying = productOfDigits / 10;
      }


      hugeIntegersArray[firstIndex] = new Integer(resultArray[firstIndex]);
      orderOfMagnitude++;
   }

   detectMultiplicationArithmeticOverflow(carrying, first, second);

   Integer result = sumOfMultiplicationResults(hugeIntegersArray);

   return result;
}

private static void detectMultiplicationArithmeticOverflow(int carrying, Integer first, Integer second) {
   if (carrying > 0) {
      throw new ArithmeticException
            (String.format("Arithmetic overflow while multiplying absolute value of %s and %s", first , second));
   }
}

private static Integer sumOfMultiplicationResults(Integer[] hugeIntegersArray) {
   Integer result = new Integer();
   for (int index = hugeIntegersArray.length - 1; index >= 0; index--) {
      result = add_absolute_values(result, hugeIntegersArray[index]);
   }

   return result;
}

private static Integer getMaxOfAbsoluteValues(Integer first, Integer second) {
   boolean firstGreaterThanSecond = is_absolute_value_greater(first, second);
   if (true == firstGreaterThanSecond) {
      return first;
   }
   else {
      return second;
   }
}

public static void detectMultiplicationArithmeticOverflow(Integer first, Integer second) {
   int firstIntegerDigit;
   int secondIntegerDigit;

   for (int firstIndex = 0; firstIndex < first.integerArray.length; firstIndex++) {
      firstIntegerDigit = first.integerArray[firstIndex];
      if (firstIntegerDigit > 0) {

         for (int secondIndex = 0; secondIndex < second.integerArray.length; secondIndex++) {
            secondIntegerDigit = second.integerArray[secondIndex];

            if (secondIntegerDigit > 0) {
               int firstOrderOfMagnitude  = first.integerArray.length - 1 - firstIndex;
               int secondOrderOfMagnitude = second.integerArray.length - 1 - secondIndex;

               if (firstOrderOfMagnitude + secondOrderOfMagnitude >= MAX_ARRAY_LENGTH) {
                  throw new ArithmeticException
                     (String.format("Arithmetic overflow while multiplying absolute value of %s and %s", first , second));
               }
            }
         }

      }
   }
}

public void negate() {
   this.signum = (byte)(-this.signum);
}

public static Integer negate(Integer integer) {
   byte negateSignum = (byte)(-integer.signum);

   return new Integer(integer.integerArray, negateSignum);
}

@Override
public String toString() {
   String result = "";

   if (this.signum > 0) {
      result += "+";
   } else if (this.signum < 0) {
      result += "-";
   }

   Byte number;
   byte skippedValue = 0;
   byte index = Integer_Parsing.skipLeadingIntegersWithValue(this.integerArray, skippedValue);

   while (index < MAX_ARRAY_LENGTH) {
      number = this.integerArray[index];
      result += number.toString();
      index++;
   }

   return result;
}
}
