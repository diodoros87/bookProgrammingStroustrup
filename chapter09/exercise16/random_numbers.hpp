#ifndef RANDOM_NUMBERS_HPP
#define RANDOM_NUMBERS_HPP

#include <random>
#include <type_traits>
#include <functional>
#include <chrono>
#include <limits>
//#include <algorithm>

using std::is_integral;
using std::uniform_int_distribution;
using std::function;
using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::default_random_engine;

class Base_Generator {
public:
   virtual long long operator()(long long) const { return 0; }
   //virtual long long operator()(long long) const = 0;
   //virtual ~ Base_Generator() = default;
};

template <typename T> 
class Generator : public Base_Generator {
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
   }
   T operator()() const { return generator(); }
   //void* operator()(void*) const { return generator(); }
   long long operator()(long long) const { return generator(); }
   T get_min() const { return min; }
   T get_max() const { return max; }
   
   //virtual ~Generator() = default;
private:
   function<T()> generator;
   const T min;
   const T max;
};

using std::numeric_limits;

template <typename T> 
struct Generator_MINMAX {
   static_assert(is_integral<T>::value && "Integral required.");
public:
   static constexpr T MIN = numeric_limits<T>::min();
   static constexpr T MAX = numeric_limits<T>::max();
   const Generator<T> GENERATOR { Generator<T> { MIN, MAX } } ;
//    Generator<T> get_generator() const { return GENERATOR; }
// private:
//    Generator<T> GENERATOR { Generator<T> { MIN, MAX } } ;
};

template <typename T>
constexpr T Generator_MINMAX<T>::MIN ;

template <typename T>
constexpr T Generator_MINMAX<T>::MAX;

#endif
