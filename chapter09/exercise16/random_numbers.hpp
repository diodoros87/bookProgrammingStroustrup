#include <random>
#include <type_traits>
#include <functional>
#include <chrono>
#include <limits>
#include <cmath>

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
   explicit Generator(const T & MIN, const T & MAX) : min(MIN) , max(MAX) {
      if (MIN > MAX)
         throw invalid_argument("Requirement: MIN <= MAX");
      time_point<system_clock> point = system_clock::now();
      duration<long long> since_epoch = duration_cast<seconds>(point.time_since_epoch());
      long long seed = since_epoch.count();
      default_random_engine engine (seed);
      uniform_int_distribution<>::param_type param {MIN, MAX};
      this->generator = bind(uniform_int_distribution<>{param}, engine);
      //(*this).min = engine.min() > MIN ? engine.min() : MIN;
      //unsigned eng_min = engine.min();
      //unsigned eng_max = engine.max();
      (*this).min = fmax(engine.min(), MIN);
      (*this).max = fmin(engine.max(), MAX);
      //(*this).max = engine.max() < MAX ? engine.max() : MAX;
   }
   T operator()() { return generator(); }
   T get_min() { return min; }
   T get_max() { return max; }
private:
   function<T()> generator;
   T min;
   T max;
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
