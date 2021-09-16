#include <iostream>
#include <cassert>
#include <string>

using namespace std;

// helper functions:

inline bool is_valid_year(int y) {
   if (0 == y)
      return false;
   return true;
}

inline bool is_valid_month(int m) {
   if (1 > m || 12 < m)
      return false;
   return true;
}

bool leapyear(int y) {
   if (! is_valid_year(y))
      throw runtime_error("Year can not be 0");
   if (y < 0)    // to convenient calculate of leap year assume years BC 
      y++;       // increased by 1
   
   if (y % 4 == 0 && y % 100 != 0)
      return true;
   if (y % 400 == 0)
      return true;
   
   return false;
}

int get_max_day(int m, int y) {
   if (! is_valid_year(y))
      throw runtime_error("Year can not be 0");
   switch(m) {
      case 1: case 3: case 5: case 7: case 8: case 10: case 12:
         return 31;
      case 4: case 6: case 9: case 11:
         return 30;
      case 2:
         return leapyear(y) ? 29 : 28;
      default:
         throw runtime_error("Month must be in range 1 - 12");
   }
}

bool is_valid(int y, int m, int d) {
   if (! is_valid_year(y))
      return false;
   if (! is_valid_month(m))
      return false;
   int max_day = get_max_day(m, y);
   if (d < 1 || d > max_day)
      return false;
   
   return true;
}

// simple Date:
// guarantee initialization with constructor
// provide some notational convenience
struct Date {
   int y, m, d;                        // year, month, day 
   
   Date(int y, int m, int d);          // check for valid date and initialize
   void add_day(int n);                // increase the Date by n days
   bool is_valide() { return is_valid(y, m, d); }
   void set_previous_day();
   void set_next_day();
};

//------------------------------------------------------------------------------



ostream& operator<<(ostream& os, const Date& d) {
   return os << '(' << d.y
            << ',' << d.m
            << ',' << d.d 
            << ')';
}

Date::Date(int y, int m, int d)  : y(y), m(m), d(d) {
   // check that (y,m,d) is a valid date
   // if it is, use it to initialize date
   if (! is_valid(y, m, d)) {
      cerr << y << ", " << m << ", " << d << " is incorrect date\n";
      throw runtime_error("Wrong date ");
   }/*
   y = y;
   m = m;
   d = d;*/
}

//------------------------------------------------------------------------------

void Date::set_previous_day() {
   if (d != 1)
      d--;
   else {
      if (m != 1)
         m--;
      else {
         m = 12;
         y--;
         if (0 == y)
            y--;
      }
      d = get_max_day(m, y);
   }
   if (! is_valide()) {
      cerr << *this << " is incorrect date\n";
      throw runtime_error("Wrong date ");
   }
}

void Date::set_next_day() {
   int max_day = get_max_day(m, y);
   if (d < max_day)
      d++;
   else {
      d = 1;
      if (m != 12) 
         m++;
      else {
         m = 1;
         y++;
         if (0 == y)
            y++;
      }
   }
   if (! is_valide()) {
      cerr << *this << " is incorrect date\n";
      throw runtime_error("Wrong date ");
   }
}

void Date::add_day(int n) {
   // increase dd by n days
   if (n > 0) {
      for (int counter = 0; counter < n; counter++) {
         cerr << *this << endl;
         set_next_day();
      }
   }
   else {  // decrease dd by n days
      for (int counter = 0; counter > n; counter--) {
         cerr << *this << endl;
         set_previous_day();
      }
   }
}

inline const char* get_message(int x, int expected_x, const string& LABEL) {
   const string message = " " + LABEL + " " + to_string(x) +
                     " != expected " + LABEL + " " + to_string(expected_x);
   return message.c_str();
}

inline void check_assertion(int x, int expected_x, const string& LABEL) {
   //const string message = " " + LABEL + " " + to_string(x) +
   //                 " != expected " + LABEL + " " + to_string(expected_x);
   //
   /*
   const char *fmt = "sqrt(2) = %f";
   int sz = snprintf(NULL, 0, fmt, sqrt(2));
   char buf[sz + 1]; // note +1 for terminating null byte
   snprintf(buf, sizeof buf, fmt, sqrt(2));
   */
   const char* message = string(" " + LABEL + " " + to_string(x) +
                     " != expected " + LABEL + " " + to_string(expected_x)).c_str();
   assert( (x == expected_x) && message);
   //assert( (x == expected_x) && get_message(x, expected_x, LABEL) );
   //string(" " + LABEL + " " + to_string(x) +
     //                " != expected " + LABEL + " " + to_string(expected_x)));
}

inline void check_assertion(const Date& date, int y, int m, int d) {
   check_assertion(date.d, d, "day");
   check_assertion(date.m, m, "month");
   check_assertion(date.y, y, "year");
}

//------------------------------------------------------------------------------

int main() {
   Date today(1978, 7, 20); 
   cerr << "today = " << today;
   check_assertion(today, 1978, 7, 20);
   Date tomorrow = today; 
   check_assertion(tomorrow, 1978, 7, 20);
   tomorrow.add_day(1);
   check_assertion(tomorrow, 1978, 7, 21);
   cout << "\n tomorrow = " << tomorrow << '\n';
   
   tomorrow.add_day(-1);
   check_assertion(tomorrow, 1978, 7, 20);
   cout << "\n tomorrow = " << tomorrow << '\n';
   
//    init_day(today, 1978, 23, 20);
//    init_day(today, 1978, 3, 44);
//    init_day(today, 0, 3, 1);
//    init_day(today, 1978, 1, -20);
//    init_day(today, 1978, -2, 7);
//    init_day(today, 1978, 0, 7);
//    init_day(today, 1978, 3, 0);
   
   
   //add_day(tomorrow, +10000);
   //cout << "\n tomorrow = " << tomorrow << '\n';
   
   //add_day(tomorrow, -10000);
   //cout << "\n tomorrow = " << tomorrow << '\n';
   
   Date test = {1999, 12, 31};
   test.add_day(+2000);
   cout << "\n test date = " << test << '\n';
   test.add_day(-2000);
   cout << "\n test date = " << test << '\n';
//    init_day(test, 1999, 12, 31);
//    add_day(test, -2000);
//    cout << "\n test = " << test << '\n';
//    
//    init_day(test, -333, 12, 31);
//    add_day(test, -2000);
//    cout << "\n test = " << test << '\n';
   
   test = {-3, 12, 31};
   test.add_day(+2000);
   cout << "\n test = " << test << '\n';
   
   //init_day(test, -3, 12, 31);
   test.add_day(-2000);
   cout << "\n test = " << test << '\n';
   
   return 0;
}

//------------------------------------------------------------------------------
