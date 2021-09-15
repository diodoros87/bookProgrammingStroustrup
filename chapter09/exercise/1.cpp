#include <iostream>
#include <cassert>

using namespace std;

// simple Date 
struct Date {
   int y; // year
   int m; // month in year
   int d; // day of month
};

//------------------------------------------------------------------------------

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

inline bool is_valid(const Date& date) {
   return is_valid(date.y, date.m, date.d);
}

ostream& operator<<(ostream& os, const Date& d) {
   return os << '(' << d.y
            << ',' << d.m
            << ',' << d.d 
            << ')';
}

void init_day(Date& date, int y, int m, int d) {
   // check that (y,m,d) is a valid date
   // if it is, use it to initialize date
   if (! is_valid(y, m, d)) {
      cerr << y << ", " << m << ", " << d << " is incorrect date\n";
      throw runtime_error("Wrong date ");
   }
   date.y = y;
   date.m = m;
   date.d = d;
}

//------------------------------------------------------------------------------

void set_previous_day(Date& date) {
   if (date.d != 1)
      date.d--;
   else {
      if (date.m != 1)
         date.m--;
      else {
         date.m = 12;
         date.y--;
         if (0 == date.y)
            date.y--;
      }
      date.d = get_max_day(date.m, date.y);
   }
   if (! is_valid(date)) {
      cerr << date << " is incorrect date\n";
      throw runtime_error("Wrong date ");
   }
}

void set_next_day(Date& date) {
   int max_day = get_max_day(date.m, date.y);
   if (date.d < max_day)
      date.d++;
   else {
      date.d = 1;
      if (date.m != 12) 
         date.m++;
      else {
         date.m = 1;
         date.y++;
         if (0 == date.y)
            date.y++;
      }
   }
   if (! is_valid(date)) {
      cerr << date << " is incorrect date\n";
      throw runtime_error("Wrong date ");
   }
}

void add_day(Date& date, int n) {
   // increase dd by n days
   if (n > 0) {
      for (int counter = 0; counter < n; counter++) {
         cerr << date << endl;
         set_next_day(date);
      }
   }
   else {  // decrease dd by n days
      for (int counter = 0; counter > n; counter--) {
         cerr << date << endl;
         set_previous_day(date);
      }
   }
}

inline void check_assertion(const Date& date, int y, int m, int d) {
   assert((date.y == y && date.m == m && date.d == d) && "assert failed");
}

//------------------------------------------------------------------------------

int main() {
   Date today; 
   init_day(today, 1978, 7, 20);
   check_assertion(today, 1978, 7, 20);
   Date tomorrow = today; 
   check_assertion(tomorrow, 1978, 7, 20);
   add_day(tomorrow, 1);
   check_assertion(tomorrow, 1978, 7, 21);
   cout << "today = " << today;
   cout << "\n tomorrow = " << tomorrow << '\n';
   
   add_day(tomorrow, -1);
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
   
   Date test;
//    init_day(test, 1999, 12, 31);
//    add_day(test, -2000);
//    cout << "\n test = " << test << '\n';
//    
//    init_day(test, -333, 12, 31);
//    add_day(test, -2000);
//    cout << "\n test = " << test << '\n';
   
   init_day(test, -3, 12, 31);
   add_day(test, +2000);
   cout << "\n test = " << test << '\n';
   
   //init_day(test, -3, 12, 31);
   add_day(test, -2000);
   cout << "\n test = " << test << '\n';
   
   return 0;
}

//------------------------------------------------------------------------------
