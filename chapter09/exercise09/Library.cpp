#include <string>
#include <cctype>
#include <iostream>
#include <ctime>    
#include <cstdlib>  

#include "error.h"
#include "date.h"
#include "Book.h"
#include "Patron.h"
#include "Library.h"

using namespace std;

static constexpr int INDEX_NOT_FOUND = -99;

int Library::get_user_index(unsigned int card_number, unsigned int begin /* = 0*/) const {
   const int SIZE = users.size();
   if (begin >= SIZE)
      throw Invalid("get_user_index: begin " + to_string(begin) + " >= SIZE " + to_string(SIZE));
   for (int i = begin; i < SIZE; i++) 
      if (users[i].get_number() == card_number)
         return i;
   return INDEX_NOT_FOUND;
}

int Library::get_user_index(const string& name, unsigned int begin /* = 0*/) const {
   const int SIZE = users.size();
   if (begin >= SIZE)
      throw Invalid("get_user_index: begin " + to_string(begin) + " >= SIZE " + to_string(SIZE));
   for (int i = begin; i < SIZE; i++) 
      if (users[i].get_user_name() == name)
         return i;
   return INDEX_NOT_FOUND;
}

int Library::get_book_index(const string& p_isbn, unsigned int begin /* = 0*/) const {
   const int SIZE = books.size();
   if (begin >= SIZE)
      throw Invalid("get_user_index: begin " + to_string(begin) + " >= SIZE " + to_string(SIZE));
   for (int i = begin; i < SIZE; i++) 
      if (books[i].isbn() == p_isbn)
         return i;
   return INDEX_NOT_FOUND;
}

static void control_users_size(const Library& library) {
   if (library.is_max_books_number())
      throw Library::Invalid_Transaction("Can not add user to library with maximum number of users = "
+ to_string(Library::MAX_USERS) + "This library has already " + to_string(library.get_users_number()) + " users.");
}

unsigned int Library::generate_user_card_number() const {
   control_users_size(*this);
   static unsigned int number = 0;
   int index = get_user_index(number);
   for (unsigned int counter = 1; INDEX_NOT_FOUND != index && counter <= UINT_MAX; counter++)
      index = get_user_index(++number);
   
   if (index == INDEX_NOT_FOUND)
      return number++;
   else
      Unforeseen_Behavior("");
}

static long long int random_number(const long long BEGIN = 0, const long long INCREMENT = 1, const long long RANGE = RAND_MAX) {
   return BEGIN + INCREMENT * rand() % RANGE;
}

static char random_alphanum() {
   static constexpr short int RANGE = 1 + ('9' - '0') + 1 + ('Z' - 'A') + 1 + ('z' - 'a');
   static constexpr short int LETTERS_RANGE = 1 + ('Z' - 'A');
   int random = rand() % RANGE;
   if (10 > random)
      random += '0'; 
   else {
      random -= 10;
      if (random < LETTERS_RANGE)
         random += 'A';
      else
         random = random - LETTERS_RANGE + 'a';
   }
   const char result = random;
   if ((result >='0' && result <= '9') || (result >= 'A' && result <= 'Z') || (result >= 'a' && result <= 'z'))
      return result;
   else
      Unforeseen_Behavior("result = " + string(1, result));
}

static void control_books_size(const Library& library) {
   if (library.is_max_books_number())
      throw Library::Invalid_Transaction("Can not add book to library with maximum number of books =  "+ to_string(Library::MAX_BOOKS)
+ " This library has already " + to_string(library.get_books_number()) + " books.");
}

string Library::generate_isbn(const Library& library) {
   control_books_size(library);
   string result;
   unsigned int counter = 0;
   do {
      result = to_string(counter);
      for (unsigned int iteration = 0; iteration < Book::ITERATIONS; iteration++) {
         result += to_string(random_number());
         result += string(1, Book::SEPARATOR_CHAR);
      }
      result += string(1, random_alphanum());
      counter++;
   } while(INDEX_NOT_FOUND != library.get_book_index(result));
   
   return result;
}

void Library::add_user(const string& name) {
   control_users_size(*this);
      
   for (Patron u : users) 
      if (name == u.get_user_name())
         throw Library::Invalid_Transaction("Identical user name detected for " + Patron::status(u));
   Patron user = Patron(name, generate_user_card_number());
   users.push_back(user);
}

void Library::add_user(const Patron& parameter) {
   control_users_size(*this);
      
   for (Patron u : users) {
      if (parameter.get_number() == u.get_number())
         throw Library::Invalid_Transaction("Identical card number detected for " + Patron::status(u));
      if (parameter.get_user_name() == u.get_user_name())
         throw Library::Invalid_Transaction("Identical user name detected for " + Patron::status(u));
   }
   users.push_back(parameter);
}

void Library::add_users(const vector<Patron>& vec) {
   for (Patron p : vec) 
      add_user(p);
}

void Library::add_book(const string& a, const string& t, Book::Genre g, const Date& d) {
   control_books_size(*this);
   Book book = Book(a, t, g, generate_isbn(*this), d);
   books.push_back(book);
}

void Library::add_book(const Book& parameter) {
   control_books_size(*this);
   for (Book b : books) 
      if (parameter.isbn() == b.isbn())
         throw Invalid_Transaction("Identical ISBN detected for  " + book_status(b));
   books.push_back(parameter);
}

void Library::add_books(const vector<Book>& vec) {
   for (Book b : vec) 
      add_book(b);
}

