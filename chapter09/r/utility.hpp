#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "variadic_template.hpp"
#include <type_traits>
#include <utility>
#include "result_codes.h"

template <typename Function, typename... Args, std::enable_if_t<std::is_function<Function>::value, bool> = true> 
inline void call(Function && func, Args &&... args ) { 
   func(std::forward<Args>(args)...);
}

template <typename Function, typename... Args> 
inline void call(Function && func, Args &&... args ) { 
   func(std::forward<Args>(args)...);
}

template <typename T>
struct Creation {
	template<typename... Args>
	Money<T> operator()(Args...args) const {
		return Money<T>::create(std::forward<Args>(args)...);
	}
};

template <class Type, template<typename> class Template>
struct Constructor {
	template<typename... Args>
	Template<Type> operator()(Args...args) const {
		return Template<Type>(std::forward<Args>(args)...);
	}
};

template <typename Function, typename... Args>  
Result_codes call_catch_exception(Function && func, Args&&... args )
   try {
      func(std::forward<Args>(args)...);
      return OK;
   } catch (const std::invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << '\n';
      return INVALID_ARG;
   } catch (const std::out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << '\n';
      return OUT_OF_RANGE_ERROR;
   }

#endif
