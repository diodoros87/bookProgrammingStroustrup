#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "variadic_template.hpp"
#include <type_traits>
#include <utility>
#include <iostream>

#include "result_codes.h"

using std::cerr;
using std::is_function;
using std::enable_if_t;
using std::forward;

extern template class Money<short>;
extern template class Money<unsigned short>;
extern template class Money<int>;
extern template class Money<unsigned int>;
extern template class Money<long>;
extern template class Money<unsigned long>;
extern template class Money<long long>;
extern template class Money<unsigned long long>;
extern template class Money<float>;
extern template class Money<double>;
extern template class Money<long double>;
/*
class Money<short>;
template class Money<unsigned short>;
template class Money<int>;
template class Money<unsigned int>;
template class Money<long>;
template class Money<unsigned long>;
template class Money<long long>;
template class Money<unsigned long long>;
template class Money<float>;
template class Money<double>;
template class Money<long double>;

extern template Money<short> create(const string &);
extern template Money<unsigned short> create(const string &);
extern template Money<int> create(const string &);
extern template Money<unsigned int> create(const string &);
extern template Money<long> create(const string &);
extern template Money<unsigned long> create(const string &);
extern template Money<long long> create(const string &);
extern template Money<unsigned long long> create(const string &);
extern template Money<float> create(const string &);
extern template Money<double> create(const string &);
extern template Money<long double> create(const string &);*/

template <typename Function, typename... Args, enable_if_t<is_function<Function>::value, bool> = true> 
inline void call(Function && func, Args &&... args ) { 
   func(forward<Args>(args)...);
}

template <typename Function, typename... Args> 
inline void call(Function && func, Args &&... args ) { 
   func(forward<Args>(args)...);
}

template <typename Result, typename Function, typename... Args, enable_if_t<is_function<Function>::value, bool> = true> 
inline Result function_result(Function && func, Args &&... args ) { 
   return func(forward<Args>(args)...);
}

template <typename Result, typename Function, typename... Args> 
inline Result function_result(Function && func, Args &&... args ) { 
   return func(forward<Args>(args)...);
}

template <typename T>
struct Creation {
	template<typename... Args>
	Money<T> operator()(Args...args) const {
		return Money<T>::create(std::forward<Args>(args)...);
	}
};

/*
template <class Type, template<typename> class Template>
struct Template_static_function {
	template <typename Function, typename... Args>
	Template<Type> operator()(Function && func, Args...args) const {
		return Template<Type>::func(std::forward<Args>(args)...);
	}
};
*/
template <class Type, template<typename> class Template>
struct Constructor {
	template<typename... Args>
	Template<Type> operator()(Args...args) const {
		return Template<Type>(forward<Args>(args)...);
	}
};

template<class T> 
inline T& unmove(T&& t) { return t; }

template <typename Function, typename... Args>  
Result_codes call_catch_exception(Function && func, Args&&... args )
   try {
      func(forward<Args>(args)...);
      return OK;
   } catch (const std::invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << '\n';
      return INVALID_ARG;
   } catch (const std::out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << '\n';
      return OUT_OF_RANGE_ERROR;
   }

#endif