int Library::book_validation_index(const Book& book) const {
   const int book_index = get_book_index(book.isbn());
   if (INDEX_NOT_FOUND == book_index)
      throw Library::Invalid_Transaction("Book " + book_status(book) + " is not in library");
   if (books[book_index] != book)
      throw Library::Invalid_Transaction("Book " + book_status(book) + " is different than \
book with the same isbn in library: " + book_status(books[book_index]));
      
   return book_index;
}

int Library::user_validation_index(const Patron& patron) const {
   const int user_index = get_user_index(patron.get_user_name());
   if (INDEX_NOT_FOUND == user_index)
      throw Library::Invalid_Transaction("User " + patron.get_user_name() + " is not registered to library");
   
   const int user_index_by_card_number = get_user_index(patron.get_number());
   if (user_index != user_index_by_card_number)
      Unforeseen_Behavior("user_index != user_index_by_card_number " + 
            to_string(user_index) + " != " + to_string(user_index_by_card_number));
   if (INDEX_NOT_FOUND == get_user_index(patron.get_number()))
      throw Library::Invalid_Transaction("User with card number " + to_string(patron.get_number())
                  + " is not registered to library");
   
   if (users[user_index_by_card_number] != patron)
      throw Library::Invalid_Transaction("User " + Patron::status(patron) + " is different than \
user with the same card number in library: " + Patron::status(users[user_index_by_card_number]));
   
   return user_index;
}

int Library::get_transaction_index(const Patron& patron, const Book& book) const {
   const int SIZE = transactions.size();
   for (int i = 0; i < SIZE; i++) 
      if (transactions[i].patron == patron && transactions[i].book == book)
         return i;
   return INDEX_NOT_FOUND;
}

bool Library::exist(const Transaction& parameter) const {
   for (Transaction t : transactions)
      if (parameter == t)
         return true;
   return false;
}

void Library::remove_transaction(const Patron& patron, const Book& book) {
   const int index = get_transaction_index(patron, book);
   if (INDEX_NOT_FOUND == index)
      throw Library::Invalid_Transaction("Can not remove transaction, because transaction for " + Patron::status(patron)
         + " and " + book_status(book) + " has not found.");
   if (index >= transactions.size())
      Unforeseen_Behavior("index(" + to_string(index) + ") >= transactions.size() " + to_string(transactions.size()));
   transactions.erase(transactions.begin() + index);  // erasing the oldest transactions to save memory
}

void Library::erase_transactions() {
   static constexpr int DELETING = TRANSACTIONS_HISTORY_LIMIT / 10;
   if (transactions.size() == TRANSACTIONS_HISTORY_LIMIT)
      transactions.erase(transactions.begin(), transactions.begin() + DELETING);  // erasing the oldest transactions to save memory
}

void Library::add_transaction(const Patron& patron, const Book& book, const Date& date/* = Chrono::get_today()*/) {
   erase_transactions();
   Transaction new_transact(patron, book, date);
   if (exist(new_transact))
      throw Library::Invalid_Transaction("Identical transaction was saved previously.");
   transactions.push_back(new_transact);
}

void Library::book_borrowing_request(const Patron& patron, const Book& book, const Date& date/* = Chrono::get_today()*/) {
   const int user_index = user_validation_index(patron);
   const int book_index = book_validation_index(book);
   if (date > Chrono::get_today())
      throw Library::Invalid_Transaction("Date " + to_string(date) + " is in the future. \
Library can not accepted transaction with date is in the future. ");
   if (users[user_index].are_charges())
      throw Library::Invalid_Transaction("User " + Patron::status(patron) + " must pay charges.");
   if (books[book_index].is_borrow())
      throw Library::Invalid_Transaction("Book " + book_status(book) + " has been borrowed already.");
   
   add_transaction(patron, book, date);
   books[book_index].borrow();
}

void Library::return_book(const Patron& patron, const Book& book, const Date& date/* = Chrono::get_today()*/) {
   user_validation_index(patron);
   const int book_index = book_validation_index(book);
   if (date > Chrono::get_today())
      throw Library::Invalid_Transaction("Date " + to_string(date) + " is in the future. \
Library can not accepted transaction with date is in the future. ");
   if (false == books[book_index].is_borrow())
      throw Library::Invalid_Transaction("Book " + book_status(book) + " has not been borrowed yet.");
   
   remove_transaction(patron, book);
   books[book_index].return_book();
}

vector<Patron> Library::get_users_with_charges() const {
   vector<Patron> result;
   for (Patron u : users)
      if (u.are_charges())
         result.push_back(u);
   return result;
}

void Library::set_charges(const Patron& patron, unsigned int charges) {
   const int user_index = user_validation_index(patron);
   users[user_index].set_charges(charges);
}

string get_string(const vector<Book>& books) {
   string result;
   for (int i = 0; i < books.size(); i++)
      result += std::to_string(i) + ". " + book_status(books[i]);
   return result;
}

string get_string(const vector<Patron>& users) {
   string result;
   for (int i = 0; i < users.size(); i++)
      result += std::to_string(i) + ". " + Patron::status(users[i]);
   return result;
}

string Library::Transaction::get_string() const {
   return " Transaction:\n Patron: " + Patron::status(patron) + 
      "\n Book: " + book_status(book) +
      "\n Date: " + to_string(date);
}

string Library::get_string(const vector<Transaction>& transactions) {
   string result;
   for (int i = 0; i < transactions.size(); i++)
      result += std::to_string(i) + ". " + transactions[i].get_string();
   return result;
}
