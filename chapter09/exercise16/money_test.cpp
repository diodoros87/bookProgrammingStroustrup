//#define DEBUG_OSTREAM

#include "integer.hpp"
#include "assertions.hpp"
#include "money.hpp"

using namespace std;
using namespace integer_space;
using namespace money;

using std::is_signed;
using std::is_unsigned;
  
template <class Number, template<typename> class Money_Template>
static void print_assert(const Money_Template<Number> & money, const string & expected = "") {
   cout << "                                       money = '" << money << "'\n";
   auto t = std::make_tuple(static_cast<string>(money), " != ", expected);
#ifdef DEBUG_OSTREAM
   cout << "\n\n------------------               DEBUG_OSTREAM\n\n";
   cout << "                                       money = '";
   ostringstream * os = reinterpret_cast<ostringstream *>(& cout);
   os << money;
   cout << "'\n";
   ostringstream ostrs;
   string output = operator<<(&ostrs, money).str();
   assert_many(output == expected, t);
#endif
   assert_many(string(money) == expected, t);
}

template <typename T>
struct Creation {
	template<typename... Args>
	Money<T> operator()(Args...args) const {
		return Money<T>::create(std::forward<Args>(args)...);
	}
};

template <typename Function, typename... Args> 
void call(Function && f, Args &&... args ) { 
   f(std::forward<Args>(args)...);
}

