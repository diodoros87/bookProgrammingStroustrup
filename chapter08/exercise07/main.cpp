#include <vector>
#include <iostream>
#include <string>

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
   cout << "Names and ages" << endl;
   const unsigned int NAMES_SIZE = names.size();
   const unsigned int AGE_SIZE = age.size();
   const unsigned int SIZE = NAMES_SIZE > AGE_SIZE ? AGE_SIZE : NAMES_SIZE;
   for (unsigned int i = 0; i < SIZE; ) {
      cout << i << ". ";
      cout << names[i] << "\t\t";
      cout << age[i++] << "\n";
   }
   if (SIZE == 0)
      cerr << " No names and ages to print " << endl;
}

double read_age() {
   double age;
   do {
      age = get_double("Enter person's age: ");
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

void read_name(string& out) {
   string name;
   do {
      name = get_string("Enter person's name: ");
      try {
         validate_name(name);
         out = name;
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

void tests() {
   unsigned int quantity = 5;
   read_names(age_ordered_names, quantity);
   read_age(global_vec_age, quantity);
   print("Entered names and ages are printed below:", age_ordered_names, 
               global_vec_age);
   
}
