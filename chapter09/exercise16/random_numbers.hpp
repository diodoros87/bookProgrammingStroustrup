#include <random>
#include <type_traits>
#include <functional>
#include <chrono>
#include<limits>

using std::is_integral;
using std::uniform_int_distribution;
using std::function;
using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::default_random_engine;

template <typename T> 
class Generator {
   static_assert(is_integral<T>::value && "Integral required.");
public:
   explicit Generator(const T & MIN, const T & MAX) {
      time_point<system_clock> point = system_clock::now();
      duration<long long> since_epoch = duration_cast<seconds>(point.time_since_epoch());
      long long seed = since_epoch.count();
      default_random_engine engine (seed);
      uniform_int_distribution<>::param_type param {MIN, MAX};
      generator = bind(uniform_int_distribution<>{param}, engine);
   }
   T operator()() { return generator(); }
private:
   function<T()> generator;
};

using std::numeric_limits;

template <typename T> 
struct Generator_MINMAX {
   static_assert(is_integral<T>::value && "Integral required.");
public:
   static constexpr T MIN = numeric_limits<T>::min();
   static constexpr T MAX = numeric_limits<T>::max();
   const Generator<T> GENERATOR { Generator<T> { MIN, MAX } } ;
};

template <typename T>
constexpr T Generator_MINMAX<T>::MIN ;

template <typename T>
constexpr T Generator_MINMAX<T>::MAX;
