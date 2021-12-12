#include "demo.hpp"
#include "print.hpp"
#include "utility.hpp"
#include "null.hpp"

#include <cmath>
#include <cfloat>

using std::cerr;

namespace Hierarchy {
   
void Abstract::validate(const double x) {
   if (! std::isfinite(x)) 
      throw std::invalid_argument(function + " argument of x: '" + x + "' is not finite");
}
   
void Abstract::virt_abstract_print() const {
   cerr << TIE( "C++", unmove(__cplusplus), __func__, " Abstract virtual area = ", virt_area(), '\n');
}

double Abstract::virt_area() const {
   return x1;
}

double Abstract::area() const {
   return x1;
}

void Abstract::fin_abstract_print() const final {
   cerr << TIE( "C++", unmove(__cplusplus), __func__, " FINAL Abstract virtual area = ", area(), '\n');
}

Abstract::Abstract(const double x) {
   validate(x, __func__);
   this->x1 = x;
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->x1) << '\n';
}

Abstract::~Abstract() {
   cerr << '\n' << TIE("C++", unmove(__cplusplus), __func__, this->name) << '\n';
}

}

/*  only to use in C++ code in manual dll (shared object) loading  */
extern "C" demo::Demo * demo_create(const string & name) {
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
