#ifndef ASSERTIONS_HPP
#define ASSERTIONS_HPP

//#define NDEBUG
#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>

#include "integer.hpp"
//#include "integer_parsing.hpp"


using std::cerr;
using std::string;

using integer_space::Integer;

#ifndef NDEBUG
#  define general_assert(Expr, Msg) \
   GENERAL_ASSERT(#Expr, Expr, __FILE__, __LINE__, __DATE__,  Msg)
#else
#   define general_assert(Expr, Msg) ;
#endif
    
#ifndef NDEBUG
#  define integer_assert(Integer, Signum_Char, Number_String) \
   INTEGER_ASSERT(Integer, Signum_Char, Number_String, __FILE__, __LINE__, __DATE__, __TIME__)
#else
#   define integer_assert(Integer, Signum_Char, String) ;
#endif
   
#ifndef NDEBUG
#  define assert_Integer(Integer, Number_String) \
   ASSERT_INTEGER(Integer, Number_String, __FILE__, __LINE__, __DATE__, __TIME__)
#else
#   define assert_Integer(Integer, String) ;
#endif

void GENERAL_ASSERT(const char* expr_str, const bool expr, const char* file, const int line, 
                const char* date, const char* time, const string& msg) {
    if (! expr) {
      cerr << "\nAssert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n"
            << "Date:\t\t"   << date  << "\n"
            << "Time:\t\t"   << time  << "\n";
      exit(EXIT_FAILURE);
   }
}

void INTEGER_ASSERT(const Integer& x, const char expected_signum, const string& expected_number,
                  const char* file, const int line,
                  const char* date, const char* time) {
   if (static_cast<string>(x) != string(1, expected_signum) + expected_number) {
      const string EXPECTED = string(1, expected_signum) + expected_number;
      const string message = " " + static_cast<string>(x) +
                        " != expected " + EXPECTED;
      GENERAL_ASSERT(EXPECTED.c_str(), static_cast<string>(x) == EXPECTED, file, line, date, time, message);
   }
}

void ASSERT_INTEGER(const Integer& x, const string& expected_number,
                  const char* file, const int line,
                  const char* date, const char* time) {
   if (static_cast<string>(x) != string(1, x.get_signum()) + expected_number) {
      const string EXPECTED = string(1, x.get_signum()) + expected_number;
      const string message = " " + static_cast<string>(x) +
                        " != expected " + EXPECTED;
      GENERAL_ASSERT(EXPECTED.c_str(), static_cast<string>(x) == EXPECTED, file, line, date, time, message);
   }
}

/*
void __my_assertion_double(double x, double expected_x, const char* file, const int line) {
   if (x != expected_x) {
      const string message = " " + to_string(x) +
                        " != expected " + " " + to_string(expected_x);
      GENERAL_ASSERT("x == expected_x", x == expected_x, file, line, message);
   }
}
*/

#endif
