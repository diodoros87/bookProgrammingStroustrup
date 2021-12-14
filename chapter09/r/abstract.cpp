#include "print.hpp"
#include "abstract.hpp"

#include <cmath>
#include <cfloat>

using std::cerr;

namespace Hierarchy {
   
const string Abstract::class_name = typeid(person).name();
   
void Abstract::validate(const double x) {
   if (! std::isfinite(x)) 
      throw std::invalid_argument(function + " argument of x: '" + x + "' is not finite");
}

void Abstract::virt_print_X() const {
   cerr << TIE( "C++", unmove(__cplusplus), class_name, __func__, " = ", virt_get_X(), '\n');
}
   
void Abstract::print_area() const {
   cerr << TIE( "C++", unmove(__cplusplus), class_name, __func__, " = ", area(), '\n');
}

double Abstract::area() const {
   return x1;
}

void Abstract::fin_abstract_print() const final {
   cerr << TIE( "C++", unmove(__cplusplus), class_name, __func__, " = ", fin_abstract_number(), '\n');
}

Abstract::Abstract(const double x) {
   validate(x, __func__);
   this->x1 = x;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->x1) << '\n';
}

Abstract::~Abstract() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->x1) << '\n';
}

}

/*  only to use in C++ code in manual dll (shared object) loading  */
extern "C" demo::Demo * abstract_create(const string & name) {
   //if (name == nullptr) 
   //   throw std::invalid_argument(string(__func__) + " argument of name is nullptr");
   using namespace demo;
   try {
      Demo * result = new Demo(name);
      return result;
   }
   catch (const std::invalid_argument & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return nullptr;
   }
}

/*  only to use in C++ code in manual dll (shared object) loading  */
extern "C" void demo_destroy(demo::Demo * & pointer) {
   if (pointer == nullptr) 
      throw std::invalid_argument(string(__func__) + " argument of demo is nullptr");
   
   delete pointer;
   pointer = nullptr;
}
