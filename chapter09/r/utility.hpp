#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "variadic_template.hpp"
#include <type_traits>
#include <utility>

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

struct A {
   template<class T, class = decltype(std::declval<T&>()()),
   class = typename std::enable_if<std::is_pointer<T>::value>::type>
   A(T f) { f(); }
};

#endif
