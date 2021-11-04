#include "variadic_template.hpp"

using std::cerr;

#define LOG_FUNC(x) cerr << '\n' << #x " = " << x << '\t'
#define LOG_ARG(x) cerr << #x " = " << x << '\t'

template <typename T>
Money<T>::Money(const string & dollars, const long double cents) {
   cerr << "\n C++ ";
   LOG_FUNC(__func__);
   LOG_ARG(dollars);
   LOG_ARG(cents);
}

template <typename T>
Money<T>::Money(const string & dollars) {
   cerr << "\n C++ ";
   LOG_FUNC(__func__);
   LOG_ARG(dollars);
}

template <typename T>
Money<T> Money<T>::create(const string & dollars) {   
   cerr << "\n C++ " << '\n';
   LOG_FUNC(__func__);
   LOG_ARG(dollars);
   return Money<T>(dollars);
}

template <typename T>
Money<T> Money<T>::create(const string & dollars, const long double cents) {
   cerr << "\n C++ " ;
   LOG_FUNC(__func__);
   LOG_ARG(dollars);
   LOG_ARG(cents);
   return Money<T>(dollars, cents);
}
