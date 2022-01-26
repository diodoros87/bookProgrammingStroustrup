#ifndef MONEY_INIT_TEST_HPP
#define MONEY_INIT_TEST_HPP

#include "money.hpp"

using money::Money;

namespace money_init_test {
   
   constexpr long double CONVERTER = (long double)(money::CENTS_PER_DOLLAR);

   template <typename T>
   struct Creation {
      template<typename... Args>
      Money<T> operator()(Args...args) const {
         return Money<T>::create(std::forward<Args>(args)...);
      }
   };

   void perform();
}

#endif