//#include <string>

using std::string;
using std::to_string;
using std::runtime_error;
using std::exception;
using std::ostream;

constexpr int FORBIDDEN_YEAR = 0;

inline void error(const string& MSG, const int X) {
   const string message = MSG + to_string(X); 
   throw runtime_error(message);
}

inline const char* make_message(const int X, const string& SEPARATOR, const int Y, const string& MSG, const int Z) {
   string message = to_string(X) + SEPARATOR + to_string(Y) + SEPARATOR + to_string(Z) + MSG; 
   return message.c_str();
}
/*
inline bool is_valid_year(int y) {
   if (FORBIDDEN_YEAR == y)
      return false;
   return true;
}
*/
inline bool is_valid_month(int m) {
   if (1 > m || 12 < m)
      return false;
   return true;
}

class Invalid : public exception { 
   string msg {"Invalid date: "};
public:
   Invalid(const string& message) { msg += message; }
   const char* what() {
      return msg.c_str();
   }
};

class Year {        // year in [MIN:MAX) range
   static constexpr int MIN = 1800;
   static constexpr int MAX = 2200;
public:
   Year(int x)  { 
      validate(x);
      y = x; 
   }
   int year() const { return y; }
   Year& operator++()  {
      if (MAX > y)
         ++y;
      return *this;
   }
   Year& operator--()  {
      if (MIN < y)
         --y;
      return *this;
   }
   
   static void validate(int x);
private:
   int y;
};

inline void Year::validate(int x) { 
   if (x < Year::MIN || Year::MAX < x) 
      throw Invalid("Wrong year: year must be from " + to_string(MIN) + " to " + to_string(MAX)); 
}

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------



class Date {
public:
   enum Month {
      jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
   };

   Date(Year yy, Month mm, int dd) : y(yy), m(mm), d(dd) {
      if (! is_date()) {
         const string MSG = make_message(y.year(), ", ", m, " is incorrect date\n", d);
         throw Invalid(MSG);
      }
   }
   
   bool is_date();
   void add_day(int n);                // increase or decrease the Date by n days
   void set_previous_day();
   void set_next_day();
   int month() const { return m; }
   int day()   const { return d; }
   int year()  const { return y.year(); }
   Month get_month() const { return m; }

private:
   Year y;
   Month m;
   int d;   // day
};

// prefix increment operator
inline Date::Month operator++(Date::Month& m)  {
   m = (m==Date::dec) ? Date::Month::jan : Date::Month(m+1);             // "wrap around"
   return m;
}

// prefix decrement operator
inline Date::Month operator--(Date::Month& m)  {
   m = (m==Date::jan) ? Date::Month::dec : Date::Month(m-1);             // "wrap around"
   return m;
}

inline ostream& operator<<(ostream& os, Date::Month m) {
   static const char* month_tbl[12] = {
      "January","February","March","April","May","June","July",
      "August","September","October","November","December"
   };
   return os << month_tbl[m - 1];
}

ostream& operator<<(ostream& os, const Date& d);
