#include "print.hpp"
#include "base.hpp"

#include <cmath>

using std::cerr;
using std::to_string;

namespace Hierarchy {
   
const string Base::class_name = typeid(Base).name();

void Base::virt_set_Y(const double n) {
   Abstract::validate(n, __func__);
   this->y = n;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, this->y) << '\n';
}

void Base::pv_print_Y() const {
   double Y = pv_Y();
   cerr << TIE( "C++", unmove(__cplusplus), class_name, __func__, " = ", Y, unmove('\n'));
}

double Base::virt_area() const {
   return X() * y;
}

void Base::virt_print_number() const { 
   double n = number();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, class_name, n) << '\n'; 
};

void Base::print_number() const { 
   double n = number();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, class_name, n) << '\n'; 
};

void Base::pv_print_char() const { 
   char c = pv_char();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, class_name, c) << '\n'; 
};

Base::Base(const double n1, const double n2) : Abstract(n1) {
   Abstract::validate(n2, __func__);
   this->y = n2;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, unmove(X()), this->y) << '\n';
}

Base::~Base() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, unmove(X()), this->y) << '\n';
}

}

extern "C" Hierarchy::Interface * base_create(const double n1, const double n2) {
   using namespace Hierarchy;
   try {
      Interface * result = new Base(n1, n2);
      return result;
   }
   catch (const std::invalid_argument & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return nullptr;
   }
}

extern "C" void base_destroy(Hierarchy::Interface * & pointer) {
   if (pointer == nullptr) 
      throw std::invalid_argument(string(__func__) + " argument of base is nullptr");
   
   delete pointer;
   pointer = nullptr;
}
