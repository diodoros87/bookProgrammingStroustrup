#include <cassert>
#include <string>
#include <cstdint>
#include <climits>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

#include <chrono>
using namespace chrono;

template<class Type>
inline Type sum(const Type& X, const Type& Y) {
   //cout << "\n " << __func__ << '\n';
   long long diff = X - Y;
   if (0 == diff)
      throw invalid_argument("difference can not be 0");
   const Type FIRST = diff < 0 ? X : Y;
   const Type END   = diff > 0 ? X : Y;
   //cout << "FIRST = " << FIRST << '\n';
   //cout << "END   = " << END << '\n';
   Type result = 0;
   for (Type value = FIRST; value < END; value++)
      result += value;
   //cout << " Function sum for " << typeid(Type).name() << " = " << result;
   return result;
}

template<class Type>
inline void add(const Type& X, const Type& Y) {
   //printf ("__FUNCTION__ name = %s\n", __FUNCTION__);
   vector<Type> vec(llabs(X - Y));
   cout << "\n" << __func__ << "() size of vector type " << typeid(Type).name() << " = " << vec.size() << '\n';
   for (Type index = 0; index < vec.size(); index++)
      try {
         vec.at(index) = sum(X, Y);    
      }
      catch (const out_of_range& e) {
         cerr << "Out of Range error: " << e.what() << '\n';
      }

   for (Type index = 1; index < vec.size(); index++) 
      vec[index] += vec[index - 1];
}

template<class Type>
inline Type difference(const Type& X, const Type& Y) {
   //cout << "\n " << __func__ << '\n';
   long long diff = X - Y;
   if (0 == diff)
      throw invalid_argument("difference can not be 0");
   const Type FIRST = diff > 0 ? X : Y;
   const Type END   = diff < 0 ? X : Y;
   //cout << "FIRST = " << FIRST << '\n';
   //cout << "END   = " << END << '\n';
   Type result = 0;
   for (Type value = FIRST; value > END; value--)
      result -= value;
   //cout << " Function difference for " << typeid(Type).name() << " = " << result;
   return result;
}

template<class Type>
inline void sub(const Type& X, const Type& Y) {
   vector<Type> vec(llabs(X - Y));
   cout << "\n" << __func__ << "() size of vector type " << typeid(Type).name() << " = " << vec.size() << '\n';
   for (Type index = 0; index < vec.size(); index++)
      try {
         vec.at(index) = difference(X, Y);    
      }
      catch (const out_of_range& e) {
         cerr << "Out of Range error: " << e.what() << '\n';
      }

   for (Type index = 1; index < vec.size(); index++) 
      vec[index] -= vec[index - 1];
}

template<class Type, typename F_P>
inline void duration_test(const Type& FIRST, const Type& END, F_P fp) {
   if (nullptr == fp)
      throw invalid_argument("pointer to function can not be NULL");
   static constexpr int MIN_DIFF = 21;
   if (labs(FIRST - END) < MIN_DIFF)
      throw invalid_argument("Min difference between numeric arguments is " + to_string(MIN_DIFF));
   cout << "\n\nFIRST = " << FIRST << '\n';
   cout << "END   = " << END << '\n';
   const string TYPE_NAME       = typeid(Type).name();
   const string P_FUNCTION_NAME = typeid(F_P).name();
   cout << "Duration test for type = " << TYPE_NAME;
   cout << " and pointer to function = " << P_FUNCTION_NAME << '\n';
   if (P_FUNCTION_NAME.find(TYPE_NAME) == string::npos)
      throw invalid_argument("pointer to function is different than argument type ");

   const time_point<high_resolution_clock> t1 = high_resolution_clock::now();
   fp(FIRST, END);
   const time_point<high_resolution_clock> t2 = high_resolution_clock::now();
   const nanoseconds ns = duration<long, nano>(t2 - t1);  
   cout << "Duration test for type = " << TYPE_NAME << " was counted nanoseconds   =   " << ns.count() << '\n';
}

void duration_test(const long FIRST, const long END) {
   void (* pf_long)(const long&, const long&) = &add;
   duration_test(FIRST, END, pf_long);

   void (* pf_short)(const short&, const short&) = &add;
   duration_test(short(FIRST), short(END), pf_short);

   void (* pf_int)(const int&, const int&) = add;
   duration_test(int(FIRST), int(END), pf_int);

   void (* pf_char)(const char&, const char&) = add;
   duration_test(char(FIRST), char(END), pf_char);

   pf_long = sub;
   duration_test(END, FIRST, pf_long);

   pf_short = sub;
   duration_test(short(END), short(FIRST), pf_short);

   pf_int = sub;
   duration_test(int(END), int(FIRST), pf_int);

   pf_char = sub;
   duration_test(char(END), char(FIRST), pf_char);
}

int main() {
   for (long FIRST = -11, END = -FIRST; END < 30; FIRST -= 10, END = -FIRST)
      duration_test(FIRST, END);
}

// g++ -Wall -O2 -std=c++14 -fcompare-debug-second -o test integer_test_1.cpp
