#include <iostream>
#include <string>

#include "date.h"

using namespace std;

namespace Chrono {

bool leapyear(int y) {
   if (! is_valid_year(y))
      throw Date::Invalid(make_message("Year can not be ", FORBIDDEN_YEAR));
   if (y < FORBIDDEN_YEAR)    // to convenient calculate of leap year assume years BC 
      y++;       // increased by 1
   
   if (y % 4 == 0 && y % 100 != 0)
      return true;
   if (y % 400 == 0)
      return true;
   
   return false;
}

int get_max_day(Date::Month m, int y) {
   if (! is_valid_year(y))
      throw Date::Invalid(make_message("Year can not be ", FORBIDDEN_YEAR));
   switch(m) {
      case Date::jan: case Date::mar: case Date::may: case Date::jul:
      case Date::aug: case Date::oct: case Date::dec:
         return 31;
      case Date::apr: case Date::jun: case Date::sep: case Date::nov:
         return 30;
      case Date::feb:
         return leapyear(y) ? 29 : 28;
      default:
         throw runtime_error("Month must be in range 1 - 12");
   }
}

bool is_date_valid(int y, Date::Month  m, int d) {
   if (! is_valid_year(y))
      return false;
   int max_day = get_max_day(m, y);
   if (d < 1 || d > max_day)
      return false;

   return true;
} 

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

enum Relation { MINOR=-1, EQUAL, MAJOR };

Relation get_relation(int first, int second) {
   if (first > second)
      return MAJOR;
   else if (first < second)
      return MINOR;
   return EQUAL;
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
   date = Date(y,Date::Month(m),d);     // update dd
   return is;
}

const Date& default_date() {
   static const Date dd(2001,Date::jan,1); // start of 21st century
   return dd;
}

//------------------------------------------------------------------------------

Date::Date()
   : y(default_date().year()),
     m(default_date().month()),
     d(default_date().day())
{
}

void Date::validate() {
   if (! is_date()) {
      string msg = to_string(y) + ", " + to_string(m) + ", " + to_string(d) + " is incorrect date\n";
      throw Invalid(msg);
   }
}

Date::Date(int yy, Month mm, int dd) : y(yy), m(mm), d(dd) {
   validate();
}

void Date::set_previous_year() {
   if (29 == d && feb == m)
      d = 28;
   y--;
   if (FORBIDDEN_YEAR == y)
      y--;
   validate();
}

void Date::set_next_year() {
   if (29 == d && feb == m) {
      m = mar;
      d = 1;
   }
   y++;
   if (FORBIDDEN_YEAR == y)
      y++;
   validate();
}

void Date::add_year(int n) {
   int next_year = y + n;
   if (y < FORBIDDEN_YEAR && next_year >= FORBIDDEN_YEAR)
      next_year++;
   if (y > FORBIDDEN_YEAR && next_year <= FORBIDDEN_YEAR)
      next_year--;
   y = next_year;
   if (29 == d && feb == m && ! leapyear(y)) {
      m = mar;
      d = 1;
   }
   validate();
}

void Date::set_previous_day() {
   if (d != 1)
      d--;
   else {
      --m;
      if (dec == m)
         --y;
      if (FORBIDDEN_YEAR == y)
         y--;
      d = get_max_day(m, y);
   }
   validate();
}

void Date::set_next_day() {
   int max_day = get_max_day(m, y);
   if (d < max_day)
      d++;
   else {
      d = 1;
      ++m;
      if (jan == m)
         ++y;
      if (FORBIDDEN_YEAR == y)
         y++;
   }
   validate();
}

void Date::add_day(int n) {
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
   --m;
   if (dec == m)
      --y;
   if (FORBIDDEN_YEAR == y)
      y--;
   if (29 <= d) {
      int month_max_day = get_max_day(m, y);
      if (d > month_max_day)
         d = month_max_day;
   }
   validate();
}

void Date::set_next_month() {
   ++m;
   if (jan == m)
      ++y;
   if (FORBIDDEN_YEAR == y)
      y++;
   if (29 <= d) {
      int month_max_day = get_max_day(m, y);
      if (d > month_max_day)
         d = month_max_day;
   }
   validate();
}

void Date::add_month(int n) {
   // increase by n months
   if (n > 0) {
      for (int counter = 0; counter < n; counter++) {
         cerr << *this << endl;
         set_next_month();
      }
   }
   else {  // decrease by n months
      for (int counter = 0; counter > n; counter--) {
         cerr << *this << endl;
         set_previous_month();
      }
   }
   cerr << *this << endl;
}

const char* Date::MONTH_NAMES[] = {
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
   Date::Month month = date.month();
   int year = date.year();
   for (Date::Month counter = Date::jan; counter < month; ++counter)
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
   //cerr << "years_diff = " << years_diff << '\n';
   for (int year = greater.year() - 1; years_diff > 1; year--, years_diff--)
      result += how_many_days(year);
   return result;
}

int days_difference(const Date& first, const Date& second) {
   //if (first == second)
   //   return 0;
   Date greater, smaller;
   date_order(first, second, greater, smaller);
   int years_diff = greater.year() - smaller.year();
   if (0 == years_diff)
      return day_in_year(greater) - day_in_year(smaller);
   int result = days_by_whole_years(first, second);
   //cerr << "result = " << result << '\n';
   result +=  days_to_end_year(smaller) + day_in_year(greater);
   //cerr << "days_to_end_year(smaller) = " << days_to_end_year(smaller) << '\n';
   //cerr << "day_in_year(greater) = " << day_in_year(greater) << '\n';
   return result;
}

Day day_of_week(const Date& other) {
   Date default_d = default_date();
   Day result = default_date_Day;
   int diff = days_difference(other, default_d);
   //cerr << "diff = " << diff << '\n';
   int offset = diff % 7;
   //cerr << "offset = " << offset << '\n';
   bool default_greater = false;
   if (default_d > other) 
      default_greater = true;
   //cerr << "default_greater = " << default_greater << '\n';
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

}
