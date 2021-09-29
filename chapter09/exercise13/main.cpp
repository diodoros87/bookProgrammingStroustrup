#include <cassert>
#include <string>
#include <vector>

#include "rational.h"

using namespace std;
using namespace rational;

//#define NDEBUG

#ifndef NDEBUG
#   define M_Assert(Expr, Msg) \
    __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#   define M_Assert(Expr, Msg) ;
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

inline void check_assertion(double x, double expected_x, const string& LABEL = "") {
   const string message = " " + LABEL + " " + to_string(x) +
                     " != expected " + LABEL + " " + to_string(expected_x);
   M_Assert(x == expected_x, message);
}

void incorrect_construction(const long NUMERATORS[], const unsigned int SIZE) {
   for (unsigned int i = 0; i < SIZE; i++) {
      try {
         Rational x(NUMERATORS[i], 0);
         assert(false);
      }
      catch (const char* e) {
         cerr << "exception for numerator = " << NUMERATORS[i] << endl;
         cerr << "exception const char* e: " << e << endl;
      }
   }
}

inline void print(const char* msg, const Rational& x) {
   cout << msg << x << endl;
}

inline void print(const char* msg, const vector<Rational>& vec) {
   cout << msg << endl;
   for (unsigned int i = 0; i < vec.size(); i++) {
      cout << i << " . " << vec[i] << endl;
   }
   if (vec.empty())
      cout << "Empty vector\n";
}

void delete_elements(vector<Rational>& vec, unsigned int deleting = 1) {
   deleting = deleting > vec.size() ? vec.size() : deleting;
   for (unsigned int i = 0; i < deleting; i++)
      vec.pop_back();
}

inline void print_add(const Rational& x, const Rational& y) {
   cout << x << " + " << y << " = " << x+y << " as " << double(x+y) << endl;
   cout << y << " + " << x << " = " << y+x << " as " << double(y+x) << endl;
}

inline void print_subtract(const Rational& x, const Rational& y) {
   cout << x << " - " << y << " = " << x-y << " as " << double(x-y) << endl;
   cout << y << " - " << x << " = " << y-x << " as " << double(y-x) << endl;
}

inline void print_multiply(const Rational& x, const Rational& y) {
   cout << x << " * " << y << " = " << x*y << " as " << double(x*y) << endl;
   cout << y << " * " << x << " = " << y*x << " as " << double(y*x) << endl;
}

