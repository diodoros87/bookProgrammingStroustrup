#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include "assertions.hpp"
#include <gmpxx.h>

#ifndef NDEBUG
#  define ASSERT_CONDITIONAL(MpzOrLongLong, Integer, boolean) \
   (boolean ? ASSERT_MPZ_INTEGER(reinterpret_cast<mpz_class& >(MpzOrLongLong), Integer, __FILE__, __LINE__, __DATE__, __TIME__) \
   : ASSERT_NUMBER_INTEGER(reinterpret_cast<long long &>(MpzOrLongLong), Integer, __FILE__, __LINE__, __DATE__, __TIME__));
#else
#   define ASSERT_CONDITIONAL(MpzOrLongLong, Integer) ;
#endif

#endif
