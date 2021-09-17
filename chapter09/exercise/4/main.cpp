#include <iostream>
#include <cassert>
#include <string>

#include "date.h"

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
      cerr << "\nAssert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n";
      exit(EXIT_FAILURE);
   }
}

inline void check_assertion(int x, int expected_x, const string& LABEL) {
   const string message = " " + LABEL + " " + to_string(x) +
                     " != expected " + LABEL + " " + to_string(expected_x);
   M_Assert(x == expected_x, message);
}

inline void check_assertion(const Date& date, int y, int m, int d) {
   check_assertion(date.day(), d, "day");
   check_assertion(date.month(), m, "month");
   check_assertion(date.year(), y, "year");
}

void test_incorrect(Date& date) {
   int d = date.day();
   int m = date.month();
   int y = date.year();
   try {
      date = {Year{FORBIDDEN_YEAR}, Date::dec, 31};
   }
   catch (Invalid& e) {
      check_assertion(date, y, m, d);
      cerr << "Exception catched: " << e.what() << endl;
   }
}

//------------------------------------------------------------------------------

void test() {
   Date today(Year{1978}, Date::jul, 20); 
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
   
   today = {Year{2000}, Date::Month::apr, 7};
   cerr << "today = " << today << endl; 
   
   test_incorrect(today); 
   
   Date test = {Year{1999}, Date::Month::dec, 31};
   cout << "\n test date = " << test << '\n';
   test.add_day(+2000);
   cout << "\n test date = " << test << '\n';
   test.add_day(-2000);
   cout << "\n test date = " << test << '\n';
   
   //Date last;
   test = {Year{2000}, Date::Month(55), 34};
   cout << "\n test = " << test << '\n';
   test.add_day(+2000);
   cout << "\n test = " << test << '\n';
   
   test.add_day(-2000);
   cout << "\n test = " << test << '\n';
}

int main() {
   try {
      test();
      return 0;
   }
   catch (Invalid& e) {
      cerr << "exception: " << e.what() << endl;
      return 3;
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
