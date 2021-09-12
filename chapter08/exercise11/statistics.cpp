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

void get_statistics(const vector<double>& vec, double& min_ref, double& max_ref, double& mean_ref,
                      double& median_ref) {
   min_ref = min(vec);
   max_ref = max(vec);
   median_ref = median(vec);
   mean_ref = mean(vec);
}

void print_statistics(const Statistics& statistics, const string& LABEL) {
   cout << "Min of " << LABEL << " = " << statistics.min << endl;
   cout << "Max of " << LABEL << " = " << statistics.max << endl;
   cout << "Median of " << LABEL << " = " << statistics.median << endl;
   cout << "Mean of " << LABEL << " = " << statistics.mean << endl;
}

void print_statistics(const double MIN, const double MAX, const double MEAN,
                      const double MEDIAN, const string& LABEL) {
   cout << "Min of " << LABEL << " = " << MIN << endl;
   cout << "Max of " << LABEL << " = " << MAX << endl;
   cout << "Median of " << LABEL << " = " << MEDIAN << endl;
   cout << "Mean of " << LABEL << " = " << MEAN << endl;
}

void print_statistics(const vector<double>& vec, const string& LABEL) {
   cout << "STATISTICS by return struct: \n";
   Statistics statistics = get_statistics(vec);
   print_statistics(statistics, LABEL);
   
   cout << "STATISTICS by refence arguments: \n";
   double max = -1;
   double min = -1;
   double mean = -1;
   double median = -1;
   get_statistics(vec, min, max, mean, median);
   print_statistics(min, max, mean, median, LABEL);
}

void test(vector<double>& vec, const string& LABEL) {
   print_statistics(vec, LABEL);
   cout << "NEW CONTENT: \n";
   vec.push_back(1);
   print_statistics(vec, LABEL);
}

void test() {
   vector<double> price = {1, 3.5, 5, 7, 0.5};
   test(price, "price");
   
   vector<double> weight = {2.5, 3, 5.5, 7.5, 0};
   test(weight, "weight");
   
   weight.clear();
   test(weight, "weight");
}
