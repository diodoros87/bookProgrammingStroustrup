#ifndef BOOK_H
#define BOOK_H

#include "error.h"
using std::string;
using std::ostream;
using Chrono::Date;

class Book {
public:
   class Invalid_Book : public Invalid { 
      string msg {"Invalid book: "};
   public:
      Invalid_Book(const string& message) { msg += message; }
      const char* what() {
         return msg.c_str();
      }
   };
   enum class Genre { 
      Philosophy, Astronomy, Mathematics, Computer_Science 
   };
   static const char* GENRE_NAMES[];
   
   string get_author() const { return author; }
   string get_title() const { return title; }
   string isbn() const { return ISBN; }
   Date get_date() const { return date; }
   Genre get_genre() const { return genre; }
   bool is_borrow() const { return borrowed; }
   
   void return_book();
   void borrow();
   
   Book(const string& a, const string& t, Genre g, const string& isbn, const Date& d);
private:
   Book();
   const string author;
   const string title;
   const string ISBN;
   const Genre genre;
   const Date date;
   bool borrowed {false};
};

inline ostream& operator<<(ostream& os, const Book::Genre& genre) {
   return os << Book::GENRE_NAMES[static_cast<int>(genre)];
}

inline ostream& operator<<(ostream& os, const Book& book) {
   return os << " ISBN: " << book.isbn() << " \t" 
           << book.get_author() << " \t" << book.get_title() << '\t'
           << book.get_genre() << '\n';
}

inline bool operator==(const Book& a, const Book& b) {
   return a.isbn() == b.isbn();
}

inline bool operator!=(const Book& a, const Book& b) {
   return !(a==b);
}

#endif
