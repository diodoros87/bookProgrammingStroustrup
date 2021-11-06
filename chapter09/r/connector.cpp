#include <cstdlib>

#include "variadic_template.hpp"
#include "demo.hpp"
#include "connector.h"

#ifdef __cplusplus
using demo::Demo;
using std::cerr;

template <class Pointer, std::enable_if_t<std::is_pointer<Pointer>::value
                                          || std::is_array<Pointer>::value, bool> = true> 
inline void check_pointer(Pointer pointer, const string & message) {
   if (nullptr == pointer)
      cerr  << message << '\n';  
}

inline void check_pointer(nullptr_t) {
   cerr  << "type: nullptr_t - pointer is null" << '\n';  
}

extern "C" {
#endif
void set_handler(void (*func)(void)) { 
   std::set_terminate(func); 
}

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

#ifdef __cplusplus
using std::bad_alloc; using std::invalid_argument; using std::bad_cast; using std::exception;
inline Result_codes get_error_code(exception * e) {
   if (dynamic_cast<bad_alloc*> (e) != nullptr)
      return BAD_ALLOC;
   if (dynamic_cast<invalid_argument*> (e) != nullptr)
      return INVALID_ARG;
   if (dynamic_cast<bad_cast*> (e) != nullptr)
      return BAD_CAST;
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
   if (name == nullptr) {
      cerr  << __func__ << " Error name = " << name << '\n';
      return INVALID_ARG;
   }
   if (*name != nullptr) {
      cerr  << __func__ << " Error *name = " << *name << '\n';
      return INVALID_ARG;
   }
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
