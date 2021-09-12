#include <vector>
#include <iostream>
#include <string>

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

inline void validate_positive_number(unsigned int number, const string& LABEL) {
   if (number == 0)
      throw runtime_error(LABEL + " must be > 0");
}

struct Statistics {
   double min;
   double max;
   double median;
   double mean;
};

vector<double> get_sorted(const vector<double>& vec) {
   const unsigned int SIZE = vec.size();
   validate_positive_number(SIZE, "Size of input vector");
   
   vector<double> sorted = vec;
   double temp;
   bool change;
   do {
      change = false;
      for (unsigned int index = 0; index < SIZE - 1; index++) {
         if (sorted[index] > sorted[index + 1]) {
            temp = sorted[index];
            sorted[index] = sorted[index + 1];
            sorted[index + 1] = temp;
            change = true;
         }
      }
   } while (change);
   
   return sorted;
}

double median(const vector<double>& vec) {
   const unsigned int SIZE = vec.size();
   validate_positive_number(SIZE, "Size of input vector");
   double median;
   vector<double> sorted;
   sorted = get_sorted(vec);
   if (SIZE % 2 == 1)
      median = sorted[SIZE / 2];
   else
      median = (sorted[SIZE / 2] + sorted[SIZE / 2 - 1]) / 2;
   
   return median;
}

double mean(const vector<double>& vec) {
   const unsigned int SIZE = vec.size();
   validate_positive_number(SIZE, "Size of input vector");
   
   double mean = vec[0];
   for (unsigned int i = 1; i < SIZE; i++)
      mean += vec[i];
   
   mean /= SIZE;
   return mean;
}

double max(const vector<double>& vec) {
   const unsigned int SIZE = vec.size();
   validate_positive_number(SIZE, "Size of input vector");
   
   double max = vec[0];
   for (unsigned int i = 1; i < SIZE; i++)
      if (vec[i] > max)
         max = vec[i];

   return max;
}

double min(const vector<double>& vec) {
   const unsigned int SIZE = vec.size();
   validate_positive_number(SIZE, "Size of input vector");
   
   double min = vec[0];
   for (unsigned int i = 1; i < SIZE; i++)
      if (vec[i] < min)
         min = vec[i];

   return min;
}

Statistics get_statistics(const vector<double>& vec) {
   Statistics result;
   result.min = min(vec);
   result.max = max(vec);
   result.median = median(vec);
   result.mean = mean(vec);
   
   return result;
}

void print_statistics(const Statistics& statistics, const string& LABEL) {
   cout << "Min of " << LABEL << " = " << statistics.min << endl;
   cout << "Max of " << LABEL << " = " << statistics.max << endl;
   cout << "Median of " << LABEL << " = " << statistics.median << endl;
   cout << "Mean of " << LABEL << " = " << statistics.mean << endl;
}

void print_statistics(const vector<double>& vec, const string& LABEL) {
   Statistics statistics = get_statistics(vec);
   print_statistics(statistics, LABEL);
}

void test() {
   vector<double> price = {1, 3.5, 5, 7, 0.5};
   vector<double> weight = {2.5, 3, 5.5, 7.5, 0};
   print_statistics(price, "price");
   print_statistics(weight, "weight");
   cout << "NEW CONTENT: \n";
   price.push_back(1);
   weight.push_back(1);
   print_statistics(price, "price");
   print_statistics(weight, "weight");
}
