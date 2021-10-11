#include "integer_parsing.hpp"
#include "integer.hpp" 

#include <cctype> 

namespace integer_parsing {

bool string_contain_only_digits(const string & STR) {
   const short STRING_LENGTH = STR.length();
   for (short index = 0; index < STRING_LENGTH; index++) 
      if (false == isdigit(STR[index])) 
         return false;
   return 0 == STRING_LENGTH ? false : true;
}

bool string_contain_only_value(const string & STR, const char value) {
   const short STRING_LENGTH = STR.length();
   for (int index = 0; index < STRING_LENGTH; index++) 
      if (value != STR[index]) 
         return false;
   return 0 == STRING_LENGTH ? false : true;
}

static void validate_signum(const string & STR, const char first_character) {
   bool contain_only_zeros = string_contain_only_value(STR, '0');
   if (true == contain_only_zeros && (Integer::PLUS == first_character || Integer::MINUS == first_character))
      throw invalid_argument("Requirement: string can not contain signum for zero integer");
}

void validate_string(const string & STR) {
   const short MAX_STRING_LENGTH       = Integer::MAX_ARRAY_LENGTH + 1;
   const short VALIDATED_STRING_LENGTH = STR.length();
   if (VALIDATED_STRING_LENGTH > MAX_STRING_LENGTH) 
      throw invalid_argument("Requirement: integer's string.length <= " + to_string(MAX_STRING_LENGTH));
   if (VALIDATED_STRING_LENGTH == 0) 
      throw invalid_argument("Requirement: integer's string can not be empty");
   const char first   = STR[0];
   if (VALIDATED_STRING_LENGTH == 1) {
      if (false == isdigit(first))
         throw invalid_argument("Requirement: string must contains only integer number without other characters");
   }
   else {
      string remained = STR.substr(1); 
      if (false == is_integer_character(first) || false == string_contain_only_digits(remained)) 
         throw invalid_argument("Requirement: string must contains only integer number without other characters");
      validate_signum(remained, first);
   }
}

template<typename Container>
static short skip_leading_integers(const Container& CONTAINER, const digit_type & skipped) {
   short index = 0;
   while (index < CONTAINER.size() - 1) 
      if (skipped == CONTAINER[index])
         index++;
      else 
         break;
      
   return index;
}

short skip_leading_integers(const array<digit_type, Integer::MAX_ARRAY_LENGTH> & ARRAY, const digit_type & skipped) {
   return skip_leading_integers(ARRAY, skipped);
}

template <unsigned int N>
short skip_leading_integers(const array<digit_type, N> & ARRAY, const digit_type & skipped) {
   if (0 == N)
      throw invalid_argument("Requirement: array can not be empty");
   return skip_leading_integers(ARRAY, skipped);
}
   
   
} 
