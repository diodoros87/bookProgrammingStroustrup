#include <iostream>
#include <string>

#include "input.h"

using std::cin;
using std::cout;

string get_string () {
   string result;
   cin >> result;
   if (! cin) 
      throw End_of_data();
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
      throw End_of_data();
}

void read_string (const string& PROMPT, string& s) {
   cout << PROMPT;
   read_string(s);
}

double get_double () {
   double result;
   cin >> result;
   if (! cin) 
      throw Number_expected();
   return result;
}

double get_double (const string& PROMPT) {
   cout << PROMPT;
   double result = get_double();
   return result;
}
