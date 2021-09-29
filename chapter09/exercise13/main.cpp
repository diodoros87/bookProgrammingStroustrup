#include <cassert>
#include <string>

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

inline void check_assertion(double x, double expected_x, const string& LABEL) {
   const string message = " " + LABEL + " " + to_string(x) +
                     " != expected " + LABEL + " " + to_string(expected_x);
   M_Assert(x == expected_x, message);
}

void incorrect_construction() {
   try {
      Rational x(1, 0);
      assert(false);
   }
   catch (const char* e) {
      cerr << "exception const char* e: " << e << endl;
   }
}

inline void print(const Rational& x) {
   cout << x << endl;
}

void test() { 
   Rational number;
   print(number);
   Rational x(1, 6);
   print(x);
}

int main() {
   try {
      test();
      incorrect_construction();
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
