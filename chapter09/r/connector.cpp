#include <cstdlib>
#include <cstdarg>

#include "variadic_template.hpp"
#include "demo.hpp"
#include "connector.h"
#include "null.hpp"

#ifdef __cplusplus
using demo::Demo;
using std::cerr;

extern "C" {
#endif
void set_handler(void (*func)(void)) { 
   std::set_terminate(func); 
}

Money_int Money_int__function(const char * dollars, ...) {
   Money<int> money = Money<int>(dollars);
   void* result = reinterpret_cast<void*>(& money);
   return result;
}

int Money_int__function(Money_int* money_ptr, Money_functions function, const char * dollars, ... ) {
   if (OK != check_pointer_2(money_ptr, __func__, " Error money pointer"))
      return INVALID_ARG;
   if (OK != check_pointer(dollars, __func__, " Error dollars"))
      return INVALID_ARG;
   va_list arg_list;

   va_start( arg_list, dollars );
   Money<int> money;
   long double arg_cents;
   switch(function) {
      case INIT_1:
         money = Money<int>(dollars);
         break;
      case CREATE_1:
         money = Money<int>::create(dollars);
         break;
      case INIT_2:
         arg_cents = va_arg( arg_list, long double );
         money = Money<int>(dollars, arg_cents);
         break;
      case CREATE_2:
         arg_cents = va_arg( arg_list, long double );
         money = Money<int>::create(dollars, arg_cents);
         break;
      default:
         cerr << "Unrecognized function = " << function << '\n';
         return INVALID_ARG;
   }
   money_ptr* = reinterpret_cast<void*>(& money);
   return OK;
}
/*
Money_int Money_int__init_1(const char * dollars) {
   Money<int> money = Money<int>(dollars);
   void* result = reinterpret_cast<void*>(& money);
   return result;
}

Money_int Money_int__create_1(const char * dollars) {
   Money<int> money =  Money<int>::create(dollars);
   void* result = reinterpret_cast<void*>(& money);
   return result;
}

Money_int Money_int__init_2(const char * dollars, const long double cents) {
   Money<int> money =  Money<int>(dollars, cents);
   void* result = reinterpret_cast<void*>(& money);
   return result;
}

Money_int Money_int__create_2(const char * dollars, const long double cents) {
   Money<int> money =  Money<int>::create(dollars, cents);
   void* result = reinterpret_cast<void*>(& money);
   return result;
}
*/
#ifdef __cplusplus
using std::bad_alloc; using std::invalid_argument; using std::bad_cast; using std::exception; using std::regex_error;
inline Result_codes get_error_code(exception * e) {
   if (dynamic_cast<bad_alloc*> (e) != nullptr)
      return BAD_ALLOC;
   if (dynamic_cast<invalid_argument*> (e) != nullptr)
      return INVALID_ARG;
   if (dynamic_cast<bad_cast*> (e) != nullptr)
      return BAD_CAST;
   if (dynamic_cast<regex_error*> (e) != nullptr)
      return REGEX_ERROR;
   if (dynamic_cast<exception*> (e) != nullptr)
      return STD_ERROR;
   
   return UNRECOGNIZED_ERROR;
}

static Demo * demo_instance = nullptr;
#endif

int demo_init(const char * name) {
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

int demo_set_name(const char * name) {
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

int demo_get_name(char ** name) {
   if (OK != check_pointer_2(name, __func__, " Error name"))
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

int demo_destroy() {
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
