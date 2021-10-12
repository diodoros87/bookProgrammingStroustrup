#include <cassert>
#include <string>
#include <vector>

#include "money.h"
#include "integer.h"

using namespace std;
using namespace integer_space;

#define NDEBUG

#ifndef NDEBUG
#   define M_Assert(Expr, Msg) \
    __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#   define M_Assert(Expr, Msg) ;
#endif
    
#ifndef NDEBUG
#   define my_assertion(Money1, Money2) \
    __my_assertion(Money1, Money2, __FILE__, __LINE__)
#else
#   define my_assertion(Money1, Money2) ;
#endif
    
#ifndef NDEBUG
#   define my_assertion_double(double1, double2) \
    __my_assertion_double(double1, double2, __FILE__, __LINE__)
#else
#   define my_assertion_double(double1, double2) ;
#endif

void __M_Assert(const char* expr_str, const bool expr,
                const char* file, const int line, const string& msg) {
    if (! expr) {
      cerr << "\nAssert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n";
      exit(EXIT_FAILURE);
   }
}

void __my_assertion(const Money& x, const Money& expected_x, const char* file, const int line) {
   if (x != expected_x) {
      const string message = " " + to_string(x) +
                        " != expected " + " " + to_string(expected_x);
      __M_Assert("x == expected_x", x == expected_x, file, line, message);
   }
}

void __my_assertion_double(double x, double expected_x, const char* file, const int line) {
   if (x != expected_x) {
      const string message = " " + to_string(x) +
                        " != expected " + " " + to_string(expected_x);
      __M_Assert("x == expected_x", x == expected_x, file, line, message);
   }
}

void incorrect_construction(const long NUMERATORS[], const unsigned int SIZE) {
   for (unsigned int i = 0; i < SIZE; i++) {
      try {
         Money x(NUMERATORS[i], 0);
         assert(false);
      }
      catch (const char* e) {
         cerr << "exception for numerator = " << NUMERATORS[i] << endl;
         cerr << "exception const char* e: " << e << endl;
      }
   }
}

inline void print(const char* msg, const Money& x) {
   cout << msg << x << endl;
}

inline void print(const char* msg, const vector<Money>& vec) {
   cout << msg << endl;
   for (unsigned int i = 0; i < vec.size(); i++) {
      cout << i << " . " << vec[i] << endl;
   }
   if (vec.empty())
      cout << "Empty vector\n";
}

void delete_elements(vector<Money>& vec, unsigned int deleting = 1) {
   deleting = deleting > vec.size() ? vec.size() : deleting;
   for (unsigned int i = 0; i < deleting; i++)
      vec.pop_back();
}

inline void print_add(const Money& x, const Money& y) {
   cout << x << " + " << y << " = " << x+y << " as " << double(x+y) << endl;
   cout << y << " + " << x << " = " << y+x << " as " << double(y+x) << endl;
}

inline void print_subtract(const Money& x, const Money& y) {
   cout << x << " - " << y << " = " << x-y << " as " << double(x-y) << endl;
   cout << y << " - " << x << " = " << y-x << " as " << double(y-x) << endl;
}

inline void print_multiply(const Money& x, const Money& y) {
   cout << x << " * " << y << " = " << x*y << " as " << double(x*y) << endl;
   cout << y << " * " << x << " = " << y*x << " as " << double(y*x) << endl;
}

void print_divide(const Money& x, const Money& y) {
   try {
      cout << x << " / " << y << " = " << x/y << " as " << double(x/y) << endl;
   }
   catch (const char* e) {
      cerr << "exception const char* e: " << e << endl;
   }
   try {
      cout << y << " / " << x << " = " << y/x << " as " << double(y/x) << endl;
   }
   catch (const char* e) {
      cerr << "exception const char* e: " << e << endl;
   }
}

void set_tests() {
   Money r = {2, 3};
   try {
      r.set_denominator(0);
   }
   catch (const char* e) {
      cerr << "exception const char* e: set_denominator : " << e << endl;
   }
   r = {5, 3};
   cout << " assignment r = " << r << '\n';
   r = r;
   cout << " self assignment r = " << r << '\n';
   Money f = {2, 3};
   f = r;
   cout << " assignment f = " << f << '\n';
   r.set_numerator(0);
   cout << " minus r = " << -r << '\n';
   try {
      Money::reverse(r);
   }
   catch (const char* e) {
      cerr << "exception const char* e: reverse : " << e << endl;
   }
}

inline void print_assignment(Money x, const Money& y) {
   cout << "ASSIGNMENT:\n";
   cout << " x = " << x << " y = " << y << endl;
   x = y;
   cout << x << " = " << y << " is " << x << " as " << double(x) << endl;
   assert(x==y && "x!=y");
   cout << "END OF ASSIGNMENT:\n";
}

inline void print_equality(const Money& x, const Money& y) {
   cout << x << " == " << y << " = " << (x==y) << endl;
   cout << x << " != " << y << " = " << (x!=y) << endl;
}

inline void print_negate(const Money& x) {
   cout << x << " and minus of this = " << -x << " as " << double(-x) << endl;
}

inline void print_plus(const Money& x) {
   cout << x << " and plus of this = " << +x << " as " << double(+x) << endl;
}

inline void print_reverse(const Money& x) {
   try {
      cout << x << " and reverse of this = " << Money::reverse(x) << " as " << 
         double(Money::reverse(x)) << endl;
   }
   catch (const char* e) {
      cerr << "exception const char* e: " << e << endl;
   }
}

