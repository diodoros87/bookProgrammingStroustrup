#include <cstdlib>
#include <cstdarg>

#include "connector.h"

#ifdef __cplusplus
#include "variadic_template.hpp"
#include "demo.hpp"
#include "utility.hpp"
#include "null.hpp"

using demo::Demo;
using std::cerr;

using std::bad_alloc; using std::invalid_argument; using std::bad_cast; using std::exception; using std::regex_error; using std::out_of_range;
inline Result_codes get_error_code(exception * e) {
   if (dynamic_cast<bad_alloc*> (e) != nullptr)
      return BAD_ALLOC;
   if (dynamic_cast<invalid_argument*> (e) != nullptr)
      return INVALID_ARG;
   if (dynamic_cast<bad_cast*> (e) != nullptr)
      return BAD_CAST;
   if (dynamic_cast<regex_error*> (e) != nullptr)
      return REGEX_ERROR;
   if (dynamic_cast<out_of_range*> (e) != nullptr)
      return OUT_OF_RANGE_ERROR;
   if (dynamic_cast<std::runtime_error*> (e) != nullptr)
      return RUNTIME_ERROR;
   if (dynamic_cast<exception*> (e) != nullptr)
      return STD_ERROR;
   return UNRECOGNIZED_ERROR;
}

inline Result_codes validate_pointers(Money_type * money_ptr, const char * dollars) {
   if (OK != check_pointer_1_1(money_ptr, __func__, " Error money pointer"))
      return INVALID_ARG;
   if (OK != check_pointer(dollars, __func__, " Error dollars"))
      return INVALID_ARG;
   return OK;
}

//template <class Type, class Function, typename... Args, std::enable_if_t<std::is_function<Function>::value, bool> = true> 
template <class Type, typename Function, typename... Args>
Result_codes call(Money_type * money_ptr, Function && func, Args &&... args) {
   if (OK != check_pointer_1_1(money_ptr, __func__, " Error money pointer"))
      return INVALID_ARG;
   try {
      Money<Type> money = func(std::forward<Args>(args)...);
      *money_ptr = reinterpret_cast<Money<Type> *>(&money);
   } catch (const invalid_argument& const_e) {
      cerr  << __func__ << " " << typeid(const_e).name() << " " << const_e.what() << '\n';
      invalid_argument &e = const_cast<invalid_argument &>(const_e);
      return get_error_code(reinterpret_cast<invalid_argument *>(&e));
   } catch (const out_of_range& const_e) {
      cerr  << __func__ << " " << typeid(const_e).name() << " " << const_e.what() << '\n';
      out_of_range &e = const_cast<out_of_range &>(const_e);
      return get_error_code(reinterpret_cast<out_of_range *>(&e));
   }
   //Money<Type> * money = reinterpret_cast<Money<Type> *>(malloc(sizeof(Money<Type>))) ;
   //*money = func(dollars);
   //*money_ptr = reinterpret_cast<Money_type>(& money);
   //free(money);
   return OK;
} 

extern "C" {
#endif
void set_handler(void (*func)(void)) { 
   std::set_terminate(func); 
}

#ifdef MANUAL_DLL_LOAD
Result_codes Money_type__function(Money_type * money_ptr, const Money_functions function, char * dollars, ... ) {
   if (OK != validate_pointers(money_ptr, dollars))
      return INVALID_ARG;
   
   va_list arg_list;
   va_start( arg_list, dollars );
   long double arg_cents;
   Result_codes result = UNRECOGNIZED_ERROR;
   //Money<int> * money = reinterpret_cast<Money<int> *>(malloc(sizeof(Money<int>))) ;
   switch(function) {
      case INIT_1: 
         //result = call<int, Constructor<int, Money>()>(money_ptr, Constructor<int, Money>(), dollars);
         result = call<int>(money_ptr, Constructor<int, Money>(), dollars);
         break;
      case CREATE_1: 
         result = call<int>(money_ptr, Creation<int>(), dollars);
         break;
      case INIT_2: 
         arg_cents = va_arg(arg_list, long double);
         result = call<int>(money_ptr, Constructor<int, Money>(), dollars, arg_cents);
         break;
      case CREATE_2:
         arg_cents = va_arg(arg_list, long double);
         result = call<int>(money_ptr, Creation<int>(), dollars, arg_cents);
         break;
      default:
         result = INVALID_ARG;
         cerr << "Unrecognized function type = " << function << '\n';
   }
   //*money_ptr = reinterpret_cast<void*>(money);
   //free(money);
   va_end(arg_list);
   return result;
}
#endif

Result_codes Money_type__init_1(Money_type * money_ptr, const char * dollars) {
   if (OK != validate_pointers(money_ptr, dollars))
      return INVALID_ARG;
   Result_codes result = call<int>(money_ptr, Constructor<int, Money>(), dollars);
   return result;
}

Result_codes Money_type__create_1(Money_type * money_ptr, const char * dollars) {
   if (OK != validate_pointers(money_ptr, dollars))
      return INVALID_ARG;
   Result_codes result =  call<int>(money_ptr, Creation<int>(), dollars);
   return result;
}

Result_codes Money_type__init_2(Money_type * money_ptr, const char * dollars, const long double cents) {
   if (OK != validate_pointers(money_ptr, dollars))
      return INVALID_ARG;
   Result_codes result = call<int>(money_ptr, Constructor<int, Money>(), dollars, cents);
   return result;
}

Result_codes Money_type__create_2(Money_type * money_ptr, const char * dollars, const long double cents) {
   if (OK != validate_pointers(money_ptr, dollars))
      return INVALID_ARG;
   Result_codes result = call<int>(money_ptr, Creation<int>(), dollars, cents);
   return result;
}

#ifdef __cplusplus
static Demo * demo_instance = nullptr;
#endif

Result_codes demo_init(const char * name) {
   try {
      if (demo_instance == nullptr)
         demo_instance = new Demo(name);
      else
         return BAD_FUNTION_CALL;
   } catch (const bad_alloc & const_e) {
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

Result_codes demo_set_name(const char * name) {
   if (demo_instance == nullptr) {
      cerr  << __func__ << " Error demo_instance = " << demo_instance << '\n';
      return INVALID_ARG;
   }
   try {
      demo_instance->set_name(name);
   } catch (exception & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return get_error_code(reinterpret_cast<exception *>(&e));
   } catch (...) {
      cerr  << __func__ << " Unrecognized exception was catched " << '\n';
      return UNRECOGNIZED_ERROR;
   }
   return OK;
}

Result_codes demo_get_name(char ** name) {
   if (OK != check_pointer_1_1(name, __func__, " Error name"))
      return INVALID_ARG;
   if (demo_instance == nullptr) {
      cerr  << __func__ << " Error demo_instance = " << demo_instance << '\n';
      return BAD_FUNTION_CALL;
   }
   try {
      *name = demo_instance->get_name();
   } catch (exception & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return get_error_code(reinterpret_cast<exception *>(&e));
   } catch (...) {
      cerr  << __func__ << " Unrecognized exception was catched " << '\n';
      return UNRECOGNIZED_ERROR;
   }
   return OK;
}

Result_codes demo_destroy() {
   if (demo_instance) {
      delete demo_instance;
      return OK;
   }
   cerr  << __func__ << " Error demo_instance = " << demo_instance << '\n';
   return BAD_FUNTION_CALL;
}

#ifdef __cplusplus
}
#endif
