#ifndef MONEY_UTILITY_TEST_HPP
#define MONEY_UTILITY_TEST_HPP

#define DEBUG_OSTREAM

//#include "integer.hpp"
#include "assertions.hpp"
//#include "money.hpp"

//using namespace std;
//using namespace integer_space;
//using namespace money;

using std::cout;
  
template <class Number, template<typename> class Template>
static void print_assert(const Template<Number> & object, const string & expected = "") {
   static const string TYPE_NAME = typeid(Template<Number>).name();
   cout << "                                       object of type " << TYPE_NAME << " = '" << object << "'\n";
   auto t = std::make_tuple(static_cast<string>(object), " != ", expected);
#ifdef DEBUG_OSTREAM
   cout << "\n------------------               DEBUG_OSTREAM\n";
   cout << "                                       object of type " << TYPE_NAME << " = '" << object << "'\n";
   ostringstream * os = reinterpret_cast<ostringstream *>(& cout);
   os << object;
   cout << "'\n";
   ostringstream ostrs;
   string output = operator<<(&ostrs, object).str();
   assert_many(output == expected, t);
#endif
   assert_many(string(object) == expected, t);
}

template <class Type, template<typename> class Template>
struct Constructor {
	template<typename... Args>
	Template<Type> operator()(Args...args) const {
		return Template<Type>(std::forward<Args>(args)...);
	}
};

#endif
