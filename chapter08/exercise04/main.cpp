#include <iostream>
#include <string>
#include <vector>

#include "fibonacci.h"

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

void print(const string& LABEL, const vector<int>& vec) {
   cout << LABEL;
   const unsigned int SIZE = vec.size();
   unsigned int i = 0;
   while (i < SIZE) {
      cout << i << ". ";
      cout << vec[i++] << endl;
   }
   if (SIZE == 0)
      cout << " has 0 elements " << endl;
}

void print(int first, int second, vector<int>& vec, int elements) {
   try {
      fibonacci(first, second, vec, elements);
      const string LABEL = "---Fibonacci numbers for int type: \n\
   start = " + to_string(first) + " second = " + to_string(second) + 
      " number of elements " + to_string(elements) + "\n";
      print(LABEL, vec);
   }
   catch (runtime_error& e) {
      cerr << e.what() << endl;
   }
}

void tests() {
   const vector<int>& fibonacci_numbers = fibonacci_wrapped();
   print("All Fibonacci numbers for int type: \n", fibonacci_numbers);
   const int MAX = get_fibonacci_max();
   cout << "Max Fibonacci number for int type: " << MAX << " \n";
   
   const vector<int>& repeat = fibonacci_wrapped();
   print("Repeat: All Fibonacci numbers for int type: \n", repeat);
   
   vector<int> fib_range;
   print(fibonacci_numbers[2], fibonacci_numbers[2], fib_range, 45);
   
   print(fibonacci_numbers[2], fibonacci_numbers[2], fib_range, 45);
   fib_range.clear();
   print(1, 2, fib_range, 6);
   
   fib_range.clear();
   print(fibonacci_numbers[3], fibonacci_numbers[3], fib_range, 4);
   print(-6, 0, fib_range, 4);
   print(1, 2, fib_range, 1);
   fib_range.clear();
   print(1, 1, fib_range, 1);
   fib_range.clear();
   print(1, 2, fib_range, 2);
   fib_range.clear();
   print(0, 1, fib_range, 2);
   fib_range.clear();
   print(0, 1, fib_range, 3);
   fib_range.clear();
   print(2, 2, fib_range, 1);
   print(2, 1, fib_range, 1);
   print(1, 4, fib_range, 1);
   print(4, 5, fib_range, 1);
   print(4, 8, fib_range, 1);
   print(1, 2, fib_range, 0);
   print(1, 2, fib_range, -4);
   print(3, 5, fib_range, 48);
   print(0, 1, fib_range, 47);
   fib_range.clear();
   print(0, 1, fib_range, 48);
   print(3, 5, fib_range, 43);
   fib_range.clear();
   print(3, 5, fib_range, 44);
   const int INDEX_MAX = get_index(MAX, fibonacci_numbers);
   print(fibonacci_numbers[INDEX_MAX - 1], MAX, fib_range, 3);
   print(fibonacci_numbers[INDEX_MAX - 1], MAX, fib_range, 2);
   fib_range.clear();
   print(fibonacci_numbers[INDEX_MAX - 1], MAX, fib_range, 1);
   fib_range.clear();
   print(MAX, MAX, fib_range, 1);
}
