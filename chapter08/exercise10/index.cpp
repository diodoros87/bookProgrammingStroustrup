#include <vector>
#include <iostream>
#include <string>
#include <limits>

using namespace std;

void test();

int main() {
   try {
      test();
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

double max(const vector<double>& vec) {
   const unsigned int SIZE = vec.size();
   if (SIZE == 0)
      throw runtime_error("Size of input vectors must be > 0");
   
   double max = -std::numeric_limits<double>::infinity();
   for (unsigned int i = 0; i < SIZE; i++)
      if (vec[i] > max)
         max = vec[i];

   return max;
}

double min(const vector<double>& vec) {
   const unsigned int SIZE = vec.size();
   if (SIZE == 0)
      throw runtime_error("Size of input vectors must be > 0");
   
   double min = std::numeric_limits<double>::infinity();
   for (unsigned int i = 0; i < SIZE; i++)
      if (vec[i] < min)
         min = vec[i];

   return min;
}

double index(const vector<double>& price, const vector<double>& weight) {
   const unsigned int SIZE = price.size();
   if (SIZE == 0)
      throw runtime_error("Size of input vectors must be > 0");
   if (SIZE != weight.size())
      throw runtime_error("Size of input vectors must be equal");
      
   double result = 0;
   for (unsigned int i = 0; i < SIZE; i++) {
      if (price[i] < 0)
         throw runtime_error("price must be >= 0");
      if (weight[i] < 0)
         throw runtime_error("weight must be >= 0");
      result += price[i] * weight[i];
   }
   return result;
}

void print_extremums(const vector<double>& vec, const string& LABEL) {
   cout << "Min of " << LABEL << " = " << min(vec) << endl;
   cout << "Max of " << LABEL << " = " << max(vec) << endl;
}

void test() {
   vector<double> price = {1, 3.5, 5, 7, 0.5};
   vector<double> weight = {2.5, 3, 5.5, 7.5, 0};
   cout << "Result of index function = " << index(price, weight) << endl;
   
   print_extremums(price, "price");
   print_extremums(weight, "weight");
}
