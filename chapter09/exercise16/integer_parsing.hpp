#ifndef INTEGER_PARSING_HPP
#define INTEGER_PARSING_HPP

#include "integer.hpp"
//#include "integer_parsing.hpp"

namespace integer_parsing {
   
using integer_space::Integer;
   
void validate_string(const string & STR);
short skip_leading_integers(const array<digit_type, Integer::MAX_ARRAY_LENGTH> & ARRAY, const digit_type & skipped);
//template <unsigned int N>
//short skip_leading_integers(const array<digit_type, N> & ARRAY, const digit_type & skipped);
bool string_contain_only_value(const string & STR, const char value);
bool string_contain_only_digits(const string & STR);

inline bool is_integer_character(const char character) {
   if (isdigit(character) || character == '+' || character == '-') 
      return true;
   return false;
}

}

#endif
