#ifndef INTEGER_H
#define INTEGER_H
typedef short digit_type;

namespace integer_space {
   
inline bool operator<=(const Integer& first, const Integer& second) {
   return ! (first > second);
}

inline bool operator>=(const Integer& first, const Integer& second) {
   return ! (first < second);
}
   
class Integer {
private:
   array<digit_type, MAX_ARRAY_LENGTH> integer_array;
   char signum {NEUTRAL_SIGNUM};   // less than 0 for integers < 0, more than 0 for integers > 0, 0 for 0   
   Integer(const vector<digit_type> & vec);
public:
   Integer(const vector<digit_type> & vec, const char signum);
   Integer(const array<digit_type, MAX_ARRAY_LENGTH> & table, const char signum) {
      validate(table, signum);
   }
   template <typename unsigned int N>
   void Integer(const array<digit_type, N> & table, const char signum) {
      validate_size(table);
      validate(table, signum);
   }
   Integer(const vector<digit_type> & vec, const char signum) {
      validate_size(vec);
      validate(vec, signum);
   }
   static constexpr short MAX_ARRAY_LENGTH = 40;
   static constexpr char NEUTRAL_SIGNUM   = ' ';
   static constexpr char PLUS_SIGNUM      = '+';
   static constexpr char MINUS_SIGNUM     = '-';
private:
   array<digit_type, MAX_ARRAY_LENGTH> get_integer_array() const {
      return integer_array;
   }
   
   template <typename Container>
   void validate(const Container& obj, char signum);

public:
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

   void setSignum(const char signum) {
      validate_signum(signum);
      this->signum = signum;
   }
   char get_signum() const { return signum; }
   bool is_zero() { return is_zero(*this); }
   static bool is_zero(const Integer& integer);
   int_fast8_t Integer::compare_signum(const Integer& integer) {
      return Integer::compare_signum(*this, integer);
   }
   static int_fast8_t compare_signum(const Integer& first, const Integer& second);
   
   bool operator==(const Integer& other) const { return operator==(*this, other); }
   bool operator!=(const Integer& other) const { return !(*this == other); } ;
   
   bool operator>(const Integer& other) const ;
   bool operator>(const Integer& other) const ;
   static bool is_absolute_value_greater(const Integer& first, const Integer& second);
   static bool is_absolute_value_less(const Integer& first, const Integer& second);
   
   static Integer remainder(const Integer& DIVIDEND, const Integer& DIVISOR);
   static Integer divide_absolute_values(const Integer& DIVIDEND, const Integer& DIVISOR);
   
   public byte compareSignum(Integer hugeInteger) {
      return Integer.compareSignum(this, hugeInteger);
   }

   public static byte compareSignum(Integer first, Integer second) {
      if (first.signum > 0) {
         if (second.signum > 0) {
            return 0;
         }
         else {  // second.signum <= 0
            return +1;
         }
      }
      else if (first.signum == 0) {
         if (second.signum > 0) {
            return -1;
         }
         else if (second.signum == 0) {
            return 0;
         }
         else { // second.signum < 0
            return +1;
         }
      }
      else {  // first.signum < 0
         if (second.signum >= 0) {
            return -1;
         }
         else { // second.signum < 0
            return 0;
         }
      }
   }

   public boolean isEqualTo(Integer hugeInteger) {
      return Integer.isEqualTo(this, hugeInteger);
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

      byte comparingSignumResult = Integer.compareSignum(first, second);
      if (0 != comparingSignumResult) {
         return false;
      }

      //assert(0 == comparingSignumResult);
      return true;
   }

   public boolean isNotEqualTo(Integer hugeInteger) {
      return Integer.isNotEqualTo(this, hugeInteger);
   }

   public static boolean isNotEqualTo(Integer first, Integer second) {
      boolean notEquality = ! isEqualTo(first, second);

      return notEquality;
   }

