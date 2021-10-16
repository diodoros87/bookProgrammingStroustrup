#include <random>
#include <type_traits>
#include <functional>

using std::is_integral;
using std::uniform_int_distribution;
using std::function;

template <typename T> class Int_Generator {
   //static_assert(0 == 0 && "Integral required.");
   static_assert(is_integral<T>::value && "Integral required.");
public:
   Int_Generator(const T & MIN, const T & MAX) {
      uniform_int_distribution<T>::param_type param {MIN, MAX};
      generator = bind(uniform_int_distribution<T>{param}, default_random_engine());
   }
   T operator()() { return generator(); }
private:
   //uniform_int_distribution<>::param_type param;
   function<T()> generator;
};
