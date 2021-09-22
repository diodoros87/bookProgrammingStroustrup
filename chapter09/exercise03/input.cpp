#include <iostream>
#include <string>
#include <limits>

#include "input.h"

using std::cin;
using std::cout;

namespace Input {
   
static void check_input() {
   if (cin.eof())
      throw End_of_data();
   std::cerr << "Error: Input operation failed - start of clearing input data\n";
   cin.clear();
   cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

string get_string () {
   string result;
   cin >> result;
   if (! cin) 
      check_input();
   return result;
}

string get_string (const string& PROMPT) {
   cout << PROMPT;
   string result = get_string();
   return result;
}

void read_string (string& s) {
   cin >> s;
   if (! cin) 
      check_input();
}

void read_string (const string& PROMPT, string& s) {
   cout << PROMPT;
   read_string(s);
}

double get_double () {
   double result;
   do {
      cin >> result;
      if (cin)
         return result;
      std::cerr << "Value entered is not type double number. Enter number ";
      check_input();
   } while (true);
}

double get_double (const string& PROMPT) {
   cout << PROMPT;
   double result = get_double();
   return result;
}

}
