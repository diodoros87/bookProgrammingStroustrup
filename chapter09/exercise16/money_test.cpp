#define DEBUG_OSTREAM

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

extern "C" void* create_by_pointer_2(const callback pointer, const char * DOLLARS, const double CENTS) {
   return pointer(DOLLARS, CENTS);
   //return pointer(DOLLARS, CENTS);
   //return Money<T>::create(DOLLARS, CENTS);
}

/*
template <typename T>
extern "C" bool create_construct (const char * DOLLARS, const double CENTS) {
   void * pointer = create_construct
   return Integer::is_zero(*integer);
   return Money<T>::create(DOLLARS, CENTS);
}
*/

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
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

/*
template <typename T> 
void construct_incorrect(const string & DOLLARS) { 
   try {
      Money<T> money(DOLLARS);
      assert(0);
      cout << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

template <typename T> 
void create_incorrect(const string & DOLLARS) { 
   try {
      Money<T> money = Money<T>::create(DOLLARS);
      assert(0);
      cout << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

template <typename T> 
void create_incorrect(const string & DOLLARS, const double CENTS ) { 
   try {
      Money<T> money = Money<T>::create(DOLLARS, CENTS);
      //Money<T> money = static_castMoney<T>::create(DOLLARS, CENTS);
      assert(0);
      cout << __func__ << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

template <typename T> 
void construct_incorrect(const string & DOLLARS, const double CENTS ) { 
   try {
      Money<T> money(DOLLARS, CENTS);
      assert(0);
      cerr << __func__ << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}
*/
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
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   try {
      Money<T> money("6", 100);
      assert(0);
      cout << __func__ << "money = " << money << '\n';
   } catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
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
      construct<T>((std::to_string(numeric_limits<T>::max() / CONVERTER)), true, "1,27");
      construct<T>((std::to_string(numeric_limits<T>::lowest() / CONVERTER)), true, "-1,28");
   }
   cerr << "END OF " << __func__ << '\n';
}

//template <typename T>
/*
template <class Number, template<typename> class Money_Template>
struct Constructor {
	template<typename... Args>
	Money_Template<Number> operator()(Args...args) const {
		return Money_Template<Number>(std::forward<Args>(args)...);
	}
};
*/
template <typename T>
struct Constructor {
	template<typename... Args>
	Money<T> operator()(Args...args) const {
		return Money<T>(std::forward<Args>(args)...);
	}
};

template <typename Fn>
auto Bar(Fn f){
   return f("20.8",20);
}

template <typename T> 
void construct_incorrect() {
   cerr << __func__ << '\n';
   incorrect<T>();
   construct_incorrect(Constructor<T>(), "20.8", 5.0L);
   construct_incorrect(Constructor<T>(), "20.8", 5);
   construct_incorrect(Constructor<T>(), "20t", 7);
   if (is_same<T, char>::value || is_same<T, int_fast8_t>::value) {
      construct_incorrect(Constructor<T>(), "-1.99999");
      construct_incorrect(Constructor<T>(), "1.99999");
      construct_incorrect(Constructor<T>(), "189.99999");
      construct_incorrect(Constructor<T>(), (std::to_string((1 + numeric_limits<T>::max()) / CONVERTER)));
      construct_incorrect(Constructor<T>(), (std::to_string((-1 + numeric_limits<T>::lowest()) / CONVERTER)));
   }
   //construct_incorrect<T>("-0", 4);
   if (is_unsigned<T>::value)
      construct_incorrect(Constructor<T>(), "-1", 4);
   //construct_incorrect<T>("+0", 4);
   if (numeric_limits<T>::is_integer) {
      construct_incorrect(Constructor<T>(), "-8577e+03");
      construct_incorrect(Constructor<T>(), "-8.577444e+02");
      construct_incorrect(Constructor<T>(), "8.577555e+02");
   }
   if (is_same<T, float>::value) {
      construct_incorrect(Constructor<T>(), (std::to_string((1.0 + numeric_limits<T>::max()) / CONVERTER)));
      construct_incorrect(Constructor<T>(), (std::to_string((-1.0 + numeric_limits<T>::lowest()) / CONVERTER)));
   }
   construct_incorrect(Constructor<T>(), "inf", 8);
   construct_incorrect(Constructor<T>(), "inf");
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