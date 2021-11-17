#ifndef PRINT_HPP
#define PRINT_HPP

#include <iostream>
#include <tuple>

using std::enable_if;
using std::ostream;
using std::get;
using std::tuple;

#define TIE(...) (std::tie(__VA_ARGS__))

#define assert_many(EX,...) \
  ((EX) || (assert_tuples (std::tie(__VA_ARGS__), #EX, __FILE__, __LINE__, __DATE__, __TIME__)))

template<class T> 
inline T& unmove(T&& t) { return t; }

template<const unsigned int N>
struct print_tuple {
   template<typename... T>
   static typename enable_if<(N < sizeof...(T))>::type
   print(ostream& os, const tuple<T...>& t) {
      os << " " << get<N>(t);
      print_tuple<N + 1>::print(os, t);
   }
   
   template<typename... T>
   static typename enable_if<(N >= sizeof...(T))>::type
   print(ostream& , const tuple<T...>&) { }    // for empty tuple
};

inline ostream& operator<< (ostream & os, const tuple<> &) {
   return os << "";
}

template<typename S, typename... T>
inline ostream& operator<< (ostream & os, const tuple<S, T...> & t) {
   os << get<0>(t);
   print_tuple<1>::print(os, t);
   return os;
}

template <typename ...Args>
bool assert_tuples(tuple<Args...> info, const char *msg, const char *file, int line,
                   const char * date, const char * time) {
   std::cerr << "\nAssertion failed:\n"
            << "Expression:\t" << msg << "\n"
            << "Source file:\t\t" << file << ", line " << line << "\n"
            << "Date:\t\t"   << date  << "\n"
            << "Time:\t\t"   << time  << "\n"
            << "  Info: " << info << '\n' ;
   abort();
   //assert(false);
   //return false;
}

#endif
