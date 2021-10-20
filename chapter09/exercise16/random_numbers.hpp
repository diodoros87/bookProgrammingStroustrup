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

class Generator_Interface {
public:
   //virtual long long operator()(char &&) const { return 0; }  // argument of char to save memory
   virtual long long operator()(char &&) const = 0;
   virtual ~ Generator_Interface() = default;
};

// template <typename T>
// class Base_Generator {
//    static_assert(is_integral<T>::value && "Integral required.");
// public:
//    virtual long long operator()(char &&) const = 0;      // argument of char to save memory
//    
//    explicit Generator(const T & MIN, const T & MAX) : min(MIN) , max(MAX) {
//       if (MIN > MAX)
//          throw invalid_argument("Requirement: MIN <= MAX");
//       time_point<system_clock> point = system_clock::now();
//       duration<long long> since_epoch = duration_cast<seconds>(point.time_since_epoch());
//       long long seed = since_epoch.count();
//       default_random_engine engine (seed);
//       uniform_int_distribution<>::param_type param {MIN, MAX};
//       this->generator = bind(uniform_int_distribution<>{param}, engine);
//    }
//    
//    T operator()() const { return generator(); }
//    T get_min() const { return min; }
//    T get_max() const { return max; }
//    
//    virtual ~ Base_Generator() = default;
// private:
//    function<T()> generator;
//    const T min;
//    const T max;
// };
// 
// template <typename T> 
// class Generator : public Base_Generator {
// public:
//    long long operator()(char &&) const override final { return generator(); }
// };

template <class T> 
class Base_Generator : public Generator_Interface {
   static_assert(is_integral<T>::value && "Integral required.");
public:
   virtual ~ Base_Generator() = default;
   virtual T operator()()  const = 0;
   explicit Base_Generator(const T & MIN, const T & MAX) : min(MIN) , max(MAX) {
      if (MIN > MAX)
         throw invalid_argument("Requirement: MIN <= MAX");
   }
   static long long generate_seed() {
      time_point<system_clock> point = system_clock::now();
      duration<long long> since_epoch = duration_cast<seconds>(point.time_since_epoch());
      long long seed = since_epoch.count();
      return seed;
   }
   //void set_seed(const long long seed) { this->seed = seed; }
   const T min;
   const T max;
};

template <typename T> 
class Generator : public Base_Generator<T> {
public:
   explicit Generator(const T & MIN, const T & MAX) : Base_Generator<T>(MIN, MAX) {
      const long long seed = Base_Generator<T>::generate_seed();
      default_random_engine engine (seed);
      uniform_int_distribution<>::param_type param {MIN, MAX};
      this->generator = bind(uniform_int_distribution<>{param}, engine);
   }
   T operator()() const override final { return generator(); }
   long long operator()(char &&) const override final { return generator(); }
private:
   function<T()> generator;
};

template <typename T> 
class Generator_64 : public Base_Generator<T> {
public:
   explicit Generator_64(const T & MIN, const T & MAX) : Base_Generator<T>(MIN, MAX) {
      const long long seed = Base_Generator<T>::generate_seed();
      std::mt19937_64 engine(seed);
      uniform_int_distribution<>::param_type param {MIN, MAX};
      this->generator = bind(uniform_int_distribution<>{param}, engine);
   }
   T operator()() const override final { return generator(); }
   long long operator()(char &&) const override final { return generator(); }
private:
   function<T()> generator;
};

using std::numeric_limits;
/*
template <typename T> 
struct Min_Max {
   static_assert(is_integral<T>::value && "Integral required.");
public:
   Min_Max(const Min_Max &other) = delete;
   Min_Max & operator=(const Min_Max &) = delete;
   static Min_Max * Get() { 
      return min_max; 
   }
   static const Min_Max * min_max;
private:
   Min_Max() { }
public:
   static constexpr T MIN = numeric_limits<T>::min();
   static constexpr T MAX = numeric_limits<T>::max();
};

template <typename T> 
const Min_Max<T> * Min_Max<T>::min_max = new Min_Max<T>;

template <typename T>
constexpr T Min_Max<T>::MIN;

template <typename T>
constexpr T Min_Max<T>::MAX;
*/

template <typename T> 
struct Generator_MINMAX {
   static_assert(is_integral<T>::value && "Integral required.");
public:
   //static const Min_Max<T> * minmax;
   //static constexpr Generator<T> GENERATOR { Generator<T> { numeric_limits<T>::min(), numeric_limits<T>::max() } } ;
   static const Generator<T> GENERATOR;
   static const Generator_64<T> GENERATOR_64;
   //static const Generator_64<T> GENERATOR_64 { Generator_64<T> { numeric_limits<T>::min(), numeric_limits<T>::max() } } ;
};

//const Generator_MINMAX generator

template <typename T> 
const Generator<T> Generator_MINMAX<T>::GENERATOR { Generator<T> { numeric_limits<T>::min(), numeric_limits<T>::max() } } ;

template <typename T> 
const Generator_64<T> Generator_MINMAX<T>::GENERATOR_64 { Generator_64<T> { numeric_limits<T>::min(), numeric_limits<T>::max() } } ;
/*
template <typename T> 
struct Generator_MINMAX_64 {
   static_assert(is_integral<T>::value && "Integral required.");
public:
   static const Min_Max<T> * minmax_64;
   const Generator_64<T> GENERATOR { Generator_64<T> { Min_Max<T>::MIN, Min_Max<T>::MAX, 'A' } } ;
};

template <typename T> 
const Min_Max<T> * Generator_MINMAX_64<T>::minmax_64 = & Min_Max<T>::min_max;
*/
struct Generator_set { 
   static const Generator_MINMAX<long long> gen_long;
   static const Generator_MINMAX<int> gen_int;
   static const Generator_MINMAX<short>  gen_short;
   static const Generator_MINMAX<char> gen_char;
};

#endif
