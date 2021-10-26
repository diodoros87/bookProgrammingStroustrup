#include "integer.hpp"
#include "assertions.hpp"
#include "money.hpp"

using namespace std;
using namespace integer_space;
using namespace money;

template <typename T> 
void construct(const string & DOLLARS, const double CENTS ) { 
   Money<T> money(DOLLARS, CENTS);
   cout << "money = " << money << '\n';
}

template <typename T> 
void construct(const string & DOLLARS) { 
   Money<T> money(DOLLARS);
   cout << "money = " << money << '\n';
}

template <typename T> 
void construct_incorrect(const string & DOLLARS, const double CENTS ) { 
   try {
      Money<T> money(DOLLARS, 34);
      assert(0);
      cerr << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   try {
      Money<T> money(DOLLARS, CENTS);
      assert(0);
      cout << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }/*
   try {
      Money<T> money("6", CENTS);
      assert(0);
      cout << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }*/
}

template <typename T> 
void construct_incorrect(const string & DOLLARS) { 
   try {
      Money<T> money(DOLLARS);
      assert(0);
      cout << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }/*
   try {
      Money<T> money("6", CENTS);
      assert(0);
      cout << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }*/
}

template <typename T> 
void construct() {
   construct<T>("20", 55);
   construct<T>("577", 45.7);
   construct<T>("-8577", 45.79);
   construct<T>("10");
   construct<T>("-8577e+03");
   construct<T>("10.67");
   construct<T>("10.679");
   construct<T>("10.6435");
}

template <typename T> 
void construct_incorrect() {
   construct_incorrect<T>("20.8", 5);
   construct_incorrect<T>("20t", 7);
   construct_incorrect<T>("-0", 4);
   //construct_incorrect<T>("57.7");
}

template <typename T>
inline void test() {
   construct<T>();
   construct_incorrect<T>();
}

int main() {
   try {
      test<char>();
      test<int_fast8_t>();
      test<int>();
      test<float>();
      test<long double>();
      test<Integer>();
      
      //test<string>();
      return 0;
   } catch (const bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const Arithmetic_Error & e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << endl;
   } catch (const runtime_error& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const exception& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (...) {
      cerr << __func__ << " unrecognized exception\n";
   }
   return 1;
}
