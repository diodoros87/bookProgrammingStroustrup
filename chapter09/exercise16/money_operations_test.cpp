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
void binary_operation(Type& (Type::*func)(const Type&), const string & A_DOLLARS, const string & B_DOLLARS, 
                      const string & expected = "") { 
   Type       A(A_DOLLARS);
   const Type B(B_DOLLARS);
   Type RESULT = (A.*func)(B);
   print_assert(RESULT, expected);
   print_assert(A, expected);
}

template <typename Type> 
void binary_operation(Type& (Type::*func)(const Type&), const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
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

template <typename Type>
string get_operator_name(bool (Type::*func)(const Type&) const) {
   if (func == &Type::operator==)
      return "operator==";
   if (func == &Type::operator!=)
      return "operator!=";
   if (func == &Type::operator>=)
      return "operator>=";
   if (func == &Type::operator<=)
      return "operator<=";
   if (func == &Type::operator>)
      return "operator>";
   if (func == &Type::operator<)
      return "operator<";
   
   throw std::invalid_argument(string(__func__) + ": not matched operator  ");
}

/*
template <typename Type> 
inline void print_assert_bool(const bool RESULT, const bool expected, bool (Type::*func)(const Type&) const, const Type& A, const Type& B) {
   cerr << std::boolalpha << " RESULT IS " << (RESULT) << '\n'; 
   if((RESULT) != (expected)) { 
      cerr << " " << A << " " << get_operator_name<Type>(func) << " " << B << " returns unexpected result "  << '\n';
      assert(false); 
   }
}
*/
#define print_assert_bool(RESULT, expected, func, A, B) \
   cerr << std::boolalpha << " RESULT IS " << (RESULT) << '\n'; \
   if((RESULT) != (expected)) { \
      cerr << " " << A << " " << get_operator_name<Type>(func) << " " << B << " returns unexpected result "  << '\n'; \
      assert(false); \
   }

template <typename Type> 
void binary_operation(bool (Type::*func)(const Type&) const, const string & A_DOLLARS, const string & B_DOLLARS, 
                      const bool expected) { 
   const Type       A(A_DOLLARS);
   const Type B(B_DOLLARS);
   bool RESULT = (A.*func)(B);
   print_assert_bool(RESULT, expected, func, A, B);
}

template <typename Type> 
void binary_operation(bool (Type::*func)(const Type&) const, const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const bool expected) { 
   const Type       A(A_DOLLARS, A_CENTS);
   const Type B(B_DOLLARS, B_CENTS);
   bool RESULT = (A.*func)(B);
   print_assert_bool(RESULT, expected, func, A, B);
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
/*
template <typename Type> 
void unary_operation(Type& (Type::*func)(const Type&), const string & A_DOLLARS, const string & expected = "") { 
   Type       A(A_DOLLARS);
   Type RESULT = (A.*func)();
   print_assert(RESULT, expected);
   print_assert(A, expected);
}
*/

template <typename Type>
string get_operator_name(Type (Type::*func)() const) {
   typedef Type (Type::*Member_func)() const;
   Member_func plus = &Type::operator+;
   Member_func minus = &Type::operator-;
   if (func == plus)
      return "operator+";
   if (func == minus)
      return "operator-";
   
   throw std::invalid_argument(string(__func__) + ": not matched operator  ");
}

template <typename Type> 
void unary_operation(Type (Type::*func)() const, const string & A_DOLLARS, const long double A_CENTS, const string & expected = "") { 
   const Type A(A_DOLLARS, A_CENTS);
   const Type RESULT = (A.*func)();
   cerr << __func__ << " Tested of " << get_operator_name(func)   << '\n';
   print_assert(RESULT, expected);
}

template <typename Type> 
void unary_operation(Type (Type::*func)() const, const string & A_DOLLARS, const string & expected = "") { 
   const Type A(A_DOLLARS);
   const Type RESULT = (A.*func)();
   cerr << __func__ << " Tested of " << get_operator_name(func)   << '\n';
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

struct Dollars_string {
public:
   const string a_dollars;
   const string b_dollars;
   
   Dollars_string (const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS) : a_dollars(dollars_string(A_DOLLARS, A_CENTS)),
                                                   b_dollars(dollars_string(B_DOLLARS, B_CENTS)) 
   { }
private:
   string dollars_string(const string & DOLLARS, const long double CENTS) {
      string cents = std::to_string(static_cast<int>(CENTS));
      if (CENTS < 10)
         cents.insert(0, "0");
      const string RESULT = DOLLARS + "." + cents;
      cerr << '\n' << __func__ << " RESULT = " << RESULT << '\n';
      return RESULT;
   }
};

string replace_char(const string & S, const string remove, const string insert) {
   const size_t pos = S.find(remove);
   string result = S;
   if (pos != string::npos)
      result.replace(pos, remove.size(), insert);
   return result;
}

template <typename Type, template<typename> class Template = Money>
class Test_unary : public Dollars_string { 
   static_assert(numeric_limits<Type>::is_signed && "Type signed required");
public:
   Test_unary(const string & A_DOLLARS, const long double A_CENTS, const string & expected = "") : 
                      Dollars_string(A_DOLLARS, A_CENTS, "", 0.0L) {
      cerr << "\n\n#########################" << __func__ << '\n';
      unary_operation<Template<Type>> (&Template<Type>::operator+, A_DOLLARS, A_CENTS, replace_dot_by_comma());
      unary_operation<Template<Type>> (&Template<Type>::operator-, A_DOLLARS, A_CENTS, expected);
   }
private:
   string replace_dot_by_comma() {
      string expected_plus = replace_char(Dollars_string::a_dollars, ".", ",");
      if (numeric_limits<Type>::is_integer && expected_plus == "-0,00")
         expected_plus.erase(0, 1);
      cerr << "\n\n#" << __func__ << " expected_plus = " << expected_plus << '\n';
      return expected_plus;
   }
};

template <typename Type, template<typename> class Template = Money>
class Test_adding : public Dollars_string { 
public:
   Test_adding(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const string & expected = "") : 
                      Dollars_string(A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS) {
      cerr << "\n\n#########################" << __func__ << '\n';
      binary_operation<Template<Type>, Template<Type>(const Template<Type>&, const Template<Type>&)> (operator+, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
      binary_operation<Template<Type>> (&Template<Type>::operator+=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
      binary_operation<Template<Type>, Template<Type>(const Template<Type>&, const Template<Type>&)> (operator+, a_dollars, b_dollars, expected);
      binary_operation<Template<Type>> (&Template<Type>::operator+=, a_dollars, b_dollars, expected);
   }
};

template <typename Type, template<typename> class Template = Money>
class Test_subtracting : public Dollars_string { 
public:
   Test_subtracting(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const string & expected = "") :
                      Dollars_string(A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS) { 
      cerr << "\n\n#########################" << __func__ << '\n';
      binary_operation<Template<Type>> (&Template<Type>::operator-=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
      binary_operation<Template<Type>> (&Template<Type>::operator-, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
      binary_operation<Template<Type>> (&Template<Type>::operator-=, a_dollars, b_dollars, expected);
      binary_operation<Template<Type>> (&Template<Type>::operator-, a_dollars, b_dollars, expected);
   }
};

template <typename Type, template<typename> class Template = Money>
class Test_relations : public Dollars_string { 
public:
   Test_relations(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const bool equality, const bool smaller) :
                      Dollars_string(A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS) { 
      cerr << "\n\n#########################" << __func__ << '\n';
      binary_operation<Template<Type>> (&Template<Type>::operator==, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, equality);
      binary_operation<Template<Type>> (&Template<Type>::operator!=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, ! equality);
      binary_operation<Template<Type>> (&Template<Type>::operator==, a_dollars, b_dollars, equality);
      binary_operation<Template<Type>> (&Template<Type>::operator!=, a_dollars, b_dollars, ! equality);
      binary_operation<Template<Type>> (&Template<Type>::operator<=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, equality || smaller);
      binary_operation<Template<Type>> (&Template<Type>::operator>=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, equality || ! smaller);
      binary_operation<Template<Type>> (&Template<Type>::operator<=, a_dollars, b_dollars, equality || smaller);
      binary_operation<Template<Type>> (&Template<Type>::operator>=, a_dollars, b_dollars, equality || ! smaller);
      binary_operation<Template<Type>> (&Template<Type>::operator<, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, smaller);
      binary_operation<Template<Type>> (&Template<Type>::operator>, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, ! smaller && ! equality);
      binary_operation<Template<Type>> (&Template<Type>::operator<, a_dollars, b_dollars, smaller);
      binary_operation<Template<Type>> (&Template<Type>::operator>, a_dollars, b_dollars, ! smaller && ! equality);
   }
};

template <typename Type, template<typename> class Template = Money>
class Failed_test_adding : public Dollars_string { 
public:
   Failed_test_adding(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, const long double B_CENTS) :
                      Dollars_string(A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS) { 
      cerr << "\n\n#########################" << __func__ << '\n';
      binary_operation_failed<Template<Type>, Template<Type>& (Template<Type>::*)(const Template<Type>&)>(&Template<Type>::operator+=,
                     A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS);
      binary_operation_failed<Template<Type>, Template<Type>& (Template<Type>::*)(const Template<Type>&)>(&Template<Type>::operator+=, a_dollars, b_dollars);
      using Alias = Template<Type>;
      binary_operation_failed< Alias, Alias(const Alias&, const Alias&)>(operator+, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS);
      binary_operation_failed< Alias, Alias(const Alias&, const Alias&)>(operator+, a_dollars, b_dollars);
   }
};

template <typename Type, template<typename> class Template = Money>
class Failed_test_subtracting : public Dollars_string { 
public:
   Failed_test_subtracting(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, const long double B_CENTS) :
                      Dollars_string(A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS) {
      cerr << "\n\n#########################" << __func__ << '\n';
      binary_operation_failed<Template<Type>, Template<Type>& (Template<Type>::*)(const Template<Type>&)>(&Template<Type>::operator-=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS);
      binary_operation_failed<Template<Type>, Template<Type>& (Template<Type>::*)(const Template<Type>&)>(&Template<Type>::operator-=, a_dollars, b_dollars);
      using Alias = Template<Type>;
      binary_operation_failed<Alias, Alias (Alias::*)(const Alias&) const>(&Alias::operator-, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS);
      binary_operation_failed<Alias, Alias (Alias::*)(const Alias&) const>(&Alias::operator-, a_dollars, b_dollars);
   }
};

template <typename T, template<typename> class Template = Money>
void correct_adding() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (! is_same<T, char>::value &&  ! is_same<T, int_fast8_t>::value) {
         Test_adding<T>("-1", 28, "0", 2, "-1,26");
         if (! is_same<T, short>::value)
            Test_adding<T>("100", 23, "-450", 34, "-350,11");
      }
      Test_adding<T, Template>("-0", 23, "-0", 34, "-0,57");
      Test_adding<T, Template>("-0", 23, "0", 34, "0,11");
      Test_adding<T, Template>("0", 23, "-0", 34, "-0,11");
      
   }
   Test_adding<T>("0", 23, "0", 34, "0,57");
}

template <typename T, template<typename> class Template = Money>
void correct_subtracting() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (! is_same<T, char>::value &&  ! is_same<T, int_fast8_t>::value) {
         Test_subtracting<T>("-1", 28, "0", 2, "-1,30");
         if (! is_same<T, short>::value)
            Test_subtracting<T>("100", 23, "-450", 34, "550,57");
      }
      Test_subtracting<T>("-0", 2, "-1", 28, "1,26");
      Test_subtracting<T>("-1", 28, "-0", 2, "-1,26");
   }
   Test_subtracting<T>("0", 99, "0", 99, "0,00");
}

template <typename T, template<typename> class Template = Money>
void correct_relations() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (! is_same<T, char>::value &&  ! is_same<T, int_fast8_t>::value) {
         Test_relations<T>("-1", 28, "0", 2, false, true);
         if (! is_same<T, short>::value)
            Test_relations<T>("100", 23, "-450", 34, false, false);
      }
      Test_relations<T, Template>("-0", 23, "-0", 34, false, false);
      Test_relations<T, Template>("-0", 23, "0", 34, false, true);
      Test_relations<T, Template>("0", 23, "-0", 34, false, false);
      
   }
   Test_relations<T>("0", 23, "0", 34, false, true);
   Test_relations<T>("0", 99, "0", 99, true, false);
   Test_relations<T>("0", 0, "0", 0, true, false);
}

template <typename T, template<typename> class Template = Money, enable_if_t<numeric_limits<T>::is_signed, bool>  = true>
void correct_unary() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (! is_same<T, char>::value &&  ! is_same<T, int_fast8_t>::value) {
      Test_unary<T>("-1", 28, "1,28");
      if (! is_same<T, short>::value) {
         Test_unary<T>("100", 23, "-100,23");
         Test_unary<T, Template>("99", 0, "-99,00");
         Test_unary<T>("-99", 99, "99,99");
      }
   }
   Test_unary<T, Template>("-0", 23, "0,23");
   Test_unary<T, Template>("0", 23, "-0,23");
   //Test_unary<T>("0", 0, "0,00");
   Test_unary<T>("-0", 0, "0,00");
}

