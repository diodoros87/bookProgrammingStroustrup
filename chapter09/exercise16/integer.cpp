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
template <typename T>
static inline void validate_size(const T& TABLE) {
   const size_t SIZE = TABLE.size();
   if (0 == SIZE)
      throw invalid_argument("Requirement: length > 0");
   if (MAX_ARRAY_LENGTH < SIZE)
      throw invalid_argument("Requirement: length <= " + to_string(MAX_ARRAY_LENGTH));
}

template <typename T>   // private
void Integer::validate(const T& TABLE, const char signum) {
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

public void set_integer_array(const vector<digit_type> & VEC) {  // assume that integerArray is positive number, change of signum is allowed by setSignum()
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

public void parse(const string& STR) {
   Integer_Parsing::validateString(STR);
   reset_number_to_zero();
   short iterationStopIndex = 0;
   if (STR[0] == PLUS_SIGNUM || STR[0] == MINUS_SIGNUM) {
      this->signum = STR[0];
      iterationStopIndex++;
   }
   for (size_t string_index = STR.length() - 1, array_index = MAX_ARRAY_LENGTH - 1;
                        string_index >= iterationStopIndex; string_index--, array_index--) 
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
   if (first->signum == PLUS_SIGNUM) {
      if (second->signum == PLUS_SIGNUM)
         return 0;
      else   
         return +1;
   }
   else if (first->signum == NEUTRAL_SIGNUM) {
      switch (second->signum) {
         case PLUS_SIGNUM:
            return -1;
         case NEUTRAL_SIGNUM:
            return 0;
         case MINUS_SIGNUM:
            return +1;
      }
   }
   else {  // first->signum  == MINUS_SIGNUM
      if (second->signum == MINUS_SIGNUM) 
         return 0;
      else 
         return -1;
   }
}

public boolean isEqualTo(Integer integer) {
   return Integer.isEqualTo(this, integer);
}

public static boolean isEqualTo(Integer first, Integer second) {
   byte firstIntegerDigit;
   byte secondIntegerDigit;
   for (byte index = 0; index < first.integerArray.length; index++) {
      firstIntegerDigit  = first.integerArray[index];
      secondIntegerDigit = second.integerArray[index];

      if (firstIntegerDigit != secondIntegerDigit) {
         return false;
      }
   }

   byte comparingSignumResult = Integer.compare_signum(first, second);
   if (0 != comparingSignumResult) {
      return false;
   }

   //assert(0 == comparingSignumResult);
   return true;
}

public boolean isNotEqualTo(Integer integer) {
   return Integer.isNotEqualTo(this, integer);
}

public static boolean isNotEqualTo(Integer first, Integer second) {
   boolean notEquality = ! isEqualTo(first, second);

   return notEquality;
}

public boolean isGreaterThan(Integer integer) throws Exception {
   return Integer.isGreaterThan(this, integer);
}

public static boolean isGreaterThan(Integer first, Integer second) throws Exception {
   byte comparingSignumResult = Integer.compare_signum(first, second);
   if (-1 == comparingSignumResult) {
      return false;
   }
   else if (+1 == comparingSignumResult) {
      return true;
   }
   // after calling Integer.compare_signum recognize both integers with identical nonzero signum

   assert(first.signum == second.signum);
   assert(first.signum != 0);
   byte identicalIntegersSignum = first.signum;

   if (identicalIntegersSignum > 0) {
      return isAbsoluteValueGreaterThan(first, second);
   }
   else if (identicalIntegersSignum < 0) {
      return isAbsoluteValueLessThan(first, second);
   }
   else {  // identicalIntegersSignum == 0
      throw new Exception("signum of integer = 0");
   }
}

public static boolean isAbsoluteValueGreaterThan(Integer first, Integer second) {
   byte firstIntegerDigit;
   byte secondIntegerDigit;
   for (byte index = 0; index < first.integerArray.length; index++) {
      firstIntegerDigit  = first.integerArray[index];
      secondIntegerDigit = second.integerArray[index];

      if (firstIntegerDigit > secondIntegerDigit) {
         return true;
      }
      else if (firstIntegerDigit < secondIntegerDigit) {
         return false;
      }
   }

   return false;
}

public static boolean isAbsoluteValueLessThan(Integer first, Integer second) {
   byte firstIntegerDigit;
   byte secondIntegerDigit;
   for (byte index = 0; index < first.integerArray.length; index++) {
      firstIntegerDigit  = first.integerArray[index];
      secondIntegerDigit = second.integerArray[index];

      if (firstIntegerDigit < secondIntegerDigit) {
         return true;
      }
      else if (firstIntegerDigit > secondIntegerDigit) {
         return false;
      }
   }

   return false;
}

public boolean isLessThanOrEqualTo(Integer integer) throws Exception {
   return Integer.isLessThanOrEqualTo(this, integer);
}

public static boolean isLessThanOrEqualTo(Integer first, Integer second) throws Exception {
   boolean notGreaterThan = ! isGreaterThan(first, second);

   return notGreaterThan;
}

public boolean isLessThan(Integer integer) throws Exception {
   return Integer.isLessThan(this, integer);
}

public static boolean isLessThan(Integer first, Integer second) throws Exception {
   byte comparingSignumResult = Integer.compare_signum(first, second);
   if (-1 == comparingSignumResult) {
      return true;
   }
   else if (+1 == comparingSignumResult) {
      return false;
   }
   // after calling Integer.compare_signum recognize both integers with identical nonzero signum

   assert(first.signum == second.signum);
   assert(first.signum != 0);
   byte identicalIntegersSignum = first.signum;

   if (identicalIntegersSignum < 0) {
      return isAbsoluteValueGreaterThan(first, second);
   }
   else if (identicalIntegersSignum > 0) {
      return isAbsoluteValueLessThan(first, second);
   }
   else {  // identicalIntegersSignum == 0
      throw new Exception("signum of integer = 0");
   }
}

public boolean isGreaterThanOrEqualTo(Integer integer) throws Exception {
   return Integer.isGreaterThanOrEqualTo(this, integer);
}

public static boolean isGreaterThanOrEqualTo(Integer first, Integer second) throws Exception {
   boolean notLessThan = ! isLessThan(first, second);

   return notLessThan;
}

public void add(Integer integer) {
   set(Integer.add(this, integer));
}

public static Integer add(Integer first, Integer second) {
   Integer result;
   byte comparingSignumResult = Integer.compare_signum(first, second);

   if (0 == comparingSignumResult) {    // integers with identical signums
      result = Integer.addAbsoluteValues(first, second);
   }
   else {  // integers with different signums
      result = Integer.subtractAbsoluteValues(first, second);
   }

   setSignumForAddingNonZeroResult(first, second, result, comparingSignumResult);

   return result;
}

private static void setSignumForAddingNonZeroResult(Integer first, Integer second,
                                       Integer result, byte comparingSignumResult) {
   if (result.signum == 0) {   // skip setSignum() for zero, zero has correct signum = 0 in private Integer(byte[]) constructor
      return;
   }

   if (0 == comparingSignumResult) {        // integers with identical signums
      result.setSignum(first.signum);
   }
   else {  // integers with different signums
      if (true == isAbsoluteValueGreaterThan(first, second)) {
         result.setSignum(first.signum);
      }
      else {
         result.setSignum(second.signum);
      }
   }
}

public void subtract(Integer integer) {
   set(Integer.subtract(this, integer));
}

public static Integer subtract(Integer first, Integer second) {
   Integer result;
   byte comparingSignumResult = Integer.compare_signum(first, second);

   if (0 == comparingSignumResult) {    // integers with identical sigmums
      result = Integer.subtractAbsoluteValues(first, second);
   }
   else {  // integers with different sigmums
      result = Integer.addAbsoluteValues(first, second);
   }

   setSignumForSubtractingNonZeroResult(first, second, result, comparingSignumResult);

   return result;
}

private static void setSignumForSubtractingNonZeroResult(Integer first, Integer second,
                                       Integer result, byte comparingSignumResult) {
   if (result.signum == 0) {   // skip setSignum() for zero, zero has correct signum = 0 in private Integer(byte[]) constructor
      return;
   }

   if (0 == comparingSignumResult) {        // integers with identical signums
      if (true == isAbsoluteValueGreaterThan(first, second)) {
         result.setSignum(first.signum);
      }
      else {
         result.setSignum((byte)(-second.signum));
      }
   }
   else {  // integers with different signums
      if (0 == first.signum) {   // if minuend is zero (with signum zero), then signum's result is negate of subtrahend's signum
         result.setSignum((byte)(-second.signum));
      }
      else {
         result.setSignum(first.signum);
      }
   }
}

public static Integer multiply(Integer first, Integer second) {
   Integer result = multiplyAbsoluteValues(first, second);
   if (result.signum == 0) {   // skip setSignum() for zero, zero has correct signum = 0 in private Integer(byte[]) constructor
      return result;
   }

   byte comparingSignumResult = Integer.compare_signum(first, second);
   if (0 == comparingSignumResult) {        // integers with identical signums
      result.setSignum((byte)(+1));
   }
   else {  // integers with different signums
      result.setSignum((byte)(-1));
   }

   return result;
}

public static Integer remainder(final Integer DIVIDEND, final Integer DIVISOR) throws Exception {
   if (true == DIVISOR.is_zero()) {
      throw new ArithmeticException("Divisor can not be zero");
   }

   Integer dividingResult    = divideAbsoluteValues(DIVIDEND, DIVISOR);
   Integer multipleOfDivisor = multiplyAbsoluteValues(dividingResult, DIVISOR);
   Integer remainder         = subtractAbsoluteValues(DIVIDEND, multipleOfDivisor);
   
   if (remainder.signum == 0) {   // skip setSignum() for zero, zero has correct signum = 0 in private Integer(byte[]) constructor
      return remainder;
   }
   
   remainder.setSignum(DIVIDEND.signum);

   return remainder;
}

public static Integer divide(Integer dividend, Integer divisor) throws Exception {
   if (true == divisor.is_zero()) {
      throw new ArithmeticException("Divisor can not be zero");
   }

   Integer result = divideAbsoluteValues(dividend, divisor);
   if (result.signum == 0) {   // skip setSignum() for zero, zero has correct signum = 0 in private Integer(byte[]) constructor
      return result;
   }

   byte comparingSignumResult = Integer.compare_signum(dividend, divisor);
   if (0 == comparingSignumResult) {        // integers with identical signums
      result.setSignum((byte)(+1));
   }
   else {  // integers with different signums
      result.setSignum((byte)(-1));
   }

   return result;
}

public static Integer divideAbsoluteValues(final Integer DIVIDEND, final Integer DIVISOR) throws Exception {
   if (true == DIVISOR.is_zero()) {
      throw new ArithmeticException("Divisor can not be zero");
   }
   
   final byte[] DIVIDEND_ARRAY = DIVIDEND.getIntegerArray();
   int currentDividendIndex = ArraysOperations.getMismatchValueIndex(DIVIDEND_ARRAY, 0, (byte)0);
   final byte[] DIVISOR_ARRAY  = DIVISOR.getIntegerArray();
   final int DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX = ArraysOperations.getMismatchValueIndex(DIVISOR_ARRAY, 0, (byte)0);
   
   if (0 > currentDividendIndex || currentDividendIndex > DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX) {
      return new Integer();   // default constructor initialized by zero
   }
   else {
      byte[] resultArray = calculateDividing(DIVIDEND_ARRAY, currentDividendIndex, DIVISOR, DIVISOR_ARRAY, 
                                                DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX);
      return new Integer(resultArray);
   }
}

private static byte[] calculateDividing(final byte[] DIVIDEND_ARRAY, int currentDividendIndex, 
               final Integer DIVISOR, final byte[] DIVISOR_ARRAY, final int DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX)
                                                      throws Exception {
   byte[] resultArray            = new byte[MAX_ARRAY_LENGTH];
   
   final int DIVISOR_DIGITS_LENGTH = DIVISOR_ARRAY.length - DIVISOR_OTHER_THAN_ZERO_VALUE_INDEX;
   
   byte[] temporaryDividendArray = {};
   Integer temporaryDividend = new Integer();
   byte nextDividendDigit;
                     
   while (currentDividendIndex < MAX_ARRAY_LENGTH) {
      nextDividendDigit = DIVIDEND_ARRAY[currentDividendIndex];
      temporaryDividendArray = modifyDividendArray(temporaryDividendArray, nextDividendDigit);
      temporaryDividend.set_integer_array(temporaryDividendArray);
      
      if (true == Integer.isAbsoluteValueLessThan(temporaryDividend, DIVISOR)) {
         resultArray[currentDividendIndex] = 0;
      }
      else {
         temporaryDividendArray = multiplyDivisor(resultArray, currentDividendIndex,
                                             temporaryDividend, temporaryDividendArray, DIVISOR);
      }
      
      currentDividendIndex++;
   }
                                                         
                                                         
   return resultArray;                                                       
}

private static byte[] modifyDividendArray(byte[] temporaryDividendArray, byte nextDividendDigit) {
   final int LENGTH = temporaryDividendArray.length + 1;
   temporaryDividendArray = Arrays.copyOf(temporaryDividendArray, LENGTH);
   temporaryDividendArray[LENGTH - 1] = nextDividendDigit;
   
   return temporaryDividendArray;
}

private static byte[] multiplyDivisor(byte[] resultArray, int currentDividendIndex,
                                       final Integer DIVIDEND, byte[] dividendArray,
                                       final Integer DIVISOR) throws Exception {
   Integer multipleOfDivisor = new Integer(DIVISOR.getIntegerArray());
   
   int multiple = 1;
   
   while (true == Integer.isAbsoluteValueLessThan(multipleOfDivisor, DIVIDEND)) {
      multipleOfDivisor = addAbsoluteValues(multipleOfDivisor, DIVISOR);
      multiple++;
   }
   
   if (true == Integer.isAbsoluteValueGreaterThan(multipleOfDivisor, DIVIDEND)) {  // if true, remainder != 0
      multipleOfDivisor = subtractAbsoluteValues(multipleOfDivisor, DIVISOR);
      multiple--;
      dividendArray = assignRemainder(DIVIDEND, multipleOfDivisor, dividendArray);
   }
   else {    // if Integer.isEqualTo(multipleOfDivisor, DIVIDEND)
      final byte[] EMPTY_ARRAY = new byte[0];
      dividendArray = Arrays.copyOf(EMPTY_ARRAY, 0);
   }
   
   if (multiple > 9) {
      throw new ArithmeticException("Unexpected multiple greater than 9");
   }
   
   resultArray[currentDividendIndex] = (byte)multiple;
   
   return dividendArray;
}

private static byte[] assignRemainder(final Integer DIVIDEND, final Integer MULTIPLE_OF_DIVISOR, 
                                       byte[] dividendArray) {
   final Integer REMAINDER = Integer.subtractAbsoluteValues(DIVIDEND, MULTIPLE_OF_DIVISOR);
   final byte[] REMAINDER_ARRAY  = REMAINDER.getIntegerArray();
   final int REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX = ArraysOperations.getMismatchValueIndex(REMAINDER_ARRAY, 0, (byte)0);
   final int REMAINDER_DIGITS_LENGTH = REMAINDER_ARRAY.length - REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX;
   
   dividendArray = Arrays.copyOf(dividendArray, REMAINDER_DIGITS_LENGTH);
   System.arraycopy(REMAINDER_ARRAY, REMAINDER_OTHER_THAN_ZERO_VALUE_INDEX, 
                     dividendArray, 0, REMAINDER_DIGITS_LENGTH);
                     
   return dividendArray;
}

public static Integer addAbsoluteValues(Integer first, Integer second) {
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

public static Integer subtractAbsoluteValues(Integer first, Integer second) {
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

public static Integer multiplyAbsoluteValues(Integer first, Integer second) {
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
      result = addAbsoluteValues(result, hugeIntegersArray[index]);
   }

   return result;
}

private static Integer getMaxOfAbsoluteValues(Integer first, Integer second) {
   boolean firstGreaterThanSecond = isAbsoluteValueGreaterThan(first, second);
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
