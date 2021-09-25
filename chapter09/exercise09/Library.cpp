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

static constexpr long long INDEX_NOT_FOUND = -1;

long long Library::get_user_index(unsigned int card_number, unsigned int begin /* = 0*/) const {
   const unsigned int SIZE = users.size();
   if (begin >= SIZE)
      throw Invalid("get_user_index: begin >= SIZE");
   for (unsigned int i = begin; i < SIZE; i++) 
      if (users[i].get_number() == card_number)
         return i;
   return INDEX_NOT_FOUND;
}

long long Library::get_user_index(const string& name, unsigned int begin /* = 0*/) const {
   const unsigned int SIZE = users.size();
   if (begin >= SIZE)
      throw Invalid("get_user_index: begin >= SIZE");
   for (unsigned int i = begin; i < SIZE; i++) 
      if (users[i].get_user_name() == name)
         return i;
   return INDEX_NOT_FOUND;
}

long long Library::get_book_index(const string& p_isbn, unsigned int begin /* = 0*/) const {
   const unsigned int SIZE = books.size();
   if (begin >= SIZE)
      throw Invalid("get_book_index: begin >= SIZE");
   for (unsigned int i = begin; i < SIZE; i++) 
      if (books[i].isbn() == p_isbn)
         return i;
   return INDEX_NOT_FOUND;
}

unsigned int Library::generate_user_card_number() const {
   if (is_max_users_number())
      throw Library::Invalid_Transaction("Can not generate card number for library has " + to_string(Library::MAX_USERS) 
         + " users. " + "This library has already maximum number of users " + to_string(Library::MAX_USERS));
   static unsigned int number = 0;
   long long index = get_user_index(number);
   for (unsigned int counter = 1; INDEX_NOT_FOUND != index && counter <= UINT_MAX; counter++)
      index = get_user_index(++number, ++index);
   
   if (index != INDEX_NOT_FOUND)
      return number++;
   else
      Unforeseen_Behavior();
}

static long long int random_number(const long long BEGIN = 0, const long long INCREMENT = 1, const long long RANGE = RAND_MAX) {
   srand (time(NULL));
   return BEGIN + INCREMENT * rand() % RANGE;
}

static char random_alphanum() {
   static constexpr short int RANGE = 1 + ('9' - '0') + 1 + ('Z' - 'A') + 1 + ('z' - 'a');
   static constexpr short int LETTERS_RANGE = 1 + ('Z' - 'A');
   short int result = rand() % RANGE;
   if (10 > result)
      return result + '0'; 
   else {
      result -= 10;
      if (result < LETTERS_RANGE)
         return result + 'A';
      else
         return result + 'a';
   }
}

string Library::generate_isbn(const Library& library) {
   string result;
   unsigned long long int counter = 0;
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

void Library::add_user(Patron parameter) {
    if (is_max_users_number())
      throw Library::Invalid_Transaction("Can not add user to library with maximum number of users. \
This library has already " + to_string(Library::MAX_USERS) + " users.");
   for (Patron u : users) {
      if (parameter.get_number() == u.get_number())
         throw Library::Invalid_Transaction("Identical card number detected for " + Patron::status(u));
      if (parameter.get_user_name() == u.get_user_name())
         throw Library::Invalid_Transaction("Identical user name detected for " + Patron::status(u));
   }
   users.push_back(parameter);
}

void Library::add_book(Book parameter) {
   for (Book b : books) 
      if (parameter.isbn() == b.isbn())
         throw Invalid_Transaction("Identical ISBN detected for  " + book_status(b));
   books.push_back(parameter);
}

static long long book_validation_index(const Book& book) {
   const long long book_index = get_book_index(patron.get_user_name());
   if (INDEX_NOT_FOUND == get_book_index(book.isbn()))
      throw Library::Invalid_Transaction("User " + patron.get_user_name() + " is not registered to library");
   return user_index;
}

static long long user_validation_index(const Patron& patron) {
   const long long user_index = get_user_index(patron.get_user_name());
   if (INDEX_NOT_FOUND == user_index)
      throw Library::Invalid_Transaction("User " + patron.get_user_name() + " is not registered to library");
   const long long user_index_by_card_number = get_user_index(patron.get_number());
   if (user_index != user_index_by_card_number)
      Unforeseen_Behavior("user_index != user_index_by_card_number " + 
            to_string(user_index) + " != " + to_string(user_index_by_card_number));
   if (INDEX_NOT_FOUND == get_user_index(patron.get_number()))
      throw Library::Invalid_Transaction("User with card number " + to_string(patron.get_number())
                  + " is not registered to library");
   return user_index;
}

void Library::book_borrowing_request(const Patron& patron, const Book& book) {
   const long long user_index = user_validation_index(patron);
   const long long book_index = book_validation_index(book);
   Transaction transaction();
}
