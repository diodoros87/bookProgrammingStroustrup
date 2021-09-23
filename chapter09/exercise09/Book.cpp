#include <string>
#include <cctype>
#include <iostream>
#include <ctime>

#include "date.h"
#include "Book.h"
#include "error.h"


using namespace std;


void validate_name (const string& s) {
   unsigned int SIZE = s.size();
   if (2 > SIZE)
      error("Name length must be > 1");
   if (! isupper(s[0]))
      error("Name's first character must be upper case");   
   for (unsigned int i = 1; i < s.size(); i++)
      if (! islower(s[i]))
         error("After first character name must contain only lower case");
}

void validate_title(const string& title) {
   for (unsigned int i = 0; i < title.size(); i++)
      if (isprint(title[i])) 
         return;
   error("Title must contain at least 1 print character");
}

void validate_ISBN(const string& isbn) {
   static constexpr char SEPARATOR_CHAR = '-';
   static constexpr unsigned int INTEGERS = 3;
   static constexpr unsigned int SEPARATORS = 3;
   static constexpr unsigned int ITERATIONS = 3;
   const unsigned int SIZE = isbn.size();
   unsigned int int_counter, separator_counter = 0;
   unsigned int index = 0;
   for (unsigned int iteration = 0; index < SIZE && iteration < ITERATIONS; index++) {
      if (isdigit(isbn[index])) 
         int_counter++;
      while(index < SIZE && isdigit(isbn[index]))
         index++;
      if (index < SIZE && isbn[index] == SEPARATOR_CHAR)
         if (++separator_counter == SEPARATORS)
            break;
   }
   if (index == SIZE - 1 && isalnum(isbn[index])) 
      if (INTEGERS == int_counter && SEPARATORS == separator_counter)
         return;
   error("Correct format ISBN is n-n-n-x where n is integer x is digit or letter");
}

void validate_date (const Date& d) {
   time_t now;
   struct tm timeinfo;

   time(&now);  /* get current time; same as: now = time(NULL)  */
   timeinfo = *localtime(&now);
   static constexpr int year_offset = 1900;
   int current_year = timeinfo.tm_year + year_offset;
   if (current_year < d.year()) 
      return;
   else if (current_year > d.year()) 
      error(to_string(d.year()), " year of book is in the future");
   
   static const int month_offset = Date::Month::jan;
   timeinfo.tm_year = d.year() - year_offset;
   timeinfo.tm_mon = d.month() - month_offset;  // in struct tm months are: 0-11
   timeinfo.tm_mday = d.day();
   timeinfo.tm_hour = 23; timeinfo.tm_min = 59; timeinfo.tm_sec = 59;
   
   double diff_seconds = difftime(now, mktime(&timeinfo));
   cerr << diff_seconds << " seconds since book date in the current timezone.\n";
   static constexpr long int SECONDS_PER_DAY = 24 * 60 * 60;
   if (diff_seconds < SECONDS_PER_DAY)
      error(to_string(d.year()), " date of book is too early");
}

Book::Book(const string& a, const string& t, Genre g, const string& isbn, const Date& d) : 
author(a), title(t), ISBN(isbn), genre(g), date(d)
{
   validate_name(author);
   validate_title(title);
   validate_ISBN(ISBN);
   validate_date(date);/*
   author = a;
   title = t;
   ISBN = isbn;
   date = d;
   genre = g;*/
   //borrowed = false;
}

void Book::return_book() {
   if (! borrowed)
      error("Book has not been borrowed yet");
   borrowed = false;
}

void Book::borrow() {
   if (borrowed)
      error("Book has been borrowed already");
   borrowed = true;
}

ostream& operator<<(ostream& os, const Book& book) {
   return os << " ISBN: " << book.isbn() << " \t" 
           << book.get_author() << " \t" << book.get_title();
   //return os;
}

bool operator==(const Book& a, const Book& b) {
   return a.isbn() == b.isbn();
}

bool operator!=(const Book& a, const Book& b) {
   return !(a==b);
}
