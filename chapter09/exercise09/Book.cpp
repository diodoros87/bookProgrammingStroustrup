#include <string>
#include <cctype>
#include <iostream>
#include <ctime>

#include "date.h"
#include "Book.h"
#include "error.h"

using namespace std;

const char* Book::GENRE_NAMES[] = {
   "Philosophy", "Astronomy", "Mathematics", "Computer Science"
};

void validate_name (const string& s) {
   const unsigned int SIZE = s.size();
   if (2 > SIZE)
      throw Book::Invalid_Book("Author name length must be > 1");
   if (! isupper(s[0]))
      throw Book::Invalid_Book("Author name's first character must be upper case");
   for (unsigned int i = 0; i < SIZE; i++)
      if (isalpha(s[i]) && (i + 1) < SIZE && isalpha(s[i + 1])) 
         return;
   throw Book::Invalid_Book("Author's name must have at least 2 alphabetic characters in succession");
}

void validate_title(const string& title) {
   for (unsigned int i = 0; i < title.size(); i++)
      if (isprint(title[i])) 
         return;
   throw Book::Invalid_Book("Title must contain at least 1 print character");
}

void validate_ISBN(const string& isbn) {
   static constexpr char SEPARATOR_CHAR = '-';
   static constexpr unsigned int INTEGERS = 3;
   static constexpr unsigned int SEPARATORS = 3;
   static constexpr unsigned int ITERATIONS = 3;
   const unsigned int SIZE = isbn.size();
   unsigned int int_counter, separator_counter = 0;
   unsigned int index = 0;
   for (unsigned int iteration = 0; index < (SIZE - 1) && iteration < ITERATIONS; iteration++) {
      if (isdigit(isbn[index])) 
         int_counter++;
      while(index < SIZE && isdigit(isbn[index]))
         index++;
      if (index < SIZE && isbn[index] == SEPARATOR_CHAR)
         ++separator_counter;
      index++;
   }
   if (index == SIZE - 1 && isalnum(isbn[index])) 
      if (INTEGERS == int_counter && SEPARATORS == separator_counter)
         return;
   throw Book::Invalid_Book("Correct format ISBN is n-n-n-x where n is integer x is digit or letter");
}

void check_time_difference(time_t& end, struct tm * timeinfo, const Date& d) {
   time_t begin = mktime(timeinfo);
   double diff_seconds = difftime(end, begin);
   cerr << diff_seconds << " seconds since book date in the current timezone.\n";
   
   static constexpr long int SECONDS_PER_DAY = 24 * 60 * 60;
   if (diff_seconds < SECONDS_PER_DAY) 
      throw Book::Invalid_Book(to_string(d) + " date of book is too early");
}

void validate_date (const Date& d) {
   time_t now;
   struct tm timeinfo;

   time(&now);  /* get current time; same as: now = time(NULL)  */
   timeinfo = *localtime(&now);
   static constexpr int year_offset = 1900;
   int current_year = timeinfo.tm_year + year_offset;
   if (current_year > d.year()) 
      return;
   else if (current_year < d.year()) 
      throw Book::Invalid_Book(to_string(d.year()) + " year of book is in the future");
   
   static constexpr int month_offset = 1;  // in struct tm months are: 0-11
   timeinfo.tm_year = d.year() - year_offset;
   timeinfo.tm_mon = d.month() - month_offset;  // in struct tm months are: 0-11
   timeinfo.tm_mday = d.day();
   timeinfo.tm_hour = 23; timeinfo.tm_min = 59; timeinfo.tm_sec = 59;
   
   check_time_difference(now, &timeinfo, d);
}

void Book::validation() const {
   validate_name(author);
   validate_title(title);
   validate_ISBN(ISBN);
   validate_date(date);
}

Book::Book(const string& a, const string& t, Genre g, const string& isbn, const Date& d) : 
author(a), title(t), ISBN(isbn), genre(g), date(d) 
{
   validation();
}

Book::Book(const string& a, const string& t, Genre g, const string& isbn, const Date& d, bool b) : 
author(a), title(t), ISBN(isbn), genre(g), date(d)
{
   validation();
   borrowed = b;
}

void Book::return_book() {
   if (! borrowed)
      throw Book::Invalid_Operation("Book has not been borrowed yet");
   borrowed = false;
}

void Book::borrow() {
   if (borrowed)
      throw Book::Invalid_Operation("Book has been borrowed already");
   borrowed = true;
}

string book_status(const Book& book) {
   return " ISBN: " + book.isbn() + " \t" + book.get_author() + " \"" +
      book.get_title() + '\"' + " " + Book::GENRE_NAMES[static_cast<int>(book.get_genre())] + 
      "\t status: " + (book.is_borrow() ? " borrowed" : " available") + "\n";
}
