#include <climits>
#include <vector>

using std::string;
using std::ostream;
using std::vector;
using Chrono::Date;

class Library {
private:
   struct Transaction {
      Patron patron;
      Book book;
      Date date;
      Transaction(const Patron& p, const Book& b, const Date& d = Chrono::get_today())
         : patron(p), book(b), date(d) {}
         
      bool operator==(const Transaction& other) const {
         return patron == other.patron && book == other.book && date == other.date;
      }
      bool operator!=(const Transaction& other) const {
         return !(*this==other);
      }
      string get_string() const ;
   };

   vector<Book> books;
   vector<Patron> users;
   vector<Transaction> transactions;
   
   bool exist(const Transaction& t);
   
   long long get_user_index(unsigned int card_number, unsigned int begin = 0) const;
   long long get_user_index(const string& name, unsigned int begin = 0) const;
   long long get_book_index(const string& p_isbn, unsigned int begin = 0) const;
   
   long long book_validation_index(const Book& book) const;
   long long user_validation_index(const Patron& patron) const;
   
public:
   static constexpr unsigned long long MAX_USERS = UINT_MAX + 1;
   static string generate_isbn(const Library& library);
   
   class Invalid_Transaction : public Invalid { 
   private:
      string msg {string(Invalid::what()) + " Invalid transaction: "};
   public:
      Invalid_Transaction(const string& message) { msg += message; }
      const char* what() {
         return msg.c_str();
      }
   };
   
   unsigned int generate_user_card_number() const;
   
   //Library() {}
   
   bool is_max_users_number() const { return users.size() == MAX_USERS; }
   vector<Book> get_books() const { return books; }
   vector<Patron> get_users() const { return users; }
   //unsigned int get_users_number() const { return users.size(); }
   vector<Transaction> get_transactions() const { return transactions; }
   
   static string get_string(const vector<Transaction>& transactions);
   
   void add_user(const string& name);
   void add_user(const Patron& patron);
   void add_users(const vector<Patron>& vec);
   
   void add_book(const Book& book);
   void add_book(const string& a, const string& t, Book::Genre g, const Date& d = Chrono::get_today());
   void add_books(const vector<Book>& vec);
   
   void book_borrowing_request(const Patron& patron, const Book& book, const Date& d = Chrono::get_today());
};

string get_string(const vector<Book>& books);
string get_string(const vector<Patron>& users);

inline ostream& print_users(ostream& os, const Library& lib) {
   return os << "\n Users: \n" << get_string(lib.get_users()) << '\n';
}

inline ostream& print_books(ostream& os, const Library& lib) {
   return os << "\n Books: \n" << get_string(lib.get_books()) << '\n';
}

inline ostream& print_transactions(ostream& os, const Library& lib) {
   return os << "\n Transactions: \n" << Library::get_string(lib.get_transactions()) << '\n';
}
/*
inline ostream& operator<<(ostream& os, const Library& lib) {
   return os << "\nLibrary content: " << print_users(os, lib) <<
      print_books(os, lib) << print_transactions(os, lib); 
}
*/
inline ostream& operator<<(ostream& os, const Library& lib) {
   return os << "\nLibrary content: \n Users: \n" << get_string(lib.get_users()) << 
      "\n Books: \n" << get_string(lib.get_books()) << 
      "\n Transactions: \n" << Library::get_string(lib.get_transactions()) << '\n';
}
