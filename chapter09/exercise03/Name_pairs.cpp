#include <string>
#include <cctype>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>

#include "Name_pairs.h"
#include "input.h"
#include "error.h"

using std::cout;
using std::cerr;
using std::endl;

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
/*
void validate() {
   if (! size_equal())
      error("Number of names different than number of ages");
}
*/
double read_age() {
   double age;
   do {
      age = Input::get_double();
      try {
         validate_age(age);
         return age;
      }
      catch (std::runtime_error& e) {
         cerr << e.what() << endl;
      }
   } while (true);
}

void Name_pairs::read_ages(unsigned int n) {
   if (n == 0)
      error(" number of age must be > 0");
   //cout << "Enter " << n << " numbers as person's age \n";
   double age;
   for (unsigned int i = 0; i < n; i++) {
      age = read_age();
      vec_age.push_back(age);
   }
}

void read_name(string& name) {
   do {
      Input::read_string(name);
      try {
         validate_name(name);
         break;
      }
      catch (std::runtime_error& e) {
         cerr << e.what() << endl;
      }
   } while (true);
}

void Name_pairs::read_names() {
   constexpr char END = '@';
   //cout << "Enter names. To finish enter " << END << " as first character of string \n";
   string name = Input::get_string();
   while (name.size() > 0 && END != name[0]) {
      try {
         validate_name(name);
         vec_names.push_back(name);
      }
      catch (std::runtime_error& e) {
         cerr << e.what() << endl;
      }
      name = Input::get_string();
   }
}

void Name_pairs::read_names(unsigned int n) {
   if (n == 0)
      error(" number of names must be > 0");
   string name;
   //cout << "Enter " << n << " names \n";
   for (unsigned int i = 0; i < n; i++) {
      read_name(name);
      vec_names.push_back(name);
   }
}
/*
void Name_pairs::validate_age_index(unsigned int index) {
   const unsigned int SIZE = vec_age.size();
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
      return vec_age[index];
   }
   catch (std::runtime_error& e) {
      error("No age for name ", name);
   }
}
*/
int Name_pairs::get_min_name_index(unsigned int begin, unsigned int end) {
   const unsigned int SIZE = vec_names.size();
   if (begin >= SIZE || end > SIZE)
      error("Index is too big");
   if (begin >= end)
      error("begin index >= end index");
   
   string min = vec_names[begin];
   int result = begin;
   for (unsigned int i = begin + 1; i < end; i++)
      if (vec_names[i] < min) {
         min = vec_names[i].size();
         result = i;
      }

   return result;
}

void Name_pairs::sort() {
   if (! size_equal())
      error("Number of names different than number of ages");
   const unsigned int SIZE = vec_age.size();
   int min_index = 0;
   //string temp_name;
   //int temp_age;
   for (unsigned int index = 0; index < SIZE - 1; index++) {
      min_index = get_min_name_index(index, SIZE);
      if (index != min_index) {
         std::swap(vec_names[index], vec_names[min_index]);
         std::swap(vec_age[index], vec_age[min_index]);/*
         temp_name = vec_names[index];
         vec_names[index] = vec_names[min_index];
         vec_names[min_index] = temp_name;
         temp_age = vec_age[index];
         vec_names[index] = vec_names[min_index];*/
      }
   }
}
/*
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
*/
std::ostream& operator<<(std::ostream& os, const Name_pairs& pairs) {
   vector<string> names = pairs.get_names();
   vector<double> ages = pairs.get_ages();
   const unsigned int NAMES_SIZE = names.size();
   const unsigned int AGE_SIZE = ages.size();
   const unsigned int SIZE = NAMES_SIZE > AGE_SIZE ? AGE_SIZE : NAMES_SIZE;
   if (SIZE == 0)
      os << " No pair of name and age to print " << endl;
   else {
      //os << LABEL << endl;
      for (unsigned int i = 0; i < SIZE; i++) {
         os << i << ". ";
         os << names[i] << "\t\t";
         os << ages[i] << "\n";
      }
   }
   return os;
}

bool operator==(const Name_pairs& a, const Name_pairs& b) {
   vector<string> a_names = a.get_names();
   vector<double> a_ages = a.get_ages();
   vector<string> b_names = b.get_names();
   vector<double> b_ages = b.get_ages();
   const unsigned int A_NAMES_SIZE = a_names.size();
   const unsigned int A_AGE_SIZE = a_ages.size();
   const unsigned int B_NAMES_SIZE = b_names.size();
   const unsigned int B_AGE_SIZE = b_ages.size();
   if (A_NAMES_SIZE != B_NAMES_SIZE || A_AGE_SIZE != B_AGE_SIZE)
      return false;
   for (unsigned int i = 0; i < A_NAMES_SIZE; i++)
      if (a_names[i] != b_names[i])
         return false;
   for (unsigned int i = 0; i < B_AGE_SIZE; i++)
      if (a_ages[i] != b_ages[i])
         return false;
   return true;
}

//------------------------------------------------------------------------------

bool operator!=(const Name_pairs& a, const Name_pairs& b) {
   return !(a==b);
}
