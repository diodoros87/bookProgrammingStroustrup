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

void print_until_s(const vector<string>& vec, const string& QUIT, const int SKIPPED) {
   if (SKIPPED < 0)
      throw runtime_error("Parameter SKIPPED must be > 0");
   int counter = 0;
   for (string s : vec) {
      if (s == QUIT) {
          if (counter == SKIPPED)
             return;
          else
             counter++;
      }
      cout << s << '\t';
   }
}

void print_until_s(const vector<string>& vec, const string& QUIT) {
   for (string s : vec) {
      if (s == QUIT)
         return;
      cout << s << '\t';
   }
}

void test() {
   vector<string> text = {"1", ", ", "3.5",  ", 5", ", 7, 0.5", " er t ", "rty", 
            "Aristotle", "Plato", "Lucius Seneca", "Epicurus", "Archimedes", "Zeno Citium", "Baruch Spinoza",
            "Leibniz", "Plutarch", "Claudius Ptolemaeus", "Epicurus", "Einstein", "Leibniz", "Bertrand Russell"};
   
   print_until_s(text, "");
   cout << "\n----------------------------------\n";
   print_until_s(text, "", 2);
   cout << "\n----------------------------------\n";
   print_until_s(text, "", 0);
   cout << "\n----------------------------------\n";
   
   print_until_s(text, " er t ");
   cout << "\n----------------------------------\n";
   print_until_s(text, "Epicurus", 1);
   cout << "\n----------------------------------\n";
   print_until_s(text, "Claudius Ptolemaeus", 0);
   cout << "\n----------------------------------\n";
   print_until_s(text, "Leibniz", 1);
   cout << "\n----------------------------------\n";
   print_until_s(text, "Macchiavelli");
   cout << "\n----------------------------------\n";
   print_until_s(text, "Macchiavelli", 5);
   cout << "\n----------------------------------\n";
   print_until_s(text, "Macchiavelli", -5);
}
