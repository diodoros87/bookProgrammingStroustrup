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

class Statistics {
public:
   int min_length;
   int max_length;
   string first_by_order;
   string last_by_order;
};

vector<int> get_sizes(const vector<string>& vec) {
   vector<int> result;
   for (unsigned int index = 0; index < vec.size(); index++)
      result.push_back(vec[index].size());
   
   return result;
}

int get_max_length(const vector<string>& vec) {
   const unsigned int SIZE = vec.size();
   validate_positive_number(SIZE, "Size of input vector");
   
   int max = vec[0].size();
   for (unsigned int i = 1; i < SIZE; i++)
      if (vec[i].size() > max)
         max = vec[i].size();

   return max;
}

int get_min_length(const vector<string>& vec) {
   const unsigned int SIZE = vec.size();
   validate_positive_number(SIZE, "Size of input vector");
   
   int min = vec[0].size();
   for (unsigned int i = 1; i < SIZE; i++)
      if (vec[i].size() < min)
         min = vec[i].size();

   return min;
}

string get_last_by_order(const vector<string>& vec) {
   const unsigned int SIZE = vec.size();
   validate_positive_number(SIZE, "Size of input vector");
   
   string max = vec[0];
   for (unsigned int i = 1; i < SIZE; i++)
      if (vec[i] > max)
         max = vec[i];

   return max;
}

string get_first_by_order(const vector<string>& vec) {
   const unsigned int SIZE = vec.size();
   validate_positive_number(SIZE, "Size of input vector");
   
   string min = vec[0];
   for (unsigned int i = 1; i < SIZE; i++)
      if (vec[i] < min)
         min = vec[i];

   return min;
}

vector<int> get_sizes_statistics(const vector<string>& vec, Statistics& statistics) {
   statistics.min_length = get_min_length(vec);
   statistics.max_length = get_max_length(vec);
   statistics.first_by_order = get_first_by_order(vec);
   statistics.last_by_order = get_last_by_order(vec);
   
   vector<int> result = get_sizes(vec);
   return result;
}

void print_statistics(const Statistics& statistics, const string& LABEL) {
   cout << "Min length string in " << LABEL << " = " << statistics.min_length << endl;
   cout << "Max length string in " << LABEL << " = " << statistics.max_length << endl;
   cout << "First by order string in " << LABEL << " = " << statistics.first_by_order << endl;
   cout << "Last by order string in " << LABEL << " = " << statistics.last_by_order << endl;
}

void print(const vector<int>& vec, const string& LABEL) {
   cout << LABEL << endl;
   if (vec.empty())
      cout << " has 0 elements ";
   for (int i : vec) 
      cout << i << '\t';

   cout << '\n';
}

void print_statistics(const vector<string>& vec, const string& LABEL) {
   Statistics statistics;
   vector<int> sizes = get_sizes_statistics(vec, statistics);
   print_statistics(statistics, LABEL);
   print(sizes, LABEL + " has strings with sizes: ");
}

void test() {
   vector<string> text = {"1", ", ", "3.5",  ", 5", ", 7, 0.5", " er t ", "rty", 
            "Aristotle", "Plato", "Lucius Seneca", "Epicurus", "Archimedes", "Zeno Citium", "Baruch Spinoza",
            "Leibniz", "Plutarch", "Claudius Ptolemaeus", "Epicurus", "Einstein", "Leibniz", "Bertrand Russell"};
   print_statistics(text, "text");
   
   text.clear();
   print_statistics(text, "text");
}
