#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "variadic_template.hpp"
#include <type_traits>
#include <utility>
#include <iostream>
#include <functional>

#include "result_codes.h"

using std::cerr;
using std::is_function;
using std::enable_if_t;
using std::forward;
using std::exception;
using std::bind;

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

Result_codes get_error_code(exception * e);

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
struct Money_Creation {
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
struct Template_Constructor {
	template<typename... Args>
	Template<Type> operator()(Args...args) const {
		return Template<Type>(forward<Args>(args)...);
	}
};

template <typename T>
struct Constructor {
   template<typename... Args>
   T operator()(Args&&...args) const {
      return T(forward<Args>(args)...);
   }
};
/*
template <typename T>
struct Destructor {
   void operator()() const {
      ~T();
   }
};
*/
template <typename T>
struct Destructor {
   Result_codes operator()(T * & ptr) {
      if (ptr) {
         delete ptr;
         ptr = nullptr;
         return OK;
      }
      cerr  << __func__ << " Error ptr = " << ptr << '\n';
      return BAD_FUNTION_CALL;
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
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return INVALID_ARG;
   } catch (const std::out_of_range& e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return OUT_OF_RANGE_ERROR;
   } catch (const std::bad_alloc & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return BAD_ALLOC;
   }
   
   
template <typename Type, typename Function, typename... Args>  
inline Result_codes bind_execute_member_function(Type & object, Function && member_function, Args&&... args ) {
   std::reference_wrapper<Type> object_ref_wrapper = std::ref(object);
   auto bind_function = bind(member_function, object_ref_wrapper, std::placeholders::_1);
   Result_codes result = call_catch_exception(bind_function, forward<Args>(args)...);
   return result;
}

using std::bad_alloc; using std::invalid_argument; using std::bad_cast; using std::exception; using std::regex_error; using std::out_of_range;
template <typename Type, typename Function, typename... Args>
Result_codes init(Type * & object_pointer, Function && constructor, Args &&... args) {
   if (object_pointer != nullptr) {
      cerr  << __func__ << " Error NON-null object_pointer type of " << typeid(Type).name() << '\n';
      return INVALID_ARG;
   }
   try {
      void * memory = operator new(sizeof(Type));
      object_pointer = new(memory) Type(constructor(forward<Args>(args)...));
   } 
   catch (const bad_alloc & const_e) {
      cerr  << __func__ << " " << typeid(const_e).name() << " " << const_e.what() << '\n';
      bad_alloc &e = const_cast<bad_alloc &>(const_e);
      return get_error_code(reinterpret_cast<bad_alloc *>(&e));
   } catch (const exception & const_e) {
      cerr  << __func__ << " " << typeid(const_e).name() << " " << const_e.what() << '\n';
      exception &e = const_cast<exception &>(const_e);
      return get_error_code(reinterpret_cast<exception *>(&e));
   } catch (...) {
      cerr  << __func__ << " Unrecognized exception was catched " << '\n';
      return UNRECOGNIZED_ERROR;
   }
   return OK;
}

template <typename Type, typename Function, typename... Args>
Result_codes init(Type & object_ref, Function && constructor, Args &&... args) {
   try {
      object_ref = constructor(forward<Args>(args)...);
   } 
   catch (const bad_alloc & const_e) {
      cerr  << __func__ << " " << typeid(const_e).name() << " " << const_e.what() << '\n';
      bad_alloc &e = const_cast<bad_alloc &>(const_e);
      return get_error_code(reinterpret_cast<bad_alloc *>(&e));
   }
   catch (const invalid_argument & const_e) {
      cerr  << __func__ << " " << typeid(const_e).name() << " " << const_e.what() << '\n';
      invalid_argument &e = const_cast<invalid_argument &>(const_e);
      return get_error_code(reinterpret_cast<invalid_argument *>(&e));
   } catch (const exception & const_e) {
      cerr  << __func__ << " " << typeid(const_e).name() << " " << const_e.what() << '\n';
      exception &e = const_cast<exception &>(const_e);
      return get_error_code(reinterpret_cast<exception *>(&e));
   } catch (...) {
      cerr  << __func__ << " Unrecognized exception was catched " << '\n';
      return UNRECOGNIZED_ERROR;
   }
   return OK;
}
#endif
