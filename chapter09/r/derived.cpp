#include "print.hpp"
#include "derived.hpp"

#include <cmath>

using std::cerr;
using std::to_string;

namespace Hierarchy {
   
const string Abstract::class_name = typeid(Derived).name();

void Derived::validate(const double number, const string & function) {
   if (! std::isfinite(number) || 0 == number) 
      throw std::invalid_argument(function + " argument of number: '" + to_string(number) + "' is not finite or is 0");
}

void Derived::virt_set_Y(const double n) {
   Abstract::validate(n, __func__);
   this->y = n;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, this->y) << '\n';
}

void Derived::pv_print_Y() const {
   double Y = pv_Y();
   cerr << TIE( "C++", unmove(__cplusplus), class_name, __func__, " = ", Y, unmove('\n'));
}

double Derived::virt_area() const {
   return X() * y;
}

void Derived::virt_print_number() const { 
   double n = number();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, class_name, n) << '\n'; 
};

void Derived::print_number() const { 
   double n = number();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, class_name, n) << '\n'; 
};

void Derived::pv_print_char() const { 
   char c = pv_char();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, class_name, c) << '\n'; 
};

Derived::Derived(const double n1, const double n2) : Abstract(n1) {
   Abstract::validate(n2, __func__);
   this->y = n2;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, unmove(X()), this->y) << '\n';
}

Derived::~Derived() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, unmove(X()), this->y) << '\n';
}

}

extern "C" Hierarchy::Abstract * derived_create(const double n1, const double n2, const double n3) {
   using namespace Hierarchy;
   try {
      Abstract * result = new Derived(n1, n2, n3);
      return result;
   }
   catch (const std::invalid_argument & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return nullptr;
   }
}

extern "C" void derived_destroy(Hierarchy::Abstract * & pointer) {
   if (pointer == nullptr) 
      throw std::invalid_argument(string(__func__) + " argument of derived is nullptr");
   
   delete pointer;
   pointer = nullptr;
}
