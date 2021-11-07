#ifndef UTILITY_HPP
#define UTILITY_HPP

template <typename Function, typename... Args, std::enable_if_t<std::is_function<Function>::value, bool> = true> 
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

#endif
