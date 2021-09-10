#include <climits>
#include <vector>
#include <iostream>
#include <string>

using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

inline void error(const string& errormessage) {
   throw std::runtime_error("!!!!! Error: " + errormessage);
}

bool exist(int x, int y, const vector<int>& vec) {
   bool is_x = false;
   bool is_y = false;
   for (unsigned int i = 0; i < vec.size() && ! (is_x && is_y); i++) {
      if (! is_x && x == vec[i])
         is_x = true;
      if (! is_y && y == vec[i])
         is_y = true;
   }
   return is_x && is_y;
}

int get_index(int x, const vector<int>& vec) {
   for (unsigned int i = 0; i < vec.size(); i++)
      if (x == vec[i]) 
         return i;
   
   return -9;   // incorrect index
}

bool is_sum_int_overflow(int a, int b);

const vector<int>& fibonacci() {
   static vector<int> numbers = {0, +1};  // must be static to exist in program all time running
   unsigned int size = numbers.size();
   int next;
   while (false == is_sum_int_overflow(numbers[size - 1], numbers[size - 2])) {
      next = numbers[size - 1] + numbers[size - 2];
      numbers.push_back(next);
      size++;
   } 
   return numbers;
}

const vector<int>& fibonacci_wrapped() {
   static vector<int> numbers = fibonacci();   // must be static to exist in program all time running
   return numbers;
}

int get_fibonacci_max() {
   const vector<int>& vec = fibonacci_wrapped();
   static const int MAX = vec[vec.size() - 1];
   return MAX;
}

bool are_consecutive_fibonacci(int x, int next) {
   const vector<int>& vec = fibonacci_wrapped();
   const int MAX_INDEX = vec.size() - 1;
   int ix = get_index(x, vec);
   if (ix < 0 || ix >= MAX_INDEX)
      return false;
   if (next == vec[ix + 1])
      return true;
   if (x == vec[ix + 1] && ix < MAX_INDEX - 1 && next == vec[ix + 2]) 
      // fib[0] = 0 fib[1] = 1 fib[2] = 1 fib[3] = 2
      return true;
   
   return false;
}

bool is_sum_int_overflow(int a, int b) {
   bool result = false;
   if (a > 0 && b > 0) {
      if (a > INT_MAX - b) {
         result = true;
         cout << " INT_MAX = " << INT_MAX << endl;
      }
   }
   else if (a < 0 && b < 0) {
      if (a < INT_MIN - b) {
         result = true;
         cout << " INT_MIN = " << INT_MIN << endl;
      }
   }
   
   if (result) {
      cout << " a = " << a << endl;
      cout << " b = " << b << endl;
      cout << "Overflow for int type \n";
   }
   return result;
}

void fibonacci(int x, int y, vector<int>& vec, int n) {
   if (n <= 0) 
      error("Number of numbers to print must be > 0 \n");
   if (false == vec.empty())
      error("Input vector must be empty \n");
   if (x > y) {
      cerr << " x = " << x << " y = " << y << endl;
      error("Requirement: x <= y \n");
   }
   const vector<int>& fib_numbers = fibonacci_wrapped();
   if (! exist(x, y, fib_numbers)) 
      error("Not all of the numbers are Fibonacci numbers \n");
   if (! are_consecutive_fibonacci(x, y))
      error("Numbers are not consecutive Fibonacci numbers \n");

   int start = get_index(x, fib_numbers);
   const int MAX_INDEX = fib_numbers.size() - 1;
   if (start < MAX_INDEX - 1 && x == fib_numbers[start + 1] && y == fib_numbers[start + 2])
      start++;
   if (MAX_INDEX < start + n - 1) {
      cerr << " Index of x (start index of printing) = " << start << endl;
      cerr << " Max index of Fibonacci numbers = " << MAX_INDEX << endl;
      cerr << " Number of numbers to print = " << n << endl;
      error("Number of numbers to print is too big \n");
   }
   for (int index = start; n > 0; n--, index++) 
      vec.push_back(fib_numbers[index]);
}