inline void print_divide(const Rational& x, const Rational& y) {
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

inline void print_assignment(Rational x, const Rational& y) {
   cout << "ASSIGNMENT:\n";
   cout << x << " = " << x << " y = " << y << endl;
   x = y;
   cout << x << " = " << y << " is " << x << " as " << double(x) << endl;
   assert(x==y && "x!=y");
   cout << "END OF ASSIGNMENT:\n";
}

inline void print_equality(const Rational& x, const Rational& y) {
   cout << x << " == " << y << " = " << (x==y) << endl;
   cout << x << " != " << y << " = " << (x!=y) << endl;
}

inline void print_negate(const Rational& x) {
   cout << x << " and negate of this = " << -x << " as " << double(-x) << endl;
}

inline void print_plus(const Rational& x) {
   cout << x << " and negate of this = " << +x << " as " << double(+x) << endl;
}

inline void print_reverse(const Rational& x) {
   try {
      cout << x << " and reverse of this = " << Rational::reverse(x) << " as " << 
         double(Rational::reverse(x)) << endl;
   }
   catch (const char* e) {
      cerr << "exception const char* e: " << e << endl;
   }
}

inline void print_relations(const Rational& x, const Rational& y) {
   cout << x << " == " << y << " = " << (x==y) << endl;
   cout << x << " != " << y << " = " << (x!=y) << endl;
   cout << x << " >= " << y << " = " << (x>=y) << endl;
   cout << x << " < " << y << " = " << (x<y) << endl;
   cout << x << " <= " << y << " = " << (x<=y) << endl;
   cout << x << " > " << y << " = " << (x>y) << endl;
}

inline void print_inequality(const Rational& x, const Rational& y) {
   cout << x << " == " << y << " = " << (x==y) << endl;
   cout << x << " != " << y << " = " << (x!=y) << endl;
}

void print_arithmetic_tests(Rational x, Rational y) {
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

inline void check_assertion(Rational x, Rational expected_x, const string& LABEL = "") {
   const string message = " " + LABEL + " " + to_string(x) +
                     " != expected " + LABEL + " " + to_string(expected_x);
   M_Assert(x == expected_x, message);
}

void arithmetic_test() {
   Rational z = Rational(-49, -21);
   Rational x = Rational(66, -165);
   check_assertion(z, Rational(7, 3));
   check_assertion(x, Rational(-2, 5));
   check_assertion(z+x, Rational(29, 15));
   check_assertion(x+z, Rational(29, 15));
   check_assertion(z-x, Rational(41, 15));
   check_assertion(x-z, Rational(-41, 15));
   check_assertion(z*x, Rational(-14, 15));
   check_assertion(x*z, Rational(-14, 15));
   check_assertion(z/x, Rational(-35, 6));
   check_assertion(x/z, Rational(-6, 35));
   check_assertion(+x, Rational(-2, 5));
   check_assertion(+z, Rational(7, 3));
   check_assertion(-x, Rational(2, 5));
   check_assertion(-z, Rational(-7, 3));
   check_assertion(Rational::reverse(x), Rational(-5, 2));
   check_assertion(Rational::reverse(z), Rational(3, 7));
   check_assertion(x=z, Rational(7, 3));
   check_assertion(z=x, Rational(7, 3));
}

void arithmetic_test(const vector<Rational>& vec) {
   cout << "arithmetic_test for vector " << endl;
   for (unsigned int i = 0; i < vec.size(); i++) 
      for (unsigned int a = 0; a < vec.size(); a++)
         print_arithmetic_tests(vec[i], vec[a]);
   if (vec.empty())
      cout << "Empty vector\n";
}

Rational get_print(const long numerator, const long denominator) {
   Rational x(numerator, denominator);
   string msg = "(" + to_string(numerator) + ", " + to_string(denominator) + ") = ";
   print(msg.c_str(), x);
   return x;
}

vector<Rational> construction_test() {
   Rational number;
   print("default number = ", number);
   Rational a = get_print(76, -24);
   Rational b = get_print(-22, -99);
   Rational c = get_print(-150, 325);
   Rational x = get_print(1, 6);
   Rational y = get_print(12, 16);
   Rational z = get_print(2, 6);
   Rational zero = get_print(0, 6);
   Rational m = get_print(-2, 6);
   Rational n = get_print(-3, -6);
   
   vector<Rational> vec(8);
   cout << "vector.capacity() = " << vec.capacity() << '\n';
   print("vector<Rational> vec after construction: ", vec);
   delete_elements(vec);
   print("vector<Rational> vec after delete_elements(vec): ", vec);
   cout << "vector.capacity() = " << vec.capacity() << '\n';
   delete_elements(vec, 34);
   print("vector<Rational> vec after delete_elements(vec, 34): ", vec);
   cout << "vector.capacity() = " << vec.capacity() << '\n';
   vec.push_back(number);
   vec.push_back(a);
   vec.push_back(b);
   vec.push_back(c);
   vec.push_back(x);
   vec.push_back(y);
   vec.push_back(z);
   vec.push_back(zero);
   vec.push_back(m);
   vec.push_back(n);
   print("vector<Rational> vec after added elements: ", vec);
   cout << "vector.capacity() = " << vec.capacity() << '\n';
   return vec;
}

void incorrect_test() { 
   constexpr long numerators[] = { -3, -4, 0, 2, 34, 1 };
   incorrect_construction(numerators, sizeof(numerators) / sizeof(*numerators));
}

void test() { 
   arithmetic_test();
   vector<Rational> vec = construction_test();
   arithmetic_test(vec);
}

int main() {
   try {
      test();
      incorrect_test();
      return 0;
   }
   catch (const char* e) {
      cerr << "main(): exception const char* e: " << e << endl;
      return 3;
   }
   catch (exception& e) {
      cerr << "main(): exception: " << e.what() << endl;
      return 1;
   }
   catch (...) {
      cerr << "main(): unrecognized exception\n";
      return 2;
   }
}