   public boolean isGreaterThan(Integer hugeInteger) throws Exception {
      return Integer.isGreaterThan(this, hugeInteger);
   }

   public static boolean isGreaterThan(Integer first, Integer second) throws Exception {
      byte comparingSignumResult = Integer.compareSignum(first, second);
      if (-1 == comparingSignumResult) {
         return false;
      }
      else if (+1 == comparingSignumResult) {
         return true;
      }
      // after calling Integer.compareSignum recognize both integers with identical nonzero signum

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

   public boolean isLessThanOrEqualTo(Integer hugeInteger) throws Exception {
      return Integer.isLessThanOrEqualTo(this, hugeInteger);
   }

   public static boolean isLessThanOrEqualTo(Integer first, Integer second) throws Exception {
      boolean notGreaterThan = ! isGreaterThan(first, second);

      return notGreaterThan;
   }

   public boolean isLessThan(Integer hugeInteger) throws Exception {
      return Integer.isLessThan(this, hugeInteger);
   }

   public static boolean isLessThan(Integer first, Integer second) throws Exception {
      byte comparingSignumResult = Integer.compareSignum(first, second);
      if (-1 == comparingSignumResult) {
         return true;
      }
      else if (+1 == comparingSignumResult) {
         return false;
      }
      // after calling Integer.compareSignum recognize both integers with identical nonzero signum

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

   public boolean isGreaterThanOrEqualTo(Integer hugeInteger) throws Exception {
      return Integer.isGreaterThanOrEqualTo(this, hugeInteger);
   }

   public static boolean isGreaterThanOrEqualTo(Integer first, Integer second) throws Exception {
      boolean notLessThan = ! isLessThan(first, second);

      return notLessThan;
   }

   public void add(Integer hugeInteger) {
      set(Integer.add(this, hugeInteger));
   }

   public static Integer add(Integer first, Integer second) {
      Integer result;
      byte comparingSignumResult = Integer.compareSignum(first, second);

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

   public void subtract(Integer hugeInteger) {
      set(Integer.subtract(this, hugeInteger));
   }

   public static Integer subtract(Integer first, Integer second) {
      Integer result;
      byte comparingSignumResult = Integer.compareSignum(first, second);

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

      byte comparingSignumResult = Integer.compareSignum(first, second);
      if (0 == comparingSignumResult) {        // integers with identical signums
         result.setSignum((byte)(+1));
      }
      else {  // integers with different signums
         result.setSignum((byte)(-1));
      }

      return result;
   }
   
   public static Integer remainder(final Integer DIVIDEND, final Integer DIVISOR) throws Exception {
      if (true == DIVISOR.isZero()) {
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
      if (true == divisor.isZero()) {
         throw new ArithmeticException("Divisor can not be zero");
      }

      Integer result = divideAbsoluteValues(dividend, divisor);
      if (result.signum == 0) {   // skip setSignum() for zero, zero has correct signum = 0 in private Integer(byte[]) constructor
         return result;
      }

      byte comparingSignumResult = Integer.compareSignum(dividend, divisor);
      if (0 == comparingSignumResult) {        // integers with identical signums
         result.setSignum((byte)(+1));
      }
      else {  // integers with different signums
         result.setSignum((byte)(-1));
      }

      return result;
   }

   public static Integer divideAbsoluteValues(final Integer DIVIDEND, final Integer DIVISOR) throws Exception {
      if (true == DIVISOR.isZero()) {
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
         temporaryDividend.setIntegerArray(temporaryDividendArray);
         
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

   public static Integer negate(Integer hugeInteger) {
      byte negateSignum = (byte)(-hugeInteger.signum);

      return new Integer(hugeInteger.integerArray, negateSignum);
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
      byte index = HugeIntegerParsing.skipLeadingIntegersWithValue(this.integerArray, skippedValue);

      while (index < MAX_ARRAY_LENGTH) {
         number = this.integerArray[index];
         result += number.toString();
         index++;
      }

      return result;
   }
}

   
}
