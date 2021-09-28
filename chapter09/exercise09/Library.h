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
   
   int get_transaction_index(const Patron& patron, const Book& book) const ;
   bool exist(const Transaction& t) const;
   void add_transaction(const Patron& patron, const Book& book, const Date& date/* = Chrono::get_today()*/);
   void erase_transactions();  // erasing the oldest transactions to save memory
   void remove_transaction(const Patron& patron, const Book& book);
   
   void check_last_borrower(const Patron& patron, const Book& book) const;
   
   int get_user_index(unsigned int card_number, unsigned int begin = 0) const;
   int get_user_index(const string& name, unsigned int begin = 0) const;
   int get_book_index(const string& p_isbn, unsigned int begin = 0) const;
   
   int book_validation_index(const Book& book) const;
   int user_validation_index(const Patron& patron) const;
   
public:
   static constexpr unsigned int MAX_USERS = 29;
   static constexpr unsigned int MAX_BOOKS = 199;
   static constexpr unsigned int TRANSACTIONS_HISTORY_LIMIT = 90;
   
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
   
   Library() { std::srand(std::time(0)); }
   
   unsigned int generate_user_card_number() const;
   
   bool is_max_users_number() const { return users.size() == MAX_USERS; }
   bool is_max_books_number() const { return books.size() == MAX_BOOKS; }
   
   vector<Book> get_books() const { return books; }
   vector<Patron> get_users() const { return users; }
   vector<Patron> get_users_with_charges() const; 
   unsigned int get_users_number() const { return users.size(); }
   unsigned int get_books_number() const { return books.size(); }
   vector<Transaction> get_transactions() const { return transactions; }
   
   static string get_string(const vector<Transaction>& transactions);
   
   void add_user(const string& name);
   void add_user(const Patron& patron);
   void add_users(const vector<Patron>& vec);
   
   void delete_user(const Patron& patron);
   void delete_book(const Book& parameter);
   
   bool exist(const Book& parameter) const;
   bool exist(const Patron& parameter) const;
   
   void add_book(const Book& book);
   void add_book(const string& a, const string& t, Book::Genre g, const Date& d);
   void add_books(const vector<Book>& vec);
   
   void book_borrowing_request(const Patron& patron, const Book& book, const Date& d = Chrono::get_today());
   void return_book(const Patron& patron, const Book& book, const Date& d = Chrono::get_today());

   void set_charges(const Patron& patron, unsigned int charges);
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
   return os << "\n TRANSACTIONS: \n" << Library::get_string(lib.get_transactions()) << '\n';
}

inline ostream& print_users_with_charges(ostream& os, const Library& lib) {
   return os << "\n Users with penalty charges: \n" << get_string(lib.get_users_with_charges());
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
      "\n TRANSACTIONS: \n" << Library::get_string(lib.get_transactions()) << '\n';
}
