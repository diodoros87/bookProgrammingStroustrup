#include <vector>
#include <iostream>
#include <string>

#include "error.h"
#include "input.h"
#include "Name_pairs.h"

using namespace std;

void test();

int main() {
   try {
      test();
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

void read(Name_pairs& pairs) {
   unsigned const int OLD_SIZE = pairs.get_names_size();
   constexpr char END = '@';
   cout << "Enter names. To finish enter " << END << " as first character of string \n";
   pairs.read_names();
   unsigned int quantity = pairs.get_names_size() - OLD_SIZE;
   if (quantity > 0) {
      cout << "Enter " << quantity << " numbers as person's age \n";
      pairs.read_ages(quantity);
      print("Entered names with age are printed below:\n", pairs);
      pairs.sort();
      print("Sorted entered names with age are printed below:\n", pairs);
   }
}

void read(Name_pairs& pairs, unsigned int quantity) {
   if (0 == quantity)
      return;
   cout << "Enter " << quantity << " names \n";
   pairs.read_names(quantity);
   cout << "Enter " << quantity << " numbers as person's age \n";
   pairs.read_ages(quantity);
   print("Entered names with age are printed below:\n", pairs);
   pairs.sort();
   print("Sorted entered names with age are printed below:\n", pairs);
}

void test_comparison(const Name_pairs& v1, const Name_pairs& v2) {
   if (v1 == v2)
      cout << "v1 == v2\n";
   else if (v1 != v2)
      cout << "v1 != v2\n";
}

void test_comparison(const Name_pairs& input) {
   Name_pairs copy = input;
   cout << "comparison of input vector and its copy\n";
   test_comparison(input, copy);
   
   Name_pairs other;
   cout << "comparison of input vector and empty vector\n";
   test_comparison(input, other); 
   
   unsigned int input_size = input.get_names_size();
   read(other, input_size);
   cout << "comparison of input vector and other vector with identical size\n";
   test_comparison(input, other); 
   
   read(other, 1);
   cout << "comparison of input vector and other vector with different size\n";
   test_comparison(input, other);
}

void test() {
   Name_pairs pairs;
   test_comparison(pairs);
   cout << pairs;
   unsigned int quantity = 5;
   read(pairs, quantity);
   read(pairs);
   test_comparison(pairs);
}
