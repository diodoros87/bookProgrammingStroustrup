#include <iostream>
#include <cassert>
#include <string>
#include <ctime>
#include <cstring>

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
   check_assertion(int(date.month()), m, "month");
   check_assertion(date.year(), y, "year");
}

void test_incorrect(Date& date) {
   int d = date.day();
   int m = static_cast<int>(date.month());
   int y = date.year();
   try {
      date = { 1969, Month::feb, 1 };
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
   Date today(1978, Month::jul, 20); 
   cerr << "today = " << today << endl;
   check_assertion(today, 1978, static_cast<int>(Month::jul), 20);
   Date tomorrow = today; 
   check_assertion(tomorrow, 1978, static_cast<int>(Month::jul), 20);
   tomorrow.add_day(1);
   check_assertion(tomorrow, 1978, static_cast<int>(Month::jul), 21);
   cout << "\n tomorrow = " << tomorrow << '\n';
   
   tomorrow.add_day(-1);
   check_assertion(tomorrow, 1978, static_cast<int>(Month::jul), 20);
   cout << "\n tomorrow = " << tomorrow << '\n';
   
   today = {2000, Month::apr, 7};
   cerr << "today = " << today << endl; 
   
   Date d; 
   cerr << "default = " << d << endl;
   
   const Date start = start_date(); 
   cerr << "start_date = " << start << endl;
   
   Date def; 
   cerr << "default = " << def << endl;
   
   Date start2 = start_date(); 
   cerr << "start_date = " << start2 << endl;
   
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
   Date test = {2000, Month::feb, 29};
   cout << "\n test date 1999/200n= " << test << '\n';
   adding_test(day, month, year, test, false);
   cout << "\n test date 1999/200n= " << test << '\n';
   adding_test(day, month, year, test, true);
   cout << "\n test date 1999/200n= " << test << '\n';
}

void test_by_ctime (const Date& date, const Day result_day) {
   if (date.year() < 1900)
      throw runtime_error("test_by_ctime precondition: date.year() >= 1900");
  time_t rawtime;
  struct tm * timeinfo;

  /* get current timeinfo and modify it to the user's choice */
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  timeinfo->tm_year = date.year() - 1900;
  timeinfo->tm_mon = static_cast<int>(date.month()) - 1;
  timeinfo->tm_mday = date.day();

  /* call mktime: timeinfo->tm_wday will be set */
  mktime ( timeinfo );

  printf ("That day is a %s.\n", DAY_NAMES[timeinfo->tm_wday]);
  check_assertion(result_day, timeinfo->tm_wday, "day by ctime");
  cerr << "result_day = " << result_day << " timeinfo->tm_wday = " << timeinfo->tm_wday << '\n';
  assert(strcmp (DAY_NAMES[timeinfo->tm_wday], DAY_NAMES[result_day]) == 0);
}

void day_test(const Date& date, const Day expected_day) {
   cout << "BEGIN TEST "  << '\n';
   Day day = day_of_week(date);
   cout << "date = " << date << " is " << day << '\n';
   check_assertion(day, expected_day, "day");
   test_by_ctime(date, day);
   Date sunday = next_Sunday(date);
   cout << "next sunday = " << sunday << '\n';
   Date weekday = next_weekday(date);
   cout << "next weekday = " << weekday << '\n';
   Date workday = next_workday(date);
   cout << "next workday = " << workday << '\n';
   cout << "END TEST "  << '\n';
}

void day_test() {
   day_test(Date{2004, Month::feb, 29}, Day::sunday);
   day_test(Date{2001, Month::may, 20}, Day::sunday);
   day_test(Date{2001, Month::jan, 2}, Day::tuesday);
   day_test(Date{2000, Month::feb, 29}, Day::tuesday);
   day_test(Date{2021, Month::sep, 20}, Day::monday);
   day_test(Date{2021, Month::sep, 22}, Day::wednesday);
   day_test(Date{2020, Month::sep, 24}, Day::thursday);
   day_test(Date{2020, Month::dec, 25}, Day::friday);
   day_test(Date{2020, Month::dec, 26}, Day::saturday);
   day_test(Date{1970, Month::jan, 1},  Day::thursday);
   day_test(Date{1995, Month::feb, 28}, Day::tuesday);
   day_test(Date(), Day::monday);
}

void week_of_year_test(const Date& date, const int expected_week) {
   cout << "BEGIN TEST "  << '\n';
   unsigned int week = week_of_year(date);
   cout << "date = " << date << " is " << week << " week" << '\n';
   check_assertion(week, expected_week, "day");
   cout << "END TEST "  << '\n';
}

void week_of_year_test() {
   week_of_year_test(Date{2001, Month::jan, 2}, 1);
   week_of_year_test(Date{2001, Month::jan, 1}, 1);
   week_of_year_test(Date{2001, Month::jan, 7}, 1);
   week_of_year_test(Date{2001, Month::jan, 8}, 2);
   week_of_year_test(Date{2021, Month::sep, 20}, 39);
   week_of_year_test(Date{2021, Month::jan, 4}, 2);
   week_of_year_test(Date{2021, Month::jan, 3}, 1);
   week_of_year_test(Date{2021, Month::dec, 31}, 53);
}

void test() { 
   standard_test();
   test_input();
   adding_test(true, false, false);
   adding_test(false, true, false);
   adding_test(false, false, true);
   day_test();
   week_of_year_test();
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
