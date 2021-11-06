#ifndef NULL_HPP
#define NULL_HPP

#include <string>
#include <type_traits>

using std::string;

template <class Pointer, std::enable_if_t<std::is_pointer<Pointer>::value
                                          || std::is_array<Pointer>::value, bool> = true> 
inline int check_pointer(Pointer pointer, const string & function, const string & message) {
   if (nullptr == pointer) {
      cerr  << function << " : " << message <<  " = " << pointer << '\n'; 
      return INVALID_ARG;
   }
   return OK;
}

template <class T> 
inline int check_pointer_2(T **pointer, const string & function, const string & message) {
   if (OK == check_pointer(pointer, function, message))
      return check_pointer(*pointer, function, message + string(1, '*'));
   return INVALID_ARG;
}

inline int check_pointer(nullptr_t) {
   cerr  << "type: nullptr_t - pointer is null" << '\n';  
   return INVALID_ARG;
}

#endif
