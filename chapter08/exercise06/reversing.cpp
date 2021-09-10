#include <vector>
#include <iostream>
#include <string>

using namespace std;

void tests();

int main() {
   try {
      tests();
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

vector<string> reverse_order_1(const vector<string>& vec) {
            cerr << "reverse_order_1" << endl;
   const unsigned int SIZE = vec.size();
   vector<string> result(SIZE);
   for (unsigned int i = 0; i < SIZE; i++)
      result[i] = vec[SIZE - 1 - i];
            cerr << "reverse_order_1 END" << endl;
   return result;
}

void swap(string& a, string& b) {
   string temp = a;
   a = b;
   b = temp;
}

void reverse_order_2(vector<string>& vec) {
            cerr << "reverse_order_2" << endl;
   const unsigned int SIZE = vec.size();
   if (0 == SIZE)
      return;
   for (unsigned int i = 0, j = SIZE - 1; i < j; i++, j--)
      swap(vec[i], vec[j]);
            cerr << "reverse_order_2 END" << endl;
}

void print(const string& LABEL, const vector<string>& vec) {
   cout << LABEL << endl;
   const unsigned int SIZE = vec.size();
   unsigned int i = 0;
   while (i < SIZE) {
      cout << i << ". ";
      cout << vec[i++] << endl;
   }
   if (SIZE == 0)
      cout << " has 0 elements " << endl;
}

void return_value_results(const string& LABEL, vector<string>& vec) {
   cout << "--- Before vector<string> reverse_order(const vector<string>& vec)\n";
   print(LABEL, vec);
   vec = reverse_order_1(vec);
   cout << "--- AFTER \n";
   print(LABEL, vec);
}

void reference_results(const string& LABEL, vector<string>& vec) {
   cout << "%%%% Before void reverse_order(vector<string>& vec)\n";
   print(LABEL, vec);
   reverse_order_2(vec);
   cout << "%%%% AFTER \n";
   print(LABEL, vec);
}

void tests(const string& LABEL, vector<string>& vec) {
   return_value_results(LABEL, vec);
   reference_results(LABEL, vec);
   cout << "\n\tNEXT VECTOR\n";
}

void tests() {
   vector<string> vec;
   tests("empty", vec);
   
   vec = {"ab c"};
   tests("one", vec);
   
   vec = {"ab c", "RT 6Y"};
   tests("two", vec);
   
   vec = {"ab c", "RT 6Y", "h"};
   tests("three", vec);
   
   vec = {"ab c", "RT 6Y", "h", "5 r \ter \n yy"};
   tests("four", vec);
   
   vec = {"ab c", "RT 6Y", "h", "5 r \ter \n yy", "5 t"};
   tests("five", vec);
}
