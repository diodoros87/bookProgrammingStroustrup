#include <string>
#include <iostream>
#include <cmath>

using namespace std;

#include <chrono>
using namespace chrono;

typedef long (* long_PF_2cr_long)(const long&, const long&);
typedef short (* short_PF_2cr_short)(const short&, const short&);
typedef int (* int_PF_2cr_int)(const int&, const int&);
typedef char (* char_PF_2cr_char)(const char&, const char&); 

template<class Type>
Type sum(const Type& X, const Type& Y) {
   cout << "\n " << __func__ << ' ';
   long long diff = X - Y;
   if (0 == diff)
      throw invalid_argument("difference can not be 0");
   const Type FIRST = diff < 0 ? X : Y;
   const Type END   = diff > 0 ? X : Y;
   cout << "FIRST = " << FIRST << ' ';
   cout << "END   = " << END << '\n';
   Type result = 0;
   for (Type value = FIRST; value < END; value++)
      result += value;
   return result;
}

template<class Type>
Type difference(const Type& X, const Type& Y) {
   cout << "\n " << __func__ << ' ';
   long long diff = X - Y;
   if (0 == diff)
      throw invalid_argument("difference can not be 0");
   const Type FIRST = diff > 0 ? X : Y;
   const Type END   = diff < 0 ? X : Y;
   cout << "FIRST = " << FIRST << ' ';
   cout << "END   = " << END << '\n';
   Type result = 0;
   for (Type value = FIRST; value > END; value--)
      result -= value;
   return result;
}

template<class Type, typename F_P>
void duration_test(const Type& X, const Type& Y, F_P fp) {
   if (nullptr == fp)
      throw invalid_argument("pointer to function can not be NULL");
   static constexpr int MIN_DIFF = 21;
   if (llabs(X - Y) < MIN_DIFF)
      throw invalid_argument("Min difference between numeric arguments is " + to_string(MIN_DIFF));
   const string TYPE_NAME       = typeid(Type).name();
   const string P_FUNCTION_NAME = typeid(F_P).name();
   cout << "\n\t$$$ START of Duration test for type = " << TYPE_NAME;
   cout << " and pointer to function = " << P_FUNCTION_NAME << '\n';
   if (P_FUNCTION_NAME.find(TYPE_NAME) == string::npos)
      throw invalid_argument("pointer to function is different than argument type ");
   
   const time_point<high_resolution_clock> t1 = high_resolution_clock::now();
   fp(X, Y);
   const time_point<high_resolution_clock> t2 = high_resolution_clock::now();
   const nanoseconds ns = duration<long, nano>(t2 - t1);  
   cout << "\n\t*** RESULT of Duration test for type = " << TYPE_NAME << " was counted nanoseconds   =   " << ns.count() << '\n';
}

struct Function_Pointers {
private:
   long_PF_2cr_long  pf_long    {0};
   short_PF_2cr_short pf_short   {0};
   int_PF_2cr_int   pf_int     {0};
   char_PF_2cr_char  pf_char    {0};
public:
   Function_Pointers(long_PF_2cr_long l, short_PF_2cr_short s, int_PF_2cr_int i, char_PF_2cr_char c) {
      if (!l || !s || !i || !c)
         throw invalid_argument("pointer to function can not be NULL");
      pf_long = l;
      pf_short = s;
      pf_int = i;
      pf_char = c;
   }
   long_PF_2cr_long  plong() const { return pf_long; }
   short_PF_2cr_short pshort() const { return pf_short; }
   int_PF_2cr_int pint() const { return pf_int; }
   char_PF_2cr_char pchar() const { return pf_char; }
};

void duration_test_1(const long X, const long Y, const Function_Pointers * const P_FUNCTIONS) {
   if (! P_FUNCTIONS)
      throw invalid_argument("pointer to struct can not be NULL");
   duration_test(X, Y, P_FUNCTIONS->plong());
   duration_test(short(X), short(Y), P_FUNCTIONS->pshort());
   duration_test(int(X), int(Y), P_FUNCTIONS->pint());
   duration_test(char(X), char(Y), P_FUNCTIONS->pchar());
}

void duration_test_1(const long X, const long Y) {
   const Function_Pointers SUM = Function_Pointers(sum, sum, sum, sum);
   duration_test_1(X, Y, &SUM);
   const Function_Pointers DIFF = Function_Pointers(difference, difference, difference, difference);
   duration_test_1(X, Y, &DIFF);
}

int main() {
   for (long FIRST = -11, END = -FIRST; END < 20; FIRST -= 10, END = -FIRST) {
      duration_test_1(FIRST, END);
      break;
   }
}

// g++ -Wall -Wextra -O2 -std=c++14 -fcompare-debug-second -o integer_test integer_test.cpp
