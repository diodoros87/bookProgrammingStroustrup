#include <iostream>
#include <cassert>
#include <string>
#include <ctime>
#include <cstring>

#include "error.h"
#include "date.h"
#include "Book.h"
#include "Patron.h"
#include "Library.h"

using namespace std;
using namespace Chrono;

//#define NDEBUG

#ifndef NDEBUG
#   define M_Assert(Expr, Msg) \
    __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#   define M_Assert(Expr, Msg) ;
#endif

static void __M_Assert(const char* expr_str, const bool expr,
                const char* file, const int line, const string& msg) {
    if (! expr) {
      cerr << "\nAssert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n";
      exit(EXIT_FAILURE);
   }
}

static inline void check_assertion(int x, int expected_x, const string& LABEL) {
   const string message = " " + LABEL + " " + to_string(x) +
                     " != expected " + LABEL + " " + to_string(expected_x);
   M_Assert(x == expected_x, message);
}

static inline void check_assertion(const Date& date, int y, int m, int d) {
   check_assertion(date.day(), d, "day");
   check_assertion(date.month(), m, "month");
   check_assertion(date.year(), y, "year");
}

static void test_incorrect(Date& date) {
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

static Date get_date_from_input() {
   Date date;
   if (cin) {
      cout << "Enter date in format (Y,M,D) ";
      cin >> date;
   }
   return date;
}

static void test_input() {
   Date date_from_input = get_date_from_input();
   cout << "date from input = " << date_from_input << endl;
   date_from_input = get_date_from_input();
   cout << "date from input = " << date_from_input << endl;
}

static void standard_test() {
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

static void adding_test(bool day, bool month, bool year, Date& date, bool minus) {
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

static void adding_test(bool day, bool month, bool year) {
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



static void test_by_ctime (const Date& date, const Day result_day) {
   if (date.year() < 1900)
      throw runtime_error("test_by_ctime precondition: date.year() >= 1900");
  time_t rawtime;
  struct tm * timeinfo;

  /* get current timeinfo and modify it to the user's choice */
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  timeinfo->tm_year = date.year() - 1900;
  timeinfo->tm_mon = date.month() - 1;
  timeinfo->tm_mday = date.day();

  /* call mktime: timeinfo->tm_wday will be set */
  mktime ( timeinfo );

  printf ("That day is a %s.\n", DAY_NAMES[timeinfo->tm_wday]);
  check_assertion(result_day, timeinfo->tm_wday, "day by ctime");
  cerr << "result_day = " << result_day << " timeinfo->tm_wday = " << timeinfo->tm_wday << '\n';
  assert(strcmp (DAY_NAMES[timeinfo->tm_wday], DAY_NAMES[result_day]) == 0);
}

static void day_test(const Date& date, const Day expected_day) {
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

static void day_test() {
   day_test(Date{2004, Date::feb, 29}, Day::sunday);
   day_test(Date{2001, Date::may, 20}, Day::sunday);
   day_test(Date{2001, Date::jan, 2}, Day::tuesday);
   day_test(Date{2000, Date::feb, 29}, Day::tuesday);
   day_test(Date{2021, Date::sep, 20}, Day::monday);
   day_test(Date{2021, Date::sep, 22}, Day::wednesday);
   day_test(Date{2020, Date::sep, 24}, Day::thursday);
   day_test(Date{2020, Date::dec, 25}, Day::friday);
   day_test(Date{2020, Date::dec, 26}, Day::saturday);
   day_test(Date{1954, Date::jul, 5}, Day::monday);
   day_test(Date(), Day::monday);
}

static void week_of_year_test(const Date& date, const int expected_week) {
   cout << "BEGIN TEST "  << '\n';
   unsigned int week = week_of_year(date);
   cout << "date = " << date << " is " << week << " week" << '\n';
   check_assertion(week, expected_week, "day");
   cout << "END TEST "  << '\n';
}

static void week_of_year_test() {
   week_of_year_test(Date{2001, Date::jan, 2}, 1);
   week_of_year_test(Date{2001, Date::jan, 1}, 1);
   week_of_year_test(Date{2001, Date::jan, 7}, 1);
   week_of_year_test(Date{2001, Date::jan, 8}, 2);
   week_of_year_test(Date{2021, Date::sep, 20}, 39);
   week_of_year_test(Date{2021, Date::jan, 4}, 2);
   week_of_year_test(Date{2021, Date::jan, 3}, 1);
   week_of_year_test(Date{2021, Date::dec, 31}, 53);
}

static void date_test() { 
   standard_test();
   //test_input();   checked many times
   adding_test(true, false, false);
   adding_test(false, true, false);
   adding_test(false, false, true);
   day_test();
   week_of_year_test();
}

static Patron test_patron_charges() {
   Patron patron = Patron{"Alexander_3_Great", 333, 0};
   assert(patron.are_charges() == false && "patron.are_charges() != false");
   cout << patron;
   patron.set_charges(-45);
   assert(patron.are_charges() && "patron.are_charges() == false");
   cout << patron;
   patron.set_charges(+45);
   assert(patron.are_charges() && "patron.are_charges() == false");
   cout << patron;
   return patron;
}

static Patron test_patron_construction(const string& name, unsigned int n) {
   Patron patron = Patron{name, n};
   assert(patron.are_charges() == false && "patron.are_charges() != false");
   cout << patron;
   return patron;
}

static void test_incorrect_patron(const string& name, unsigned int n) {
   try {
      test_patron_construction(name, n);
      assert(false);
   }
   catch (Patron::Invalid_Patron& e) {
      cerr << "Exception catched: " << e.what() << endl;
   }
}

static vector<Patron> test_patron() {
   vector<Patron> v;
   v.push_back(test_patron_construction("Hipparch", 200));
   v.push_back(test_patron_construction("Aristarchus_of_Samos", 155));
   v.push_back(test_patron_construction("hades", 0));
   v.push_back(test_patron_construction("democrit_abdera", -747));
   v.push_back(test_patron_construction("Epictetus", 99));
   v.push_back(test_patron_construction("Musonius_Rufus", 66));
   v.push_back(test_patron_construction("socrates", 405));
   v.push_back(test_patron_construction("heraclitus", 500));
   
   test_incorrect_patron("eric", 33);
   test_incorrect_patron("Ur", 111);
   test_incorrect_patron("3ertfgf", 77);
   test_incorrect_patron("democrit abdera", 747);
   test_incorrect_patron("democrit-abdera", 747);
   return v;
}

static void test_incorrect_book_return(Book& book) {
   if (book.is_borrow())
      return;
   try {
      book.return_book();
      assert(false);
   }
   catch (Book::Invalid_Operation& e) {
      cerr << "Exception catched: " << e.what() << endl;
   }
}

static void test_incorrect_book_borrow(Book& book) {
   if (! book.is_borrow())
      return;
   try {
      book.borrow();
      assert(false);
   }
   catch (Book::Invalid_Operation& e) {
      cerr << "Exception catched: " << e.what() << endl;
   }
}

static void test_book_borrowing() {
   Book meditations = Book{"Marcus Aurelius", "Meditations", Book::Genre::Philosophy, 
      "1-2-3-5", Date{172, Date::jan, 4}, false};
   cout << book_status(meditations);
   test_incorrect_book_return(meditations);
   cout << book_status(meditations);
   meditations.borrow();
   cout << book_status(meditations);
   test_incorrect_book_borrow(meditations);
   cout << book_status(meditations);
   meditations.return_book();
   cout << book_status(meditations);
}

static Book test(const string& a, const string& t, Book::Genre g, const string& isbn, const Date& d) {
   Book book = Book{a, t, g, isbn, d};
   cout << book;
   return book;
}

static void test_incorrect_books(const string& a, const string& t, Book::Genre g, const string& isbn, const Date& d) {
   try {
      test(a, t, g, isbn, d);
      assert(false);
   }
   catch (Book::Invalid_Book& e) {
      cerr << "Exception catched: " << e.what() << endl;
   }
}

static vector<Book> test_book_construction() {
   vector<Book> v;
   v.push_back(test("Aristotle", "Metaphysics", Book::Genre::Philosophy, "1-2-3-5", Date{-343, Date::jan, 4}));
   v.push_back(test("Nicolaus Copernicus", "De revolutionibus orbium coelestium", 
        Book::Genre::Astronomy, "14-24-34-c", Date{1543, Date::dec, 6}));
   v.push_back(test("Lucius Annaeus Seneca", "Epistulae morales ad Lucilium", 
        Book::Genre::Philosophy, "544-24-34-N", Date{63, Date::dec, 16}));
   
   test_incorrect_books("Aristotle", "Incorrect ISBN", Book::Genre::Philosophy, "1-2-3-54", Date{-343, Date::jan, 4});
   test_incorrect_books("Aristotle", "Incorrect ISBN", Book::Genre::Philosophy, "1+2-3-5", Date{2021, Date::sep, 22});
   test_incorrect_books("Aristotle", "Today", Book::Genre::Philosophy, "1-2-3-5", get_today());
   
   test_incorrect_books("Euclid", "Incorrect ISBN",  Book::Genre::Mathematics,
        "544b-24-34-N", Date{-300, Date::jun, 30});
   test_incorrect_books("E", "Incorrect Name",  Book::Genre::Mathematics,
        "544-24-34-N", Date{-300, Date::jun, 30});
   test_incorrect_books("E v", "Incorrect Name",  Book::Genre::Mathematics,
        "544-24-34-N", Date{-300, Date::jun, 30});
   test_incorrect_books("we", "Incorrect Name",  Book::Genre::Mathematics,
        "544-24-34-N", Date{-300, Date::jun, 30});
   test_incorrect_books("we", "Incorrect Name",  Book::Genre::Mathematics,
        "544-24-34-N", Date{-300, Date::jun, 30});
   
   v.push_back(test("Euclid", "Elements",  Book::Genre::Mathematics, "544-24-34-M", Date{-300, Date::jun, 30}));
   v.push_back(test("Aristotle", "Nicomachean Ethics", Book::Genre::Philosophy, "1-2-3-n", Date{-343, Date::jan, 4}));
   
   v.push_back(test("Claudius Ptolemaeus", "Almagest", Book::Genre::Astronomy, "0-2-3-5", Date{160, Date::jan, 4}));
   v.push_back(test("Bjarne Stroustrup", "C++ Language", Book::Genre::Computer_Science, "11-98-79-c", Date{2012, Date::dec, 4}));
   v.push_back(test("Cay S. Horstmann", "Java Fundamentals", Book::Genre::Computer_Science, "1-2-3-j", Date{2016, Date::dec, 4}));
   v.push_back(test("Aristotle", "Organon", Book::Genre::Philosophy, "1-2-3-o", Date{-342, Date::sep, 4}));
   v.push_back(test("Plato", "Fedon", Book::Genre::Philosophy, "1-2-3-f", Date{-377, Date::feb, 4}));
   v.push_back(test("Marcus Aurelius", "Meditations", Book::Genre::Philosophy, "1-2-3-a", Date{172, Date::jan, 4}));
   return v;
}

static void add_and_test(Library& lib) {
   lib.add_books(test_book_construction());
   test_book_borrowing();
   lib.add_users(test_patron());
   lib.add_user(test_patron_charges());
}

static void add_2_times(Library& lib, const string& name) {
   lib.add_user(name);
   try {
      lib.add_user(name);
   }
   catch (Library::Invalid_Transaction& e) {
      cerr << "Exception catched: \n" << e.what() << endl;
   }
}

static void incorrect_add(Library& lib) {
   add_2_times(lib, "marcus_aurelius");
}

static void library_test() {
   Library library;
   add_and_test(library);
   cout << library;
   incorrect_add(library);
}

int main() {
   try {
      date_test();
      library_test();
      return 0;
   }
   catch (Library::Invalid_Transaction& e) {
      cerr << "main(): exception\n" << e.what() << endl;
   }
   catch (Book::Invalid_Operation& e) {
      cerr << "main(): exception\n" << e.what() << endl;
   }
   catch (Book::Invalid_Book& e) {
      cerr << "main(): exception\n" << e.what() << endl;
   }
   catch (Patron::Invalid_Patron& e) {
      cerr << "main(): exception\n" << e.what() << endl;
   }
   catch (Date::Invalid& e) {
      cerr << "main(): exception\n" << e.what() << endl;
   }
   catch (Invalid& e) {
      cerr << "main(): exception\n" << e.what() << endl;
   }
   catch (runtime_error& e) {
      cerr << "main(): exception\n" << e.what() << endl;
   }
   catch (exception& e) {
      cerr << "main(): exception\n" << e.what() << endl;
   }
   catch (...) {
      cerr << "main(): unrecognized exception\n";
   }
   return 1;
}
