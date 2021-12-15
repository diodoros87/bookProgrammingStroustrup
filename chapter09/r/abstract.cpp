#include "print.hpp"
#include "abstract.hpp"

#include <cmath>
#include <cfloat>

using std::cerr;
using std::to_string;

namespace Hierarchy {
   
const string Abstract::class_name = typeid(Abstract).name();
   
void Abstract::validate(const double number, const string & function) {
   if (! std::isfinite(number)) 
      throw std::invalid_argument(function + " argument of number: '" + to_string(number) + "' is not finite");
}

void Abstract::virt_set_X(const double number) {
   validate(number, __func__);
   this->x = number;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, this->x) << '\n';
}

void Abstract::virt_print_X() const {
   double X = virt_get_X();
   cerr << TIE( "C++", unmove(__cplusplus), class_name, __func__, " = ", X, unmove('\n'));
}
   
void Abstract::virt_print_area() const {
   double area = virt_area();
   cerr << TIE( "C++", unmove(__cplusplus), class_name, __func__, " = ", area, unmove('\n'));
}

double Abstract::virt_area() const {
   return x;
}

void Abstract::print_number() const { 
   double n = number();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, class_name, n) << '\n'; 
};

void Abstract::pv_print_number() const  { 
   int pvn = pv_number();
   cerr << TIE( "C++", unmove(__cplusplus), __func__, class_name, pvn) << '\n'; 
};

Abstract::Abstract(const double number) {
   validate(number, __func__);
   this->x = number;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, this->x) << '\n';
}

Abstract::~Abstract() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), class_name, __func__, this->x) << '\n';
}

}

/*  only to use in C++ code in manual dll (shared object) loading  */
extern "C" Hierarchy::Abstract * abstract_create(const double number) {
   using namespace Hierarchy;
   try {
      Abstract * result = new Abstract(number);
      return result;
   }
   catch (const std::invalid_argument & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return nullptr;
   }
}

/*  only to use in C++ code in manual dll (shared object) loading  */
extern "C" void demo_destroy(Hierarchy::Abstract * & pointer) {
   if (pointer == nullptr) 
      throw std::invalid_argument(string(__func__) + " argument of demo is nullptr");
   
   delete pointer;
   pointer = nullptr;
}
