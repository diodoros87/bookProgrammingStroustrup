#include <string>
#include <cctype>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>

#include "persons.h"
#include "error.h"

using std::cout;
using std::cerr;
using std::endl;

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

void validate_age_index(unsigned int index) {
   const unsigned int SIZE = global_vec_age.size();
   if (SIZE <= index)
      error("No age for index " + std::to_string(index));
}

unsigned int get_name_index(const string& s, unsigned int identical_name_counter) {
   for (unsigned int i = 0; i < age_ordered_names.size(); i++)
      if (s == age_ordered_names[i]) {
         if (0 == identical_name_counter)
            return i;
         else
            identical_name_counter--;
      }
      
   error("No index for name ", s); 
}

double get_age(const string& name, unsigned int identical_name_counter) {
   const unsigned int index = get_name_index(name, identical_name_counter);
   try {
      validate_age_index(index);
      return global_vec_age[index];
   }
   catch (std::runtime_error& e) {
      error("No age for name ", name);
   }
}

unsigned int get_size_to_print() {
   const unsigned int NAMES_SIZE = age_ordered_names.size();
   const unsigned int AGE_SIZE = global_vec_age.size();
   const unsigned int SIZE = NAMES_SIZE > AGE_SIZE ? AGE_SIZE : NAMES_SIZE;
   return SIZE;
}

void print(const string& LABEL) {
   const unsigned int SIZE = get_size_to_print();
   cout << LABEL << endl;
   for (unsigned int i = 0; i < SIZE; i++) {
      cout << i << ". ";
      cout << age_ordered_names[i] << "\t\t";
      cout << global_vec_age[i] << "\n";
   }
   if (SIZE == 0)
      cerr << " No pair of name and age to print " << endl;
}

vector<string> get_sorted_vector(const vector<string>& input) {
   vector<string> result = input;
   sort(result.begin(), result.end());
   return result;
}

void print_sorted(const string& LABEL) {
   const unsigned int SIZE = get_size_to_print();
   cout << LABEL << endl;
   vector<string> sorted_names = get_sorted_vector(age_ordered_names);
   double age = 0;
   string previous_name; // empty string, name can not be empty
   unsigned int identity_counter = 0;
   for (unsigned int i = 0; i < SIZE; i++) {
      if (previous_name == sorted_names[i])
         identity_counter++;
      else
         identity_counter = 0;
      age = get_age(sorted_names[i], identity_counter);
      cout << i << ". ";
      cout << sorted_names[i] << "\t\t";
      cout << age << "\n";
      previous_name = sorted_names[i];
   }
   if (SIZE == 0)
      cerr << " No pair of name and age to print " << endl;
}
