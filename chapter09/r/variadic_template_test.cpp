#include "variadic_template_test.hpp"
#include "utility.hpp"
#include "variadic_template.hpp"

namespace tests {
   
Result_codes test_money() {
   Result_codes result = call_catch_exception(Constructor<int, Money>(), "-20", -80.0L);
   if (OK == result)
      call_catch_exception(Constructor<long long, Money>(), "78");
   if (OK == result)
      call_catch_exception(Creation<unsigned long long>(), "2", 5);
   if (OK == result)
      call_catch_exception(Creation<long double>(), "7");
   return result;
}

}
