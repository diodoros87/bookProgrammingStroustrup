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

long long Library::get_user_index(unsigned int number, unsigned int begin /* = 0*/) {
   const unsigned int SIZE = users.size();
   if (begin >= SIZE)
      throw Invalid("exist_card_number: begin >= SIZE");
   for (unsigned int i = begin; i < SIZE; i++) 
      if (users[i].get_number() == number)
         return i;
   return INDEX_NOT_FOUND;
}
/*
long long get_user_index(unsigned int number) {
   const unsigned int SIZE = users.size();
   for (unsigned int i = 0; i < SIZE; i++) 
      if (users[i].get_number() == number)
         return i;
   return -1;
}
*/
unsigned int Library::generate_user_card_number() {
   if (is_max_users_number())
      throw Library::Invalid_Transaction("Can not generate card number for library has " + to_string(Library::MAX_USERS) 
         + " users. " + "This library has already maximum number of users " + to_string(Library::MAX_USERS));
   static unsigned int number = 0;
   long long index = get_user_index(number);
   for (unsigned int counter = 1; INDEX_NOT_FOUND != index && counter <= UINT_MAX; counter++)
      index = get_user_index(++number, ++index);
   
   if (index != INDEX_NOT_FOUND)
      return number++;
   Unforeseen_Behavior();
   /*
   if (is_max_users_number())
      throw Library::Invalid_Transaction("Can not generate card number for library has >= " + to_string(Library::MAX_USERS) 
         + " users. " + "This library has already maximum number of users " + to_string(Library::MAX_USERS));
   srand (time(NULL));
   return rand();*/
}

string Library::generate_isbn() {
   static unsigned int number = 0;
   if (UINT_MAX == number)
      throw Library::Invalid_Transaction("Can not generate card number for library has " + to_string(Library::MAX_USERS) 
         + " users. " + "This library has already maximum number of users " + to_string(Library::MAX_USERS));
   return number++;
   /*
   if (is_max_users_number())
      throw Library::Invalid_Transaction("Can not generate card number for library has >= " + to_string(Library::MAX_USERS) 
         + " users. " + "This library has already maximum number of users " + to_string(Library::MAX_USERS));
   srand (time(NULL));
   return rand();*/
}

void Library::add_user(Patron parameter) {
    if (is_max_users_number())
      throw Library::Invalid_Transaction("Can not add user to library with maximum number of users. "
          + "This library has already " + to_string(Library::MAX_USERS) + " users.");
   for (Patron u : users) {
      if (parameter.get_number() == u.get_number())
         throw Library::Invalid_Transaction("User " + u.get_user_name() + " has identical card number " + u.get_number());
      if (parameter.get_user_name() == u.get_user_name())
         throw Library::Invalid_Transaction("User '" + u.get_user_name() + "' was added earlier ");
   }
   users.push_back(parameter);
}

void Library::add_book(Patron parameter) {
    if (library.is_max_users_number())
      throw Library::Invalid_Transaction("Can not add user to library with maximum number of users. "
          + "This library has already " + to_string(MAX_USERS) + " users.");
   for (Patron u : users) {
      if (parameter.get_number() == u.get_number())
         throw Invalid_Transaction("User " + u.get_user_name() + " has identical card number " + u.get_number());
      if (parameter.get_user_name() == u.get_user_name())
         throw Invalid_Transaction("User '" + u.get_user_name() + "' was added earlier ");
   }
   users.push_back(parameter);
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
