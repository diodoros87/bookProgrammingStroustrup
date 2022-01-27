#define DEBUG_OSTREAM

#include "integer.hpp"
#include "assertions.hpp"
#include "money.hpp"

#include <cmath>

#include "integers_extremums_tests.hpp"
#include "money_operations_test.hpp"
#include "money_init_test.hpp"
#include "money_utility_test.hpp"

#include <functional>

using namespace std;
using namespace integer_space;
using namespace money;
using namespace money_init_test;

template <typename T> 
extern
Money<T> construct_cents(const string & DOLLARS, const long double CENTS, bool creating, const string & expected = "");

template <typename T> 
extern
Money<T> construct(const string & DOLLARS, bool creating, const string & expected/* = ""*/);

namespace money_operations_test {
   
template <typename Function, typename... Args>  
void operation_failed(Function && f, Args&&... args ) { 
   try {
      f(std::forward<Args>(args)...);
      assert(0 && "---------------------- operation should be failed !!!");
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}
/*  
template <typename Function, typename... Args>  
void operation_failed(Function && f, Args&&... args ) { 
   try {
      f(std::forward<Args>(args)...);
      assert(0 && "---------------------- operation should be failed !!!");
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}
*/

/*
template <typename Type, typename... Args>  
void operation_failed(Type& (Type::*func)(const Type&), Args&&... args ) { 
   try {
      f(std::forward<Args>(args)...);
      assert(0 && "---------------------- operation should be failed !!!");
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}
*/
/*
template <typename T, typename Function, typename... Args>  
void operation_failed(Type& (Type::*func)(const Type&), Args&&... args ) { 
   try {
      f(std::forward<Args>(args)...);
      T obj = f(std::forward<Args>(args)...);
      cerr << __func__ << " !!!!!!!!!!!!!!!!!!" << obj << endl;
      assert(0);
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}
  */
//typedef  int (Fred::*FredMemFn)(char x, float y);
//template <typename T, typename T (T::*oper)(const& T)> 
template <typename Type> 
void binary_operation(Type& (Type::*func)(const Type&) , const string & A_DOLLARS, const string & B_DOLLARS, 
                      const string & expected = "") { 
   Type       A(A_DOLLARS);
   const Type B(B_DOLLARS);
   Type RESULT = (A.*func)(B);
   print_assert(RESULT, expected);
   print_assert(A, expected);
}

template <typename Type> 
void binary_operation(Type& (Type::*func)(const Type&) , const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const string & expected = "") { 
   Type       A(A_DOLLARS, A_CENTS);
   const Type B(B_DOLLARS, B_CENTS);
   Type RESULT = (A.*func)(B);
   print_assert(RESULT, expected);
   print_assert(A, expected);
}

template <typename Type> 
void binary_operation(Type (Type::*func)(const Type&) const, const string & A_DOLLARS, const string & B_DOLLARS, 
                      const string & expected = "") { 
   const Type       A(A_DOLLARS);
   const Type B(B_DOLLARS);
   Type RESULT = (A.*func)(B);
   print_assert(RESULT, expected);
}

template <typename Type> 
void binary_operation(Type (Type::*func)(const Type&) const, const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const string & expected = "") { 
   const Type       A(A_DOLLARS, A_CENTS);
   const Type B(B_DOLLARS, B_CENTS);
   Type RESULT = (A.*func)(B);
   print_assert(RESULT, expected);
}

template <typename Type, typename Function> 
void binary_operation(Function && func, const string & A_DOLLARS, const string & B_DOLLARS, const string & expected = "") { 
   const Type A(A_DOLLARS);
   const Type B(B_DOLLARS);
   const Type RESULT = func(A, B);
   print_assert(RESULT, expected);
}

template <typename Type, typename Function> 
void binary_operation(Function && func, const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const string & expected = "") { 
   const Type A(A_DOLLARS, A_CENTS);
   const Type B(B_DOLLARS, B_CENTS);
   const Type RESULT = func(A, B);
   print_assert(RESULT, expected);
}

template <typename Type> 
void unary_operation(Type& (Type::*func)(const Type&) , const string & A_DOLLARS, const string & expected = "") { 
   Type       A(A_DOLLARS);
   Type RESULT = (A.*func)();
   print_assert(RESULT, expected);
   print_assert(A, expected);
}

template <typename T, typename Function> 
void unary_operation(Function && func, const string & A_DOLLARS, const long double A_CENTS, const string & expected = "") { 
   const Money<T> A(A_DOLLARS, A_CENTS);
   const Money<T> RESULT = func(A);
   print_assert(RESULT, expected);
}

template <typename T, typename Function> 
void unary_operation(Function && func, const string & A_DOLLARS, const string & expected = "") { 
   const Money<T> A(A_DOLLARS);
   const Money<T> RESULT = func(A);
   print_assert(RESULT, expected);
}

template <typename Binary_Oper_Type, typename Function, typename... Args>  
void binary_operation_failed(Function && f, Args&&... args ) { 
   try {
      cerr << __func__ <<  endl;
      binary_operation<Binary_Oper_Type>(f, std::forward<Args>(args)...);
      assert(0 && "---------------------- operation should be failed !!!");
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}

template <typename Function, typename... Args>  
void unary_operation_failed(Function && f, Args&&... args ) { 
   try {
      unary_operation(f, std::forward<Args>(args)...);
      assert(0 && "---------------------- operation should be failed !!!");
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}
/*
template <typename Type> 
void unary_operation(Type (Type::*func)(const Type&) const, const string & A_DOLLARS, const long double A_CENTS, const string & expected = "") { 
   Type       A(A_DOLLARS, A_CENTS);
   Type RESULT = (A.*func)(B);
   print_assert(RESULT, expected);
}
*/

inline string dollars_string(const string & DOLLARS, const long double CENTS) {
   string cents = std::to_string(static_cast<int>(CENTS));
   if (CENTS < 10)
      cents.insert(0, "0");
   const string RESULT = DOLLARS + "." + cents;
   cerr << '\n' << __func__ << " RESULT = " << RESULT << '\n';
   return RESULT;
}

template <typename Type, template<typename> class Template = Money>
inline void test_adding(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const string & expected = "") { 
   cerr << "\n\n#########################" << __func__ << '\n';
   binary_operation<Template<Type>, Template<Type>(const Template<Type>&, const Template<Type>&)> (operator+, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
   binary_operation<Template<Type>> (&Template<Type>::operator+=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
   const string a_dollars = dollars_string(A_DOLLARS, A_CENTS);
   const string b_dollars = dollars_string(B_DOLLARS, B_CENTS);
   binary_operation<Template<Type>, Template<Type>(const Template<Type>&, const Template<Type>&)> (operator+, a_dollars, b_dollars, expected);
   binary_operation<Template<Type>> (&Template<Type>::operator+=, a_dollars, b_dollars, expected);
}

template <typename Type, template<typename> class Template = Money>
inline void test_subtracting(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const string & expected = "") { 
   cerr << "\n\n#########################" << __func__ << '\n';
   binary_operation<Template<Type>> (&Template<Type>::operator-=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
   binary_operation<Template<Type>> (&Template<Type>::operator-, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
   const string a_dollars = dollars_string(A_DOLLARS, A_CENTS);
   const string b_dollars = dollars_string(B_DOLLARS, B_CENTS);
   binary_operation<Template<Type>> (&Template<Type>::operator-=, a_dollars, b_dollars, expected);
   binary_operation<Template<Type>> (&Template<Type>::operator-, a_dollars, b_dollars, expected);
}

template <typename Type, template<typename> class Template = Money>
inline void failed_test_adding(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS) { 
   cerr << "\n\n#########################" << __func__ << '\n';
   binary_operation_failed<Template<Type>, Template<Type>& (Template<Type>::*)(const Template<Type>&)>(&Template<Type>::operator+=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS);
   using Alias = Template<Type>;
   binary_operation_failed< Alias, Alias(const Alias&, const Alias&)>(operator+, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS);
   const string a_dollars = dollars_string(A_DOLLARS, A_CENTS);
   const string b_dollars = dollars_string(B_DOLLARS, B_CENTS);
   binary_operation_failed< Alias, Alias(const Alias&, const Alias&)>(operator+, a_dollars, b_dollars);
   binary_operation_failed<Template<Type>, Template<Type>& (Template<Type>::*)(const Template<Type>&)>(&Template<Type>::operator+=, a_dollars, b_dollars);
}

template <typename Type, template<typename> class Template = Money>
inline void failed_test_subtracting(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS) { 
   cerr << "\n\n#########################" << __func__ << '\n';
   binary_operation_failed<Template<Type>, Template<Type>& (Template<Type>::*)(const Template<Type>&)>(&Template<Type>::operator-=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS);
   binary_operation_failed<Template<Type>, Template<Type> (Template<Type>::*)(const Template<Type>&) const>(&Template<Type>::operator-, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS);
   const string a_dollars = dollars_string(A_DOLLARS, A_CENTS);
   const string b_dollars = dollars_string(B_DOLLARS, B_CENTS);
   binary_operation_failed<Template<Type>, Template<Type>& (Template<Type>::*)(const Template<Type>&)>(&Template<Type>::operator-=, a_dollars, b_dollars);
   binary_operation_failed<Template<Type>, Template<Type> (Template<Type>::*)(const Template<Type>&) const>(&Template<Type>::operator-, a_dollars, b_dollars);
   //operation_failed(binary_operation<Template<Type>>, &Template<Type>::operator-=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, "");
   //binary_operation<Template<Type>> (&Template<Type>::operator-=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
}

template <typename T, template<typename> class Template = Money>
void correct_adding() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (! is_same<T, char>::value &&  ! is_same<T, int_fast8_t>::value) {
         test_adding<T>("-1", 28, "0", 2, "-1,26");
         if (! is_same<T, short>::value)
            test_adding<T>("100", 23, "-450", 34, "-350,11");
      }
      test_adding<T, Template>("-0", 23, "-0", 34, "-0,57");
      test_adding<T, Template>("-0", 23, "0", 34, "0,11");
      test_adding<T, Template>("0", 23, "-0", 34, "-0,11");
      
   }
   test_adding<T>("0", 23, "0", 34, "0,57");
}

template <typename T, template<typename> class Template = Money>
void correct_subtracting() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (! is_same<T, char>::value &&  ! is_same<T, int_fast8_t>::value) {
         test_subtracting<T>("-1", 28, "0", 2, "-1,30");
         if (! is_same<T, short>::value)
            test_subtracting<T>("100", 23, "-450", 34, "550,57");
      }
      test_subtracting<T>("-0", 2, "-1", 28, "1,26");
      test_subtracting<T>("-1", 28, "-0", 2, "-1,26");
   }
   test_subtracting<T>("0", 99, "0", 99, "0,00");
}

template <typename T, template<typename> class Template = Money>
void failed_adding() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (is_same<T, char>::value || is_same<T, int_fast8_t>::value) {
         failed_test_adding<T>("-1", 28, "-0", 2);
      }
      if (is_same<T, short>::value)
         failed_test_adding<T>("100", 23, "310", 34);
   }
}

template <typename T, template<typename> class Template = Money>
void failed_subtracting() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (is_same<T, char>::value || is_same<T, int_fast8_t>::value) {
         failed_test_subtracting<T>("-1", 28, "0", 2);
      }
      if (is_same<T, short>::value)
         failed_test_subtracting<T>("100", 23, "-310", 34);
   }
}

