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

bool Date::operator==(const Date& other) {
   return year()==other.year()
      && month()==other.month()
      && day()==other.day();
}

//------------------------------------------------------------------------------

bool Date::operator!=(const Date& other) {
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

bool Date::operator>(const Date& other) {
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
   cerr << *this << endl;
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

//const string Date::NN = "kk";

const char* Date::MONTH_NAMES[] = {
   "January","February","March","April","May","June","July",
   "August","September","October","November","December"
};

/*
enum Day {
   sunday, monday, tuesday, wednesday, thursday, friday, saturday
};

constexpr Day default_date_Day = tuesday;

long days_difference(const Date& first, const Date& second) {
   
}

Day day_of_week(const Date& other) {
   Date default_d = default_date();
   if (default_d == other)
      return default_date_Day;
   bool greater = false;
   if (other > default_d) 
      greater = true;
   
   return d;
}

//------------------------------------------------------------------------------

Date next_Sunday(const Date& d) {
    // ...
    return sunday;
}

//------------------------------------------------------------------------------

Date next_weekday(const Date& d) {
    // ...
    return d;
}
*/
}