template <typename Function, typename... Args>  
void construct_incorrect(Function && f, Args&&... args ) { 
   try {
      f(std::forward<Args>(args)...);
      assert(0);
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}

template <typename T> 
Money<T> construct_cents(const string & DOLLARS, const double CENTS, bool creating, const string & expected = "") { 
   Money<T> money(DOLLARS, CENTS);
   print_assert(money, expected);
   if (creating) {
      Money<T> money_creating = Money<T>::create(DOLLARS, CENTS);
      print_assert(money_creating, expected);
      assert(money == money_creating);
   }
   else
      construct_incorrect(Creation<T>(), DOLLARS, CENTS);
   return money;
}


template <typename T> 
Money<T> construct(const string & DOLLARS, bool creating, const string & expected = "") { 
   Money<T> money(DOLLARS);
   print_assert(money, expected);
   if (creating) {
      Money<T> money_creating = Money<T>::create(DOLLARS);
      print_assert(money_creating, expected);
      assert(money == money_creating);
   }
   else
      construct_incorrect(Creation<T>(), DOLLARS);
   return money;
}

template <typename T> 
void incorrect() { 
   try {
      Money<T> money("6", -34);
      assert(0);
      cerr << __func__ << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
   try {
      Money<T> money("6", 100);
      assert(0);
      cout << __func__ << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}

constexpr long double CONVERTER = (long double)(CENTS_PER_DOLLAR);

template <typename T> 
void construct() {
   cerr << __func__ << '\n';
   construct_cents<T>("0", 99, true, "0,99");
   construct_cents<T>("1", 1, true, "1,01");
   construct_cents<T>("1", 0, true, "1,00");
   construct_cents<T>("0", 99.9, false, "1,00");
   construct_cents<T>("0", 0, true, "0,00");
   construct<T>("0.99999", false, "1,00");
   construct<T>("0.5", true, "0,50");
   construct<T>("0.50", true, "0,50");
   construct<T>("0.500", true, "0,50");
   if (numeric_limits<T>::is_signed) {
      construct_cents<T>("-1", 0, true, "-1,00");
      construct_cents<T>("-1", 1, true, "-1,01");
      construct_cents<T>("-1", 3, true, "-1,03");
      construct_cents<T>("-0", 99.9, false, "-1,00");
      construct_cents<T>("-0", 1, true, "-0,01");
      construct_cents<T>("-0", 99, true, "-0,99");
      construct<T>("-0.99999", false, "-1,00");
      construct<T>("-0.5", true, "-0,50");
      construct<T>("-0.50", true, "-0,50");
      construct<T>("-0.500", true, "-0,50");
   }
   if (! is_same<T, char>::value && ! is_same<T, int_fast8_t>::value) {
      construct<T>("1.99999", false, "2,00");
      
      construct_cents<T>("20", 55, true, "20,55");
      construct_cents<T>("577", 45.7, false, "577,46");
      if (numeric_limits<T>::is_signed) {
         construct_cents<T>("-8577", 45.79, false, "-8577,46");   
         construct<T>("-1.99999", false, "-2,00");
         construct_cents<T>("-1", 99.9, false, "-2,00");
         construct<T>("-0.5", true, "-0,50");
         construct_cents<T>("-1", 99, true, "-1,99");
      }
      construct<T>("10", true, "10,00");
      if (is_floating_point<T>::value) {
         construct<T>("8.577444e+02", false, "857,74");
         construct<T>("8.577555e+02", false, "857,76");
         if (is_same<T, float>::value) {
            construct<T>("-8577e+03", true, "-8577000,68");
         }
         if (is_same<T, double>::value || is_same<T, long double>::value) {
            construct<T>("-8577e+03", true, "-8577000,00");
         }
      }
      construct<T>("10.67", true, "10,67");
      construct<T>("10.679", false, "10,68");
      construct<T>("10.6435", false, "10,64");
      construct<T>("10.099", false, "10,10");
   }
   if (is_same<T, char>::value || is_same<T, int_fast8_t>::value) {
//#if defined(__clang__)
      const long double MAX = { numeric_limits<T>::max() };
      const long double LOWEST = { numeric_limits<T>::lowest() };
      construct<T>((std::to_string(MAX / CONVERTER)), true, "1,27");
      construct<T>((std::to_string(LOWEST / CONVERTER)), true, "-1,28");
//#elif defined(__GNUG__)
      //construct<T>((std::to_string(numeric_limits<T>::max() / CONVERTER)), true, "1,27");
      //construct<T>((std::to_string(numeric_limits<T>::lowest() / CONVERTER)), true, "-1,28");
//#endif
   }
   cerr << "END OF " << __func__ << '\n';
}

template <class Type, template<typename> class Template>
struct Constructor {
	template<typename... Args>
	Template<Type> operator()(Args...args) const {
		return Template<Type>(std::forward<Args>(args)...);
	}
};

#if defined(__clang__)

#elif defined(__GNUG__)
void test_Integer_overflow() {
   const array <Integer, 2> EXTREMUMS = { numeric_limits<Integer>::max(), numeric_limits<Integer>::lowest() };
   const Integer FACTOR = Integer(15);
   const Integer Integer_CONVERTER = Integer(CONVERTER);
   for (size_t i = 0; i < EXTREMUMS.size(); i++)
      try {
         const string MAX_STR = std::to_string((Integer(15) * EXTREMUMS[i]) / Integer_CONVERTER);
         assert(0);
         cout << __func__ << " MAX_STR for type = " << typeid(Integer).name() << " " << MAX_STR << '\n';
      } catch (const out_of_range& e) {
         cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
      }
}
#endif

template <typename T> 
void construct_incorrect() {
   cerr << __func__ << '\n';
   incorrect<T>();
   construct_incorrect(Constructor<T, Money>(), "20.8", 5.0L);
   construct_incorrect(Constructor<T, Money>(), "20.8", 5);
   construct_incorrect(Constructor<T, Money>(), "20t", 7);
   if (is_same<T, char>::value || is_same<T, int_fast8_t>::value) {
      construct_incorrect(Constructor<T, Money>(), "-1.99999");
      construct_incorrect(Constructor<T, Money>(), "1.99999");
      construct_incorrect(Constructor<T, Money>(), "189.99999");
//#if defined(__clang__)
      const long double MAX = { numeric_limits<T>::max() };
      const long double LOWEST = { numeric_limits<T>::lowest() };
      construct_incorrect(Constructor<T, Money>(), (std::to_string(1 + MAX / CONVERTER)));
      construct_incorrect(Constructor<T, Money>(), (std::to_string(-1 + LOWEST / CONVERTER)));
//#elif defined(__GNUG__)
//      construct_incorrect(Constructor<T, Money>(), (std::to_string((1 + numeric_limits<T>::max()) / CONVERTER)));
//      construct_incorrect(Constructor<T, Money>(), (std::to_string((-1 + numeric_limits<T>::lowest()) / CONVERTER)));
//#endif
   }
   else if (is_same<T, Integer>::value) {
#if defined(__clang__)
      const long double MAX = { numeric_limits<T>::max() };
      const long double LOWEST = { numeric_limits<T>::lowest() };
      construct_incorrect(Constructor<T, Money>(), (std::to_string(10 * MAX / CONVERTER)));
      construct_incorrect(Constructor<T, Money>(), (std::to_string(10 * LOWEST / CONVERTER)));
#elif defined(__GNUG__)
      test_Integer_overflow();
      //construct_incorrect(Constructor<T, Money>(), (std::to_string((15 * numeric_limits<T>::max()) / CONVERTER)));
      //construct_incorrect(Constructor<T, Money>(), (std::to_string((15 * numeric_limits<T>::lowest()) / CONVERTER)));
#endif
   }
   else if (is_same<T, float>::value) {
//#if defined(__clang__)
      const long double MAX = { numeric_limits<T>::max() };
      const long double LOWEST = { numeric_limits<T>::lowest() };
      construct_incorrect(Constructor<T, Money>(), (std::to_string(1.0 + MAX / CONVERTER)));
      construct_incorrect(Constructor<T, Money>(), (std::to_string(-1.0 + LOWEST / CONVERTER)));
//#elif defined(__GNUG__)
//      construct_incorrect(Constructor<T, Money>(), (std::to_string((1.0 + numeric_limits<T>::max()) / CONVERTER)));
//      construct_incorrect(Constructor<T, Money>(), (std::to_string((-1.0 + numeric_limits<T>::lowest()) / CONVERTER)));
//#endif
   }
   
   //construct_incorrect<T>("-0", 4);
   if (is_unsigned<T>::value)
      construct_incorrect(Constructor<T, Money>(), "-1", 4);
   //construct_incorrect<T>("+0", 4);
   if (numeric_limits<T>::is_integer) {
      construct_incorrect(Constructor<T, Money>(), "-8577e+03");
      construct_incorrect(Constructor<T, Money>(), "-8.577444e+02");
      construct_incorrect(Constructor<T, Money>(), "8.577555e+02");
   }
   
   construct_incorrect(Constructor<T, Money>(), "inf", 8);
   construct_incorrect(Constructor<T, Money>(), "inf");
   //construct_incorrect<T>("57.7");
   cerr << "END OF " << __func__ << '\n';
}

template <typename T>
inline void test() {
   construct<T>();
   construct_incorrect<T>();
}

int main() {
   try {
      test<char>();
      test<int_fast8_t>();
      test<int>();
      test<long double>();
      test<double>();
      test<float>();
      test<Integer>();
      test<unsigned int>();
      
      //test<string>();
      return 0;
   } catch (const bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const Arithmetic_Error & e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const runtime_error& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const exception& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (...) {
      cerr << __func__ << " unrecognized exception\n";
   }
   return 1;
}
