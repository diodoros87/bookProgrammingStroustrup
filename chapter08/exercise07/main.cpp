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

void read_names(vector<string>& vec_name) {
   const string END = "@";
   string name;
   cout << "Enter names. To finish enter " << END << " \n";
   read_string(name);
   while (END != name) {
      try {
         validate_name(name);
         vec_name.push_back(name);
      }
      catch (runtime_error& e) {
         cerr << e.what() << endl;
      }
      read_string(name);
   }
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
   read_names(age_ordered_names, 5);
   read_age(global_vec_age, quantity);
   print("Entered names with age are printed below:");
   
   print_sorted("Sorted entered names with age are printed below:");
   print("Entered names with age are printed below:");
   
   age_ordered_names.clear();
   global_vec_age.clear();
   read_names(age_ordered_names);
   quantity = age_ordered_names.size();
   if (quantity > 0) {
      read_age(global_vec_age, quantity);
      print("Entered names with age are printed below:");
      
      print_sorted("Sorted entered names with age are printed below:");
      print("Entered names with age are printed below:");
   }
}
