//#ifndef BOOK_H
//#define BOOK_H

#include "error.h"
using std::string;
using std::ostream;
using Chrono::Date;

class Book {
public:
   static constexpr char SEPARATOR_CHAR = '-';
   static constexpr unsigned int INTEGERS = 3;
   static constexpr unsigned int SEPARATORS = 3;
   static constexpr unsigned int ITERATIONS = 3;
   
   class Invalid_Book : public Invalid { 
      string msg {string(Invalid::what()) + " Invalid book: "};
   public:
      Invalid_Book(const string& message) { msg += message; }
      const char* what() {
         return msg.c_str();
      }
   };
   class Invalid_Operation : public Invalid { 
      string msg {string(Invalid::what()) + " Invalid book operation: "};
   public:
      Invalid_Operation(const string& message) { msg += message; }
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
   Book(const string& a, const string& t, Genre g, const string& isbn, const Date& d, bool b);
private:
   Book();
   void validation() const;
   string author;
   string title;
   string ISBN;
   Genre genre;
   Date date;
   bool borrowed {false};
};

inline ostream& operator<<(ostream& os, const Book::Genre& genre) {
   return os << Book::GENRE_NAMES[static_cast<int>(genre)];
}

inline ostream& operator<<(ostream& os, const Book& book) {
   return os << " ISBN: " << book.isbn() << " \t" 
           << book.get_author() << " \"" << book.get_title() << "\" "
           << book.get_genre() << '\n';
}

inline bool operator==(const Book& a, const Book& b) {
   return a.isbn() == b.isbn() && a.get_title() == b.get_title() && a.get_author() == b.get_author()
      && a.get_date() == b.get_date() && a.get_genre() == b.get_genre() && a.is_borrow() == b.is_borrow();
}

inline bool operator!=(const Book& a, const Book& b) {
   return !(a==b);
}

string book_status(const Book& book);

//#endif
