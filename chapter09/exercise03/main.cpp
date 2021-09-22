#include <vector>
#include <iostream>
#include <string>

#include "error.h"
#include "input.h"
#include "Name_pairs.h"

using namespace std;

void tests();

int main() {
   try {
      tests();
      return 0;
   }
   catch (Input::End_of_data& e) {
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

void print(const string& LABEL, const Name_pairs& pairs) {
   cout << LABEL;
   cout << pairs;
}

void tests() {
   Name_pairs pairs;
   cout << pairs;
   unsigned int quantity = 5;
   cout << "Enter " << quantity << " names \n";
   pairs.read_names(quantity);
   cout << "Enter " << quantity << " numbers as person's age \n";
   pairs.read_ages(quantity);
   print("Entered names with age are printed below:\n", pairs);
   //cout << "Entered names with age are printed below:\n";
   //cout << pairs;
   
   pairs.sort();
   print("Sorted entered names with age are printed below:\n", pairs);
   //cout << "Sorted entered names with age are printed below:\n";
   //cout << pairs;
   
   constexpr char END = '@';
   cout << "Enter names. To finish enter " << END << " as first character of string \n";
   pairs.read_names();
   quantity = pairs.get_names_size() - quantity;
   if (quantity > 0) {
      cout << "Enter " << quantity << " numbers as person's age \n";
      pairs.read_ages(quantity);
      print("Entered names with age are printed below:\n", pairs);
      //cout << "Entered names with age are printed below:\n";
      //cout << pairs;
      pairs.sort();
      print("Sorted entered names with age are printed below:\n", pairs);
      //cout << "Sorted entered names with age are printed below:\n";
      //cout << pairs;
   }
}
