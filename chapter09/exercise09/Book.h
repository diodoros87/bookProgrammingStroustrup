using std::string;
//using std::vector;
using std::ostream;
//using Chrono;
using Chrono::Date;

class Book {
public:
   enum class Genre { 
      Philosophy, Astronomy, Mathematics, Computer_Science 
   };
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

ostream& operator<<(ostream& os, const Book& book);
bool operator==(const Book& a, const Book& b);
bool operator!=(const Book& a, const Book& b);
