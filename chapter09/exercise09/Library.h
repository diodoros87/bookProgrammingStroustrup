#include <climits>
#include <vector>

using std::string;
using std::ostream;
using std::vector;
using Chrono::Date;

class Library {
private:
   struct Transaction {
      Date date;
      Patron patron;
      Book book;
   };

   vector<Book> books;
   vector<Patron> users;
   vector<Transaction> transactions;
   
   long long get_user_index(unsigned int card_number, unsigned int begin = 0) const;
   long long get_user_index(const string& name, unsigned int begin = 0) const;
   long long get_book_index(const string& p_isbn, unsigned int begin = 0) const;
   
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
   
   Library();
   
   bool is_max_users_number() const { return users.size() == MAX_USERS; }
   vector<Book> get_books() const { return books; }
   vector<Patron> get_users() const { return users; }
   //unsigned int get_users_number() const { return users.size(); }
   vector<Transaction> get_transactions() const { return transactions; }
   
   void add_user(Patron patron);
   void add_book(Book book);
   void book_borrowing_request(Patron patron, Book book);
};
