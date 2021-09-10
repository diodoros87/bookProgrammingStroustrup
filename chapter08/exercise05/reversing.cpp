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

vector<int> reverse_order_1(const vector<int>& vec) {
            cerr << "reverse_order_1" << endl;
   const unsigned int SIZE = vec.size();
   vector<int> result(SIZE);
   for (unsigned int i = 0; i < SIZE; i++)
      result[i] = vec[SIZE - 1 - i];
            cerr << "reverse_order_1 END" << endl;
   return result;
}

void swap(int& a, int& b) {
   int temp = a;
   a = b;
   b = temp;
}

void reverse_order_2(vector<int>& vec) {
            cerr << "reverse_order_2" << endl;
   const unsigned int SIZE = vec.size();
   if (0 == SIZE)
      return;
   for (unsigned int i = 0, j = SIZE - 1; i < j; i++, j--)
      swap(vec[i], vec[j]);
            cerr << "reverse_order_2 END" << endl;
}

void print(const string& LABEL, const vector<int>& vec) {
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

void return_value_results(const string& LABEL, vector<int>& vec) {
   cout << "--- Before vector<int> reverse_order(const vector<int>& vec)\n";
   print(LABEL, vec);
   vec = reverse_order_1(vec);
   cout << "--- AFTER \n";
   print(LABEL, vec);
}

void reference_results(const string& LABEL, vector<int>& vec) {
   cout << "%%%% Before void reverse_order(vector<int>& vec)\n";
   print(LABEL, vec);
   reverse_order_2(vec);
   cout << "%%%% AFTER \n";
   print(LABEL, vec);
}

void tests(const string& LABEL, vector<int>& vec) {
   return_value_results(LABEL, vec);
   reference_results(LABEL, vec);
   cout << "\n\tNEXT VECTOR\n";
}

void tests() {
   vector<int> vec;
   tests("empty", vec);
   
   vec = {1};
   tests("one", vec);
   
   vec = {1, 2};
   tests("two", vec);
   
   vec = {1, 2, 3};
   tests("three", vec);
   
   vec = {1, 2, 3, 4};
   tests("four", vec);
   
   vec = {1, 2, 3, 4, 5};
   tests("four", vec);
}
