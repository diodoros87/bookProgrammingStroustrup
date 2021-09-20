#include <iostream>
#include <cassert>
#include <string>

#include "date.h"

using namespace std;
using namespace Chrono;

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
      date = { 1900, Date::feb, 29 };
      assert(false);
   }
   catch (Date::Invalid& e) {
      check_assertion(date, y, m, d);
      cerr << "Exception catched: " << e.what() << endl;
   }
}

Date get_date_from_input() {
   Date date;
   if (cin) {
      cout << "Enter date in format (Y,M,D) ";
      cin >> date;
   }
   return date;
}

void test_input() {
   Date date_from_input = get_date_from_input();
   cout << "date from input = " << date_from_input << endl;
   date_from_input = get_date_from_input();
   cout << "date from input = " << date_from_input << endl;
}

void standard_test() {
   Date today(1978, Date::jul, 20); 
   cerr << "today = " << today << endl;
   check_assertion(today, 1978, Date::jul, 20);
   Date tomorrow = today; 
   check_assertion(tomorrow, 1978, Date::jul, 20);
   tomorrow.add_day(1);
   check_assertion(tomorrow, 1978, Date::jul, 21);
   cout << "\n tomorrow = " << tomorrow << '\n';
   
   tomorrow.add_day(-1);
   check_assertion(tomorrow, 1978, Date::jul, 20);
   cout << "\n tomorrow = " << tomorrow << '\n';
   
   today = {2000, Date::Month::apr, 7};
   cerr << "today = " << today << endl; 
   
   test_incorrect(today); 
}

void adding_test(bool day, bool month, bool year, Date& date, bool minus) {
   int number = 0;
   if (day) {
      number = minus ? -2000 : +2000;
      date.add_day(number);
   }
   if (month) {
      number = minus ? -170 : +170;
      date.add_month(number);
   }
   if (year) {
      number = minus ? -10 : +10;
      date.add_year(number);
   }
}

void adding_test(bool day, bool month, bool year) {
   Date test = {1999, Date::Month::dec, 31};
   cout << "\n test date 1999/200n= " << test << '\n';
   adding_test(day, month, year, test, false);
   cout << "\n test date 1999/200n= " << test << '\n';
   adding_test(day, month, year, test, true);
   cout << "\n test date 1999/200n= " << test << '\n';
   
   test = {-3, Date::Month::dec, 31};
   cout << "\n test date BC/AD= " << test << '\n';
   adding_test(day, month, year, test, false);
   cout << "\n test date BC/AD= " << test << '\n';
   adding_test(day, month, year, test, true);
   
   test = {-1, Date::Month::feb, 29};
   cout << "\n test date BC/AD= " << test << '\n';
   adding_test(day, month, year, test, false);
   cout << "\n test date BC/AD= " << test << '\n';
   adding_test(day, month, year, test, true);
   
   cout << "\n test date BC/AD= " << test << '\n';
}

void day_test(const Date& date, const Day expected_day) {
   Day day = day_of_week(date);
   cout << "date = " << date << " is " << day << '\n';
   check_assertion(day, expected_day, "day");
   //assert(day == expected_day && "day != Day::tuesday");
}

void day_test() {
   //day_test(Date{2004, Date::feb, 29}, Day::sunday);
   day_test(Date{2001, Date::may, 20}, Day::sunday);
   day_test(Date{2001, Date::jan, 2}, Day::tuesday);
   day_test(Date{2000, Date::feb, 29}, Day::tuesday);
   day_test(Date{2021, Date::sep, 20}, Day::monday);
   //Date date = Date{2000, Date::feb, 29};
   //cout << "default date day of week = " << day_of_week(def) << '\n';
   //day_of_week();
}

void test() {
   standard_test();/*
   test_input();
   adding_test(true, false, false);
   adding_test(false, true, false);
   adding_test(false, false, true);*/
   day_test();
}

int main() {
   try {
      test();
      return 0;
   }
   catch (Date::Invalid& e) {
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
