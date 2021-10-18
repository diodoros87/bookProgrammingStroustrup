#ifndef ASSERTIONS_HPP
#define ASSERTIONS_HPP

//#define NDEBUG
#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include<sstream>

#include "integer.hpp"
//#include "integer_parsing.hpp"


using std::cerr;
using std::string;
using std::ostringstream;
using std::ios_base;

using integer_space::Integer;

#ifndef NDEBUG
#  define general_assert(Expr, Msg) \
   GENERAL_ASSERT(#Expr, Expr, __FILE__, __LINE__, __DATE__, __TIME__,  Msg)
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
   
#ifndef NDEBUG
#  define assert_number_Integer(LongLong, Integer) \
   ASSERT_NUMBER_INTEGER(LongLong, Integer, __FILE__, __LINE__, __DATE__, __TIME__)
#else
#   define assert_number_Integer(LongLong, Integer) ;
#endif

inline void GENERAL_ASSERT(const char* expr_str, const bool expr, const char* file, const int line, 
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
   const string signum_str = expected_signum == Integer::NEUTRAL ? "" : string(1, expected_signum);
   if (static_cast<string>(x) != signum_str + expected_number) {
      const string EXPECTED = signum_str + expected_number;
      const string message = " " + static_cast<string>(x) +
                        " != expected " + EXPECTED;
      GENERAL_ASSERT(EXPECTED.c_str(), static_cast<string>(x) == EXPECTED, file, line, date, time, message);
   }
}

void ASSERT_INTEGER(const Integer& x, const string& expected_number,
                  const char* file, const int line,
                  const char* date, const char* time) {
   const string signum_str = expected_number == "0" ? "" : string(1, x.get_signum());
   if (static_cast<string>(x) != signum_str + expected_number) {
      const string EXPECTED = signum_str + expected_number;
      const string message = " " + static_cast<string>(x) +
                        " != expected " + EXPECTED;
      GENERAL_ASSERT(EXPECTED.c_str(), static_cast<string>(x) == EXPECTED, file, line, date, time, message);
   }
}

void ASSERT_NUMBER_INTEGER(const long long & NUMBER, const Integer & OBJECT,
                     const char* file, const int line,
                     const char* date, const char* time) {
   ostringstream stream;
   if (0 != NUMBER)
      stream.setf(ios_base::showpos);
   stream << NUMBER;
   const string number_string = stream.str();
   const string object_string = string(OBJECT);
   if (number_string != object_string) {
      const string message = "\n number = " + number_string + "\n object = " + object_string;
      GENERAL_ASSERT(object_string.c_str(), number_string == object_string, file, line, date, time, message);
   }
} 

#endif