template <typename T, template<typename> class Template = Money>
void correct() {
   correct_adding<T>();
   correct_subtracting<T>();
}

template <typename T, template<typename> class Template = Money>
void failed() {
   failed_adding<T>();
   failed_subtracting<T>();
}

template <typename T, template<typename> class Template = Money>
void perform() {
   correct<T>();
   failed<T>();
}

void perform() {
   
   perform<Integer, Money>();
   perform<char, Money>();
   perform<int_fast8_t>();
   perform<short>();
   perform<unsigned short>();
   perform<int>();
   perform<unsigned>();
   perform<long>();
   perform<unsigned long>();
   perform<long long>();
   perform<unsigned long long>();
   perform<float>();
   perform<double>();
   perform<long double>();
   
   
   ///binary_operation<Money<Integer>>(&Money<Integer>::operator+, "100", 23, "-450", 34, "-350,11");
   //binary_operation<Money<Integer>>(Money<Integer> (&operator+)(const Money<Integer> &, const Money<Integer> &), "100", 23, "-450", 34, "-350,11");
   ///binary_operation<Money<Integer>, Money<Integer>(const Money<Integer>&, const Money<Integer>&)>(operator+, "100", 23, "-450", 34, "-350,11");
   //binary_operation<Money<Integer>>(&Money<Integer>::operator*, "100", 23, "-450", 34, "-350,11");
   
   cerr << '\n' << __func__ << ": END of ALL TESTS\n";
}

}
