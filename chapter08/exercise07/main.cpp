#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

#include "input.h"
#include "error.h"
#include "persons.h"

using namespace std;

void tests();

int main() {
   try {
      tests();
      return 0;
   }
   catch (Number_expected) {
      cerr << "Enter non-number value, but number was expected" << endl;
      return 1;
   }
   catch (End_of_data) {
      cerr << "End-of-transmission character was detected" << endl;
      return 0;
   }
   catch (exception& e) {
      cerr << "exception: " << e.what() << endl;
      return 1;
   }
   catch (...) {
      cerr << "unrecognized exception\n";
      return 2;
   }
}

void print(const string& LABEL, const vector<string>& names, const vector<double>& age) {
   cout << LABEL << endl;
   const unsigned int NAMES_SIZE = names.size();
   const unsigned int AGE_SIZE = age.size();
   const unsigned int SIZE = NAMES_SIZE > AGE_SIZE ? AGE_SIZE : NAMES_SIZE;
   for (unsigned int i = 0; i < SIZE; i++) {
      cout << i << ". ";
      cout << names[i] << "\t\t";
      cout << age[i] << "\n";
   }
   if (SIZE == 0)
      cerr << " No names and ages to print " << endl;
}

void print_sorted(const string& LABEL, const vector<string>& names, const vector<double>& vec_age) {
   cout << LABEL << endl;
   const unsigned int NAMES_SIZE = names.size();
   const unsigned int AGE_SIZE = vec_age.size();
   const unsigned int SIZE = NAMES_SIZE > AGE_SIZE ? AGE_SIZE : NAMES_SIZE;
   double age = 0;
   for (unsigned int i = 0; i < SIZE; i++) {
      age = get_age(names[i]);
      cout << i << ". ";
      cout << names[i] << "\t\t";
      cout << age << "\n";
   }
   if (SIZE == 0)
      cerr << " No names and ages to print " << endl;
}

double read_age() {
   double age;
   do {
      age = get_double();
      try {
         validate_age(age);
         return age;
      }
      catch (runtime_error& e) {
         cerr << e.what() << endl;
      }
   } while (true);
}

void read_age(vector<double>& vec_age, unsigned int n) {
   if (n == 0)
      error(" number of age must be > 0");
   cout << "Enter " << n << " number as person's age \n";
   double age;
   for (unsigned int i = 0; i < n; i++) {
      age = read_age();
      vec_age.push_back(age);
   }
}

void read_name(string& name) {
   do {
      read_string(name);
      try {
         validate_name(name);
         break;
      }
      catch (runtime_error& e) {
         cerr << e.what() << endl;
      }
   } while (true);
}

void read_names(vector<string>& vec_name, unsigned int n) {
   if (n == 0)
      error(" number of names must be > 0");
   string name;
   cout << "Enter " << n << " names \n";
   for (unsigned int i = 0; i < n; i++) {
      read_name(name);
      vec_name.push_back(name);
   }
}

vector<string> get_sorted_vector(const vector<string>& input) {
   vector<string> result = input;
   sort(result.begin(), result.end());
   return result;
}

void tests() {
   unsigned int quantity = 5;
   read_names(age_ordered_names, quantity);
   read_age(global_vec_age, quantity);
   print("Entered names and ages are printed below:", age_ordered_names, 
               global_vec_age);
   vector<string> sorted_names = get_sorted_vector(age_ordered_names);
   print("Entered names and ages are printed below:", age_ordered_names, 
               global_vec_age);
   print_sorted("Sorted entered names and ages are printed below:", sorted_names, 
               global_vec_age);
}