template <typename T, template<typename> class Template = Money>
void failed_adding() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (is_same<T, char>::value || is_same<T, int_fast8_t>::value) {
         Failed_test_adding<T>("-1", 28, "-0", 2);
      }
      if (is_same<T, short>::value)
         Failed_test_adding<T>("100", 23, "310", 34);
   }
}

template <typename T, template<typename> class Template = Money>
void failed_subtracting() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (is_same<T, char>::value || is_same<T, int_fast8_t>::value) {
         Failed_test_subtracting<T>("-1", 28, "0", 2);
      }
      if (is_same<T, short>::value)
         Failed_test_subtracting<T>("100", 23, "-310", 34);
   }
}

template <typename T, template<typename> class Template = Money>
void correct() {
   correct_adding<T>();
   correct_subtracting<T>();
   correct_relations<T>();
}

template <typename T, template<typename> class Template = Money>
void failed() {
   failed_adding<T>();
   failed_subtracting<T>();
}

template <typename T, template<typename> class Template = Money, enable_if_t<numeric_limits<T>::is_signed, bool>  = true>
void perform() {
   correct<T>();
   correct_unary<T>();
   failed<T>();
}

template <typename T, template<typename> class Template = Money, enable_if_t<false == numeric_limits<T>::is_signed, bool>  = true>
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
   
   cerr << '\n' << __func__ << ": END of ALL TESTS\n";
}

}