inline void print_relations(const Money& x, const Money& y) {
   cout << x << " == " << y << " = " << (x==y) << endl;
   cout << x << " != " << y << " = " << (x!=y) << endl;
   cout << x << " >= " << y << " = " << (x>=y) << endl;
   cout << x << " < " << y << " = " << (x<y) << endl;
   cout << x << " <= " << y << " = " << (x<=y) << endl;
   cout << x << " > " << y << " = " << (x>y) << endl;
}

inline void print_inequality(const Money& x, const Money& y) {
   cout << x << " == " << y << " = " << (x==y) << endl;
   cout << x << " != " << y << " = " << (x!=y) << endl;
}

void print_arithmetic_tests(Money x, Money y) {
   print_add(x, y);
   print_subtract(x, y);
   print_multiply(x, y);
   print_divide(x, y);
   print_relations(x, y);
   print_assignment(x, y);
   print_negate(x);
   print_negate(y);
   print_plus(x);
   print_plus(y);
   print_reverse(x);
   print_reverse(y);
} 

void arithmetic_test() {
   Money z = Money(-49, -21);
   Money x = Money(66, -165);
   my_assertion_double(z.get_numerator(), 7);
   my_assertion_double(z.get_denominator(), 3);
   my_assertion_double(x.get_numerator(), -2);
   my_assertion_double(x.get_denominator(), 5);
   my_assertion(z, Money(7, 3));
   my_assertion(x, Money(-2, 5));
   my_assertion(z+x, Money(29, 15));
   my_assertion(x+z, Money(29, 15));
   my_assertion(z-x, Money(41, 15));
   my_assertion(x-z, Money(-41, 15));
   my_assertion(z*x, Money(-14, 15));
   my_assertion(x*z, Money(-14, 15));
   my_assertion(z/x, Money(-35, 6));
   my_assertion(x/z, Money(-6, 35));
   my_assertion(+x, Money(-2, 5));
   my_assertion(+z, Money(7, 3));
   my_assertion(-x, Money(2, 5));
   my_assertion(-z, Money(-7, 3));
   my_assertion(Money::reverse(x), Money(-5, 2));
   my_assertion(Money::reverse(z), Money(3, 7));
   assert(z!=x && "z==x");
   assert(z>x && "z<=x");
   assert(z>=x && "z<x");
   assert(x<z && "x>=z");
   assert(x<=z && "x>z");
   my_assertion(x=z, Money(7, 3));  // x = z
   my_assertion(z=x, Money(7, 3));
   my_assertion(x=z, Money(7, 3));
   
#ifndef NDEBUG
   assert(z==x && "z!=x");
   assert(z>=x && "z<x");
   assert(z<=x && "z>x");
   assert(x>=z && "x<z");
   assert(x<=z && "x>z");
#endif
   
   z.set_numerator(-9);
   my_assertion_double(z.get_numerator(), -3);
   my_assertion(z, Money(-3, 1));
   x = Money(8, 10);
   my_assertion(x, Money(-4, -5));
   x.set_denominator(-12);
   my_assertion_double(x.get_denominator(), 3);
   my_assertion(x, Money(15, -45));
   z = Money(0, 7);
   x = { 0, -5 };
   assert(z==x && "z!=x");
   assert(!(z!=x) && "z!=x");
}

void arithmetic_test(const vector<Money>& vec) {
   cout << "arithmetic_test for vector " << endl;
   for (unsigned int i = 0; i < vec.size(); i++) 
      for (unsigned int a = 0; a < vec.size(); a++)
         print_arithmetic_tests(vec[i], vec[a]);
   if (vec.empty())
      cout << "Empty vector\n";
}

Money get_print(const long numerator, const long denominator) {
   Money x(numerator, denominator);
   string msg = "(" + to_string(numerator) + ", " + to_string(denominator) + ") = ";
   print(msg.c_str(), x);
   return x;
}

vector<Money> construction_test() {
   Money number;
   print("default number = ", number);
   Money a = get_print(76, -24);
   Money b = get_print(-22, -99);
   Money c = get_print(-150, 325);/*
   Money x = get_print(1, 6);
   Money y = get_print(12, 16);
   Money z = get_print(2, 6);
   Money zero = get_print(0, 6);
   Money m = get_print(-2, 6);
   Money n = get_print(-3, -6);*/
   
   vector<Money> vec(8);
   cout << "vector.capacity() = " << vec.capacity() << '\n';
   print("vector<Money> vec after construction: ", vec);
   delete_elements(vec);
   print("vector<Money> vec after delete_elements(vec): ", vec);
   cout << "vector.capacity() = " << vec.capacity() << '\n';
   delete_elements(vec, 34);
   print("vector<Money> vec after delete_elements(vec, 34): ", vec);
   cout << "vector.capacity() = " << vec.capacity() << '\n';
   vec.push_back(number);
   vec.push_back(a);
   vec.push_back(b);
   vec.push_back(c);/*
   vec.push_back(x);
   vec.push_back(y);
   vec.push_back(z);
   vec.push_back(zero);
   vec.push_back(m);
   vec.push_back(n);*/
   print("vector<Money> vec after added elements: ", vec);
   cout << "vector.capacity() = " << vec.capacity() << '\n';
   return vec;
}



void test() { 
   Integer i;
   cout << i;
   /*
   arithmetic_test();
   vector<Money> vec = construction_test();
   arithmetic_test(vec);
   */
}

int main() {
   try {
      test();
      incorrect_test();
      return 0;
   }
   catch (const Arithmetic_Error & e) {
      cerr << __func__ << " exception : " << e.what() << endl;
   }
   catch (const exception& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   catch (const exception& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   catch (...) {
      cerr << __func__ << " unrecognized exception\n";
   }
   return 1;
}
