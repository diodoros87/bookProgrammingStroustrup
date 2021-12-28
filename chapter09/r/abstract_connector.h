#ifndef ABSTRACT_CONNECTOR_H
#define ABSTRACT_CONNECTOR_H

#include "result_codes.h"

#ifdef __cplusplus

   #include "base.hpp"
   #include "null.hpp"
   #include "utility.hpp"
   
   using Hierarchy::Base;
   
   //using std::invalid_argument;
   using std::is_base_of;

   //// template <class T, enable_if_t<is_base_of<Base, T>::value, bool> = true>
   template <class T>
   class Abstract_connector {
   public:
      Result_codes pv_number(int * const);
      Result_codes pv_char(char * const);
      virtual Result_codes pv_X(double * const) = 0;
      Result_codes virt_set_X(const double);
      Result_codes pv_Y(double * const);
      Result_codes virt_set_Y(const double);
      Result_codes virt_area(double * const);
      Result_codes number(int * const);
      Result_codes destruct();
//       virtual ~Abstract_connector();
   protected:
      static T * instance;
//       Abstract_connector();
   };
   
   //// template <class T, enable_if_t<is_base_of<Base, T>::value, bool> = true>
   template <class T>
   Result_codes Abstract_connector<T>::pv_number(int * const n) {
      if (OK != check_pointer(n, __func__, " Error number pointer")) 
         return INVALID_ARG;
      *n = instance->pv_number();
      return OK;
   }
   
   //// template <class T, enable_if_t<is_base_of<Base, T>::value, bool> = true>
   template <class T>
   Result_codes Abstract_connector<T>::pv_char(char * const c) {
      if (OK != check_pointer(c, __func__, " Error char pointer")) 
         return INVALID_ARG;
      *c = instance->pv_char();
      return OK;
   }
   
   //// template <class T, enable_if_t<is_base_of<Base, T>::value, bool> = true>
//    template <class T>
//    Result_codes Abstract_connector<T>::pv_X(double * const x) {
//       if (OK != check_pointer(x, __func__, " Error number pointer")) 
//          return INVALID_ARG;
//       *x = instance->pv_X();
//       return OK;
//    }
   
   //// template <class T, enable_if_t<is_base_of<Base, T>::value, bool> = true>
   template <class T>
   Result_codes Abstract_connector<T>::virt_set_X(const double x) {
      try {
         instance->virt_set_X(x);
      } catch (exception & e) {
         cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
         return get_error_code(reinterpret_cast<exception *>(&e));
      } catch (...) {
         cerr  << __func__ << " Unrecognized exception was catched " << '\n';
         return UNRECOGNIZED_ERROR;
      }
      return OK;
   }
   
   // template <class T, enable_if_t<is_base_of<Base, T>::value, bool> = true>
   template <class T>
   Result_codes Abstract_connector<T>::pv_Y(double * const y) {
      if (OK != check_pointer(y, __func__, " Error number pointer")) 
         return INVALID_ARG;
      *y = instance->pv_Y();
      return OK;
   }
   
   // template <class T, enable_if_t<is_base_of<Base, T>::value, bool> = true>
   template <class T>
   Result_codes Abstract_connector<T>::virt_set_Y(const double y) {
      try {
         instance->virt_set_Y(y);
      } catch (exception & e) {
         cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
         return get_error_code(reinterpret_cast<exception *>(&e));
      } catch (...) {
         cerr  << __func__ << " Unrecognized exception was catched " << '\n';
         return UNRECOGNIZED_ERROR;
      }
      return OK;
   }
   
   // template <class T, enable_if_t<is_base_of<Base, T>::value, bool> = true>
   template <class T>
   Result_codes Abstract_connector<T>::virt_area(double * const area) {
      if (OK != check_pointer(area, __func__, " Error double pointer")) 
         return INVALID_ARG;
      *area = instance->virt_area();
      return OK;
   }
   
   // template <class T, enable_if_t<is_base_of<Base, T>::value, bool> = true>
   template <class T>
   Result_codes Abstract_connector<T>::number(int * const number) {
      if (OK != check_pointer(number, __func__, " Error int pointer")) 
         return INVALID_ARG;
      *number = instance->number();
      return OK;
   }
   
   // template <class T, enable_if_t<is_base_of<Base, T>::value, bool> = true>
   template <class T>
   Result_codes Abstract_connector<T>::destruct() {
      Result_codes result = Destructor<T>()(instance);
      return result;
   }

#endif

#ifdef __cplusplus
extern "C" {
#endif
   
#ifdef  __cplusplus
}
#endif

//Result_codes test_base_connector(void);

#endif
