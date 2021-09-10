#include <iostream>
#include <string>
#include <vector>

#include "fibonacci.h"

using namespace std;

void print(string label, const vector<int>& vec) {
   cout << label;
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
   fibonacci(first, second, vec, elements);
   string label = "---Fibonacci numbers for int type: \n\
start = " + to_string(first) + " second = " + to_string(second) + 
   " number of elements " + to_string(elements) + "\n";
   print(label, vec);
}

int main() {
   const vector<int>& fibonacci_numbers = fibonacci_wrapped();
   print("All Fibonacci numbers for int type: \n", fibonacci_numbers);
   
   cout << "Max Fibonacci number for int type: " << get_fibonacci_max() << " \n";
   
   const vector<int>& repeat = fibonacci_wrapped();
   print("Repeat: All Fibonacci numbers for int type: \n", repeat);
   
   vector<int> fib_range;
   print(fibonacci_numbers[2], fibonacci_numbers[2], fib_range, 45);
   
   print(fibonacci_numbers[2], fibonacci_numbers[2], fib_range, 45);
   fib_range.clear();
   print(1, 2, fib_range, 6);
   
   fib_range.clear();
}
