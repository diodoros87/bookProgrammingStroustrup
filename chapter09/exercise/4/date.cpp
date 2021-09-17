#include <iostream>
#include <string>

#include "date.h"

using namespace std;

bool leapyear(int y) {
   Year::validate(y);
   /*if (! Year::validate(y))
   /   error("Year can not be ", FORBIDDEN_YEAR);
   if (y < 0)    // to convenient calculate of leap year assume years BC 
      y++;       // increased by 1
   */
   if (y % 4 == 0 && y % 100 != 0)
      return true;
   if (y % 400 == 0)
      return true;
   
   return false;
}

int get_max_day(int m, int y) {
   Year::validate(y);
   //if (! Year::validate(y))
   //   error("Year can not be ", FORBIDDEN_YEAR);
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
   try {
      Year::validate(y);
   }
   catch (Invalid&) {
      return false;
   }
   //if (! is_valid_year(y))
   //   return false;
   if (! is_valid_month(m))
      return false;
   int max_day = get_max_day(m, y);
   if (d < 1 || d > max_day)
      return false;
   
   return true;
}

//------------------------------------------------------------------------------

bool Date::is_date() {
   return is_valid(year(), month(), day());
}

ostream& operator<<(ostream& os, const Date& d) {
   return os << '(' << d.year()
            << ',' << d.get_month()
            << ',' << d.day() 
            << ')';
}
/*
Date::Date(int y, int m, int d) {
   // check that (y,m,d) is a valid date
   // if it is, use it to initialize date
   
   if (! is_valid(y, m, d)) {
      error(y, ", ", m, " is incorrect date\n", d);
      //cerr << y << ", " << m << ", " << d << " is incorrect date\n";
      //throw runtime_error("Wrong date ");
   }
   this->y = y;
   this->m = m;
   this->d = d;
}
*/
//------------------------------------------------------------------------------

void Date::set_previous_day() {
   if (d != 1)
      d--;
   else {
      --m;
      if (dec == m)
         --y;
      d = get_max_day(m, y.year());
   }
   if (! is_date()) {
      cerr << *this << " is incorrect date\n";
      throw runtime_error("Wrong date ");
   }
}

void Date::set_next_day() {
   int max_day = get_max_day(m, y.year());
   if (d < max_day)
      d++;
   else {
      d = 1;
      ++m;
      if (jan == m)
         ++y;
   }
   if (! is_date()) {
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
