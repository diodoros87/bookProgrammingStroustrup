using std::string;
using std::to_string;
using std::runtime_error;
using std::exception;
using std::ostream;
using std::istream;

namespace Chrono {
class Date;   
const Date& start_date();
//constexpr int START_YEAR = start_date().year();
constexpr int START_YEAR = 1970;
constexpr int START_DAY = 1;

inline string make_message(const string& MSG, const int X) {
   const string message = MSG + to_string(X); 
   return message;
}

inline string make_message(const string& MSG, const string& MESSAGE) {
   const string message = MSG + MESSAGE; 
   return message;
}

inline bool is_valid_year(int y) {
   return START_YEAR < y ? false : true;
}

inline bool is_valid_month(int m) {
   return (1 > m || 12 < m) ? false : true;
}

bool leapyear(int y);

inline unsigned int how_many_days(int year) {
   return leapyear(year) ? 366 : 365;
}

const char* MONTH_NAMES[] = {
   "January","February","March","April","May","June","July",
   "August","September","October","November","December"
};

enum class Month {
   jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
};

constexpr Month START_MONTH = Month::jan;

Month int_to_month(int x);

class Date {
public:
   class Invalid : public exception { 
      string msg {"Invalid date: "};
   public:
      Invalid(const string& message) { msg += message; }
      const char* what() {
         return msg.c_str();
      }
   };
   
   Date(int yy, Month mm, int dd);
   Date();
   bool is_date();
   void validate();
   
   void add_day(int n);                // increase or decrease the Date by n days
   void add_month(int n);
   void add_year(int n);
   
   void set_previous_day();
   void set_next_day();
   void set_previous_month();
   void set_next_month();
   void set_previous_year();
   void set_next_year();
   
   int year_or_month_or_day (bool get_year, bool get_month) const;
   int day()   const   { return year_or_month_or_day(false, false); }
   Month month() const { return int_to_month(year_or_month_or_day(false, true));  }
   int year()  const   { return year_or_month_or_day(true, false); }
   
   bool operator==(const Date& other) const ;
   bool operator!=(const Date& other) const ;
   bool operator>(const Date& other) const ;
private:
   long int days {0};  
};

unsigned int day_in_year(const Date& date);

inline unsigned int days_to_end_year(const Date& date) {
   return how_many_days(date.year()) - day_in_year(date);
}

// prefix increment operator
inline Month operator++(Month& m)  {
   m = (m==Month::dec) ? Month::jan : int_to_month(static_cast<int>(m)+1);             // "wrap around"
   return m;
}

// prefix decrement operator
inline Month operator--(Month& m)  {
   m = (m==Month::jan) ? Month::dec : int_to_month(static_cast<int>(m)-1);             // "wrap around"
   return m;
}

inline ostream& operator<<(ostream& os, Month m) {
   return os << MONTH_NAMES[int(m) - 1];
}

ostream& operator<<(ostream& os, const Date& d);
istream& operator>>(istream& is, Date& date);

enum Day {
   sunday, monday, tuesday, wednesday, thursday, friday, saturday
};

const char* DAY_NAMES[] = {
   "sunday","monday","tuesday","wednesday","thursday","friday","saturday"
};

inline ostream& operator<<(ostream& os, Day day) {
   return os << DAY_NAMES[day];
}

Day day_of_week(const Date& other);
Date next_Sunday(const Date& date);
Date next_weekday(const Date& date);
Date next_workday(const Date& date);
unsigned int week_of_year(const Date& date);

}
