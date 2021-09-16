#include <iostream>
#include <cassert>
#include <string>

using namespace std;

//#define NDEBUG

#ifndef NDEBUG
#   define M_Assert(Expr, Msg) \
    __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#   define M_Assert(Expr, Msg) ;
#endif

void __M_Assert(const char* expr_str, const bool expr,
                const char* file, const int line, const string& msg) {
    if (! expr) {
      std::cerr << "\nAssert failed:\t" << msg << "\n"
         << "Expected:\t" << expr_str << "\n"
         << "Source:\t\t" << file << ", line " << line << "\n";
      exit(EXIT_FAILURE);
   }
}

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
   void add_day(int n);                // increase or decrease the Date by n days
   void set_previous_day();
   void set_next_day();
};

//------------------------------------------------------------------------------

inline bool is_valid(const Date& date) {
   return is_valid(date.y, date.m, date.d);
}

ostream& operator<<(ostream& os, const Date& d) {
   return os << '(' << d.y
            << ',' << d.m
            << ',' << d.d 
            << ')';
}

Date::Date(int y, int m, int d) {
   // check that (y,m,d) is a valid date
   // if it is, use it to initialize date
   
   if (! is_valid(y, m, d)) {
      cerr << y << ", " << m << ", " << d << " is incorrect date\n";
      throw runtime_error("Wrong date ");
   }
   this->y = y;
   this->m = m;
   this->d = d;
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
   if (! is_valid(*this)) {
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
   if (! is_valid(*this)) {
      cerr << *this << " is incorrect date\n";
      throw runtime_error("Wrong date ");
   }
}

void Date::add_day(int n) {
   // increase by n days
   if (n > 0) {
      for (int counter = 0; counter < n; counter++) {
         cerr << *this << endl;
         set_next_day();
      }
   }
   else {  // decrease by n days
      for (int counter = 0; counter > n; counter--) {
         cerr << *this << endl;
         set_previous_day();
      }
   }
}

inline void check_assertion(int x, int expected_x, const string& LABEL) {
   const string message = " " + LABEL + " " + to_string(x) +
                     " != expected " + LABEL + " " + to_string(expected_x);
   M_Assert(x == expected_x, message);
}

inline void check_assertion(const Date& date, int y, int m, int d) {
   check_assertion(date.d, d, "day");
   check_assertion(date.m, m, "month");
   check_assertion(date.y, y, "year");
}

void test_incorrect(Date& date) {
   int d = date.d;
   int m = date.m;
   int y = date.y;
   try {
      //date = {999, 16, 0};
      date = Date(999, 16, 0);
   }
   catch (runtime_error& e) {
      check_assertion(date, y, m, d);
      cerr << "Exception catched: " << e.what() << endl;
   }
}

//------------------------------------------------------------------------------

int main() {
   Date today(1978, 7, 20); 
   cerr << "today = " << today << endl;
   check_assertion(today, 1978, 7, 20);
   Date tomorrow = today; 
   check_assertion(tomorrow, 1978, 7, 20);
   tomorrow.add_day(1);
   check_assertion(tomorrow, 1978, 7, 21);
   cout << "\n tomorrow = " << tomorrow << '\n';
   
   tomorrow.add_day(-1);
   check_assertion(tomorrow, 1978, 7, 20);
   cout << "\n tomorrow = " << tomorrow << '\n';
   
   today = {2000, 4, 7};
   cerr << "today = " << today << endl; 
   
   test_incorrect(today); 
   
   Date test = {1999, 12, 31};
   cout << "\n test date = " << test << '\n';
   test.add_day(+2000);
   cout << "\n test date = " << test << '\n';
   test.add_day(-2000);
   cout << "\n test date = " << test << '\n';
   
   test = {-3, 12, 31};
   cout << "\n test = " << test << '\n';
   test.add_day(+2000);
   cout << "\n test = " << test << '\n';
   
   test.add_day(-2000);
   cout << "\n test = " << test << '\n';
}
