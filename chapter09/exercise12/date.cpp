#include <iostream>
#include <string>

#include "date.h"

using namespace std;

namespace Chrono {

bool leapyear(int y) {
   if (! is_valid_year(y))
      throw Date::Invalid(make_message("Year can not be < ", START_YEAR));
   if (y % 4 == 0 && y % 100 != 0)
      return true;
   if (y % 400 == 0)
      return true;
   
   return false;
}

int get_max_day(Month m, int y) {
   if (! is_valid_year(y))
     throw Date::Invalid(make_message("Year can not be < ", START_YEAR));
   switch(m) {
      case Month::jan: case Month::mar: case Month::may: case Month::jul:
      case Month::aug: case Month::oct: case Month::dec:
         return 31;
      case Month::apr: case Month::jun: case Month::sep: case Month::nov:
         return 30;
      case Month::feb:
         return leapyear(y) ? 29 : 28;
      default:
         throw runtime_error("Month must be in range 1 - 12");
   }
}

bool is_date_valid(int y, Month  m, int d) {
   if (! is_valid_year(y))
      return false;
   int max_day = get_max_day(m, y);
   if (d < 1 || d > max_day)
      return false;

   return true;
} 
/*
struct ymd_date {
   int d;
   Month m;
   int y;
};*/

Month int_to_month(int x) {
   if (static_cast<int>(Month::jan) > x || static_cast<int>(Month::dec) < x)
      throw Date::Invalid(make_message("Month must be in range 1 - 12, can not be ", x));
   return static_cast<Month>(x);
}

void validation(int y, Month m, int d) {
   if (! is_date_valid(y, m, d)) {
      string msg = to_string(y) + ", " + MONTH_NAMES[int(m) - 1] + ", " + to_string(d) + " is incorrect date\n";
      throw Date::Invalid(msg);
   }
}

int Date::year_or_month_or_day(bool get_year, bool get_month) const {
   int year = START_YEAR;
   long int days_counter = how_many_days(year);
   long int all_days = days + 1;   // add 1(START_DAY) because 1st January 1970 is days = 0
   while (all_days > days_counter) 
      days_counter += how_many_days(++year);  // sum of all days in year (from jan to dec)
   if (! is_valid_year(year))
      throw Invalid(make_message("Year can not be < ", START_YEAR));
   if (get_year)
      return year;
   
   days_counter -= how_many_days(year); // go back to begin year (Month::jan)
   Month month = Month::jan;
   days_counter += get_max_day(month, year);
   while (all_days > days_counter) {
      days_counter += get_max_day(++month, year);
      if (month == Month::jan)
         throw Invalid(make_message("Another year has been reached ", MONTH_NAMES[int(month) - 1]));
   }
   if (get_month)
      return int(month);

   days_counter -= get_max_day(month, year); // go back to begin month
   int day = all_days - days_counter;
   validation(year, month, day);
   return day;
}
/*
int Date::day()   const {
   return year_or_month_or_day(false, false);
}

Month Date::month()   const {
   return year_or_month_or_day(false, true);
}

int Date::year()   const {
   return year_or_month_or_day(true, false);
}
*/
bool Date::is_date() {
   return is_date_valid(year(), month(), day());
}

bool Date::operator==(const Date& other) const {
   return year()==other.year()
      && month()==other.month()
      && day()==other.day();
}

//------------------------------------------------------------------------------

bool Date::operator!=(const Date& other) const {
   return !(*this==other);
}

bool Date::operator>(const Date& other) const {
   if (year() != other.year())
      return year() > other.year() ? true : false;
   if (month() != other.month())
      return month() > other.month() ? true : false;
   if (day() != other.day())
      return day() > other.day() ? true : false;
   
   return false;
}

//------------------------------------------------------------------------------

ostream& operator<<(ostream& os, const Date& d) {
   return os << '(' << d.year()
            << ',' << d.month()
            << ',' << d.day() 
            << ')';
}

//------------------------------------------------------------------------------

istream& operator>>(istream& is, Date& date) {
   int y, m, d;
   char ch1, ch2, ch3, ch4;
   is >> ch1 >> y >> ch2 >> m >> ch3 >> d >> ch4;
   if (cin.eof())
      return is;
   if (!is) {
      cerr << "format error: for year or month or day data entered was not integer\n";
      return is;
   }
   if (ch1!='(' || ch2!=',' || ch3!=',' || ch4!=')') { // oops: format error
      cerr << "format error: date must be in format (Y,M,D)\n";
      is.clear();
      //is.clear(ios_base::failbit);                    // set the fail bit
      return is;
   }
   date = Date(y, int_to_month(m),d);     // update dd
   return is;
}

const Date& start_date() {
   static const Date start(START_YEAR, START_MONTH, START_DAY);
   //static const Date start(1970, Month::jan, 1);
   return start;
}

const Date& default_date() {
   static const Date dd(2001,Month::jan,1); // start of 21st century
   return dd;
}

//------------------------------------------------------------------------------

unsigned int day_in_year(int y, Month m, int d) {
   validation(y, m, d);
   unsigned int result = d;
   for (Month counter = Month::jan; counter < m; ++counter)
      result += get_max_day(counter, y);
   return result;
}

unsigned int days_to_end_year(int y, Month m, int d) {
   validation(y, m, d);
   return how_many_days(y) - day_in_year(y, m, d);
}

long int how_many_days(int y, Month m, int d) {
   validation(y, m, d);
   if (y == START_YEAR)
      return day_in_year(y, m, d) - day_in_year(START_YEAR, START_MONTH, START_DAY);
   long int result = 0;   // 1st Jan 1970 is day 0
   for (int year = y - 1; year > START_YEAR; year--)  // number of days between whole years
      result += how_many_days(year);
   result +=  days_to_end_year(START_YEAR, START_MONTH, START_DAY) + day_in_year(y, m, d);
   return result;
}

long int days_difference(const Date& first, const Date& second);

Date::Date() {
   days = how_many_days(default_date().year(), default_date().month(), default_date().day());
   validate();
}

void Date::validate() {
   if (! is_date()) {
      string msg = to_string(year()) + ", " + MONTH_NAMES[int(month()) - 1] + ", " + 
                  to_string(day()) + " is incorrect date\n";
      throw Invalid(msg);
   }
}

Date::Date(int y, Month m, int d) {
   validation(y, m, d);
   days = how_many_days(y, m, d);
}

void Date::set_previous_year() {
   int d = day();
   Month m = month();
   int y = year();
   if (29 == d && Month::feb == month())
      d = 28;
   y--;
   long diff = days_difference(*this, Date(y, m, d));
   days -= diff;
   validate();
}

void Date::set_next_year() {
   int d = day();
   Month m = month();
   int y = year();
   if (29 == d && Month::feb == m) {
      m = Month::mar;
      d = 1;
   }
   y++;
   long diff = days_difference(*this, Date(y, m, d));
   days += diff;
   validate();
}

void Date::add_year(int n) {
   int d = day();
   Month m = month();
   int y = year() + n;
   if (29 == d && Month::feb == m && ! leapyear(y)) {
      m = Month::mar;
      d = 1;
   }
   long diff = days_difference(*this, Date(y, m, d));
   if (n > 0)
      days += diff;
   else
      days -= diff;
   validate();
}

void Date::set_previous_day() {
   /*
   int d = day();
   Month m = month();
   int y = year();
   if (d != 1)
      d--;
   else {
      --m;
      if (Month::dec == m)
         --y;
      d = get_max_day(m, y);
   }
   */
   --days;
   validate();
}

void Date::set_next_day() {
   /*
   int max_day = get_max_day(m, y);
   if (d < max_day)
      d++;
   else {
      d = 1;
      ++m;
      if (Month::jan == m)
         ++y;
   }*/
   ++days;
   validate();
}

void Date::add_day(int n) {
   //days += n;
   //validate();
   // increase by n days
   if (n > 0) {
      for (int counter = 0; counter < n; counter++) {
         //cerr << *this << endl;
         set_next_day();
      }
   }
   else {  // decrease by n days
      for (int counter = 0; counter > n; counter--) {
         //cerr << *this << endl;
         set_previous_day();
      }
   }
   //cerr << *this << endl;
}

void Date::set_previous_month() {
   int d = day();
   Month m = month();
   int y = year();
   --m;
   if (Month::dec == m)
      --y;
   if (29 <= d) {
      int month_max_day = get_max_day(m, y);
      if (d > month_max_day)
         d = month_max_day;
   }
   long diff = days_difference(*this, Date(y, m, d));
   days -= diff;
   validate();
}

void Date::set_next_month() {
   int d = day();
   Month m = month();
   int y = year();
   ++m;
   if (Month::jan == m)
      ++y;
   if (29 <= d) {
      int month_max_day = get_max_day(m, y);
      if (d > month_max_day)
         d = month_max_day;
   }
   long diff = days_difference(*this, Date(y, m, d));
   days += diff;
   validate();
}

void Date::add_month(int n) {
   // increase by n months
   if (n > 0) {
      for (int counter = 0; counter < n; counter++) {
         //cerr << *this << endl;
         set_next_month();
      }
   }
   else {  // decrease by n months
      for (int counter = 0; counter > n; counter--) {
         //cerr << *this << endl;
         set_previous_month();
      }
   }
   //cerr << *this << endl;
}

const char* MONTH_NAMES[] = {
   "January","February","March","April","May","June","July",
   "August","September","October","November","December"
};

const char* DAY_NAMES[] = {
   "sunday","monday","tuesday","wednesday","thursday","friday","saturday"
};

// prefix increment operator
inline Day operator++(Day& day)  {
   day = (day==Day::saturday) ? Day::sunday : Day(day+1);             // "wrap around"
   return day;
}

// prefix decrement operator
inline Day operator--(Day& day)  {
   day = (day==Day::sunday) ? Day::saturday : Day(day-1);             // "wrap around"
   return day;
}

constexpr Day default_date_Day = monday;

unsigned int day_in_year(const Date& date) {
   unsigned int result = date.day();
   Month month = date.month();
   int year = date.year();
   for (Month counter = Month::jan; counter < month; ++counter)
      result += get_max_day(counter, year);
   return result;
}

void date_order(const Date& first, const Date& second, Date& greater, Date& not_greater) {
   if (first > second) {
      greater = first;
      not_greater = second;
   }
   else {
      greater = second;
      not_greater = first;
   }
}

int days_by_whole_years(const Date& first, const Date& second) {
   Date greater, smaller;
   date_order(first, second, greater, smaller);
   int result = 0;
   int years_diff = greater.year() - smaller.year();
   for (int year = greater.year() - 1; years_diff > 1; year--, years_diff--)
      result += how_many_days(year);
   return result;
}

long int days_difference(const Date& first, const Date& second) {
   Date greater, smaller;
   date_order(first, second, greater, smaller);
   int years_diff = greater.year() - smaller.year();
   if (0 == years_diff)
      return day_in_year(greater) - day_in_year(smaller);
   int result = days_by_whole_years(first, second);
   result +=  days_to_end_year(smaller) + day_in_year(greater);
   return result;
}

Day day_of_week(const Date& other) {
   Date default_d = default_date();
   Day result = default_date_Day;
   int diff = days_difference(other, default_d);
   int offset = diff % 7;
   bool default_greater = false;
   if (default_d > other) 
      default_greater = true;
   for (int i = 0; i < offset; i++)
      if (default_greater)
         --result;
      else 
         ++result;
   return result;
}

//------------------------------------------------------------------------------

Date next_Sunday(const Date& date) {
   Day day = day_of_week(date);
   Date sunday {date}; 
   int offset = 0;
   if (day == Day::sunday)
      offset = 7;
   else
      for (; day != Day::sunday; ++day)
         offset++;
   sunday.add_day(offset);
   return sunday;
}

//------------------------------------------------------------------------------

Date next_weekday(const Date& date) {
   Date next {date}; 
   Day day = day_of_week(date);
   int offset = 0;
   if (day == Day::monday)
      offset = 7;
   else
      for (; day != Day::monday; ++day)
         offset++;
   next.add_day(offset);
   return next;
}

Date next_workday(const Date& date) {
   Date next {date}; 
   Day day = day_of_week(date);
   int offset = 1;
   switch(day) {
      case Day::friday :
         offset++;
      case Day::saturday :
         offset++;
   }
   next.add_day(offset);
   return next;
}

// every week in every year (except 1st week) must start on Day::monday
// week 1 is first week in year - may contains less than 7 days
// // Day::sunday is last day in week
Date start_week(int week, int year) {
   if (1 > week)
      throw runtime_error("number of week in year can not be < 1");
   Date result(year, Month::jan, 1);
   for (int counter = 1; counter < week; counter++) {
      if (year != result.year())
         throw runtime_error("number of week in year is too big");
      result = next_weekday(result);
   }
   return result;
}

// every week in every year (except 1st week) must start on Day::monday
// week 1 is first week in year - may contains less than 7 days
// // Day::sunday is last day in week
unsigned int week_of_year(const Date& date) {
   unsigned int week = 1;
   Date temp;
   do {
      temp = start_week(++week, date.year()); 
      if (temp > date) // if start of next week is after than parameter date then:
         return --week;  // return previous week
   } while (true);
}

}
