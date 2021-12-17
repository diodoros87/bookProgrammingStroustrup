#ifndef PRINT_HPP
#define PRINT_HPP

#include <iostream>
#include <tuple>

using std::enable_if;
using std::ostream;
using std::get;
using std::tuple;
using std::cerr;

#include "utility.hpp"

#define TIE(...) (std::tie(__VA_ARGS__))

#define assert_many(EX,...) \
  ((EX) || (assert_tuples (std::tie(__VA_ARGS__), #EX, __FILE__, __LINE__, __DATE__, __TIME__)))

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
   cerr << "\nAssertion failed:\n"
            << "Expression:\t" << msg << "\n"
            << "Source file:\t\t" << file << ", line " << line << "\n"
            << "Date:\t\t"   << date  << "\n"
            << "Time:\t\t"   << time  << "\n"
            << "  Info: " << info << '\n' ;
   abort();
   //assert(false);
   //return false;
}

template<typename T>
inline void print_and_assert(const T& value, const T& expected_value, const string& value_string, const string& function) {
   cerr << TIE( "C++", unmove(__cplusplus), function, value) << '\n';
   assert_many(value == expected_value, unmove(value_string + " == "), value);
}

template <typename Object, typename Value, typename Func_1, typename Func_2, typename... Args>  
Result_codes bind_execute_member_function_assert(Object & object, Func_1 && m_funct, 
                                                        const Value & expected_value, const string& value_string, const string& function,
                                                        Func_2 && m_funct_args, Args&&... args ) {
   //auto func = bind(m_funct_args, object, std::placeholders::_1);
   //Result_codes result = call_catch_exception(func, forward<Args>(args)...);
   std::reference_wrapper<Object> object_ref_wrapper = std::ref(object);
   Result_codes result = bind_execute_member_function(object_ref_wrapper, m_funct_args, args ...);
   //Result_codes result = bind_execute_member_function(std::ref(object), m_funct_args, forward<Args>(args)...);
   if (OK != result)
      return result;
   //auto bind_function = bind(m_funct, std::cref(object), std::placeholders::_1);
   cerr << " \n-----------------------------\n";
   auto bind_function = std::mem_fn(m_funct);
   Value value = bind_function(object);
   print_and_assert(value, expected_value, value_string, function);
   return result;
}

#endif
