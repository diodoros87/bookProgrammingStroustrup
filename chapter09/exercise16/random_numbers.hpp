#include <random>
#include <type_traits>
#include <functional>
#include <chrono>

using std::is_integral;
using std::uniform_int_distribution;
using std::function;
using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::default_random_engine;

template <typename T> class Int_Generator {
   //static_assert(0 == 0 && "Integral required.");
   static_assert(is_integral<T>::value && "Integral required.");
public:
   Int_Generator(const T & MIN, const T & MAX) {
      time_point<system_clock> point = system_clock::now();
      duration<long long> since_epoch = duration_cast<seconds>(point.time_since_epoch());
      long long seed = since_epoch.count();
      default_random_engine engine (seed);
      uniform_int_distribution<>::param_type param {MIN, MAX};
      generator = bind(uniform_int_distribution<>{param}, engine);
   }
   T operator()() { return generator(); }
private:
   //uniform_int_distribution<>::param_type param;
   function<T()> generator;
};
