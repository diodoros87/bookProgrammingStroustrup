#include <string>
#include <cctype>
#include <iostream>

#include "Patron.h"

using namespace std;
   
static void validate_name (const string& s) {
   static constexpr unsigned int MIN_SIZE = 5;
   const unsigned int SIZE = s.size();
   if (MIN_SIZE > SIZE)
      throw Patron::Invalid_Patron("User name length must be >= " + to_string(MIN_SIZE));
   if (! isalpha(s[0]))
      throw Patron::Invalid_Patron("User name's first character must be alphabetic");
   static constexpr char UNDERSCORE = '_';
   for (unsigned int i = 1; i < SIZE; i++)
      if (! isalnum(s[i]) && s[i] != UNDERSCORE) 
         throw Patron::Invalid_Patron("User name can contains only alphanumerical chars or " +
            string(1, UNDERSCORE));
}

Patron::Patron(const string& name, unsigned int n, unsigned int ch) : 
user_name(name), number(n), charges(ch)
{
   validate_name(name);
}

Patron::Patron(const string& name, unsigned int n) : 
user_name(name), number(n)
{
   validate_name(name);
}
