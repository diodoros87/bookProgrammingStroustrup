#include <string>
#include <cctype>
#include <vector>
#include <stdexcept>

#include "persons.h"
#include "error.h"

vector<string> age_ordered_names;
vector<double> global_vec_age;

void validate_name (const string& s) {
   unsigned int SIZE = s.size();
   if (2 > SIZE)
      error("Name length must be > 1");
   if (! isupper(s[0]))
      error("Name's first character must be upper case");   
   for (unsigned int i = 1; i < s.size(); i++)
      if (! islower(s[i]))
         error("After first character name must contain only lower case");
}

 void validate_age (double age) {
   if (age <= 0 || age > 150)
      error("Age must be > 0 and <= 150");
}

unsigned int get_name_index(const string& s) {
   for (unsigned int i = 0; i < age_ordered_names.size(); i++)
      if (s == age_ordered_names[i])
         return i;
      
   error("No index for name ", s);
}

void validate_age_index(unsigned int index) {
   const unsigned int SIZE = global_vec_age.size();
   if (SIZE <= index)
      error("No age for index " + std::to_string(index));
}

double get_age(const string& name) {
   const unsigned int index = get_name_index(name);
   try {
      validate_age_index(index);
      return global_vec_age[index];
   }
   catch (std::runtime_error& e) {
      error("No age for name ", name);
   }
}
