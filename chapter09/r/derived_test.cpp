#include "derived_test.hpp"
#include "print.hpp"
#include "utility.hpp"
#include "derived.hpp"

#include <functional>

using namespace std::placeholders;
using std::function;
using std::numeric_limits;

using std::bind;
   
namespace tests {
   
using namespace Hierarchy;
   
#ifdef MANUAL_DLL_LOAD
static Derived * test_constructor(Derived * (*create)(const double, const double, const double)) {
   Derived * d = create(4, -3, 7); 
   assert_many(d == nullptr, "d pointer == ", d);
   d = create(4, 55, 107); 
   assert_many(d == nullptr, "d pointer == ", d);
   d = create(4, 101, 7); 
   assert_many(d != nullptr, "d pointer == ", d);
   return d;
}
#else
static Result_codes test_constructor(Derived & d) {
   Result_codes result = init<Derived>(d, Constructor<Derived>(), 4, -3, 7); 
   assert_many(result == INVALID_ARG, "result == ", result);
   result = init<Derived>(d, Constructor<Derived>(), 4, 55, 107);
   assert_many(result == INVALID_ARG, "result == ", result);
   result = init<Derived>(d, Constructor<Derived>(), 4, 101, 7);
   assert_many(result == OK, "result == ", result);
   return result;
}
#endif

#ifdef MANUAL_DLL_LOAD
   const Manual_DLL_derived Derived_test::manual_interface("libderived_cpp.so", "derived_create", "derived_destroy");
   Derived * Derived_test::d = nullptr;
#else
   Derived Derived_test::d;
#endif
   
Interface_expected Derived_test::interface_expected;
Abstract_expected Derived_test::abstract_expected;
Base_expected Derived_test::base_expected;
Derived_expected Derived_test::derived_expected;
Interface_real Derived_test::interface_real;
Abstract_real Derived_test::abstract_real;
Base_real Derived_test::base_real;
Derived_real Derived_test::derived_real;

void Derived_test::test_interface(const Interface_expected & expected, const Interface_real & real) {
   print_and_assert(real.pv_number, expected.pv_number.second, expected.pv_number.first);
   print_and_assert(real.pv_char,expected.pv_char.second, expected.pv_char.first);
}
 
void Derived_test::test_interface(int n, char c) {
#ifdef MANUAL_DLL_LOAD
   Interface * inter = d;
   interface_real.set(inter->pv_number(), inter->pv_char());
#else
   Interface & inter = d;
   interface_real.set(inter.pv_number(), inter.pv_char());
#endif
   interface_expected.set(n, c);
   test_interface(interface_expected, interface_real);
}

void Derived_test::test_abstract(const Abstract_expected & expected, const Abstract_real & real) {
   print_and_assert(real.X, expected.X.second, expected.X.first);
   print_and_assert(real.pv_Y, expected.pv_Y.second, expected.pv_Y.first);
   print_and_assert(real.virt_area, expected.virt_area.second, expected.virt_area.first);
   print_and_assert(real.pv_number, expected.pv_number.second, expected.pv_number.first);
   print_and_assert(real.pv_char,expected.pv_char.second, expected.pv_char.first);
   print_and_assert(real.number, expected.number.second, expected.number.first);
}

void Derived_test::test_abstract(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
#ifdef MANUAL_DLL_LOAD
   Abstract * abs = d;
   abstract_real.set(abs->pv_number(), abs->pv_char(), abs->X(), abs->pv_Y(), abs->virt_area(), abs->number());
#else
   Abstract & abs = d;
   abstract_real.set(abs.pv_number(), abs.pv_char(), abs.X(), abs.pv_Y(), abs.virt_area(), abs.number());
#endif
   abstract_expected.set(pv_n, pv_c, x, pv_y, area, n);
   test_interface(abstract_expected, abstract_real);
}

void Derived_test::test_base(const Base_expected & expected, const Base_real & real) {
   print_and_assert(real.X, expected.X.second, expected.X.first);
   print_and_assert(real.pv_Y, expected.pv_Y.second, expected.pv_Y.first);
   print_and_assert(real.virt_area, expected.virt_area.second, expected.virt_area.first);
   print_and_assert(real.pv_number, expected.pv_number.second, expected.pv_number.first);
   print_and_assert(real.pv_char,expected.pv_char.second, expected.pv_char.first);
   print_and_assert(real.number, expected.number.second, expected.number.first);
}

void Derived_test::test_base(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
#ifdef MANUAL_DLL_LOAD
   Base * b = d;
   base_real.set(b->pv_number(), b->pv_char(), b->X(), b->pv_Y(), b->virt_area(), b->number());
#else
   Base & b = d;
   base_real.set(b.pv_number(), b.pv_char(), b.X(), b.pv_Y(), b.virt_area(), b.number());
#endif
   base_expected.set(pv_n, pv_c, x, pv_y, area, n);
   test_base(base_expected, base_real);
}

void Derived_test::test_base_cutting(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
#ifdef MANUAL_DLL_LOAD
   Base b = *d;
#else   
   Base b = d;
#endif
   base_real.set(b.pv_number(), b.pv_char(), b.X(), b.pv_Y(), b.virt_area(), b.number());
   base_expected.set(pv_n, pv_c, x, pv_y, area, n);
   test_base(base_expected, base_real);
}

void Derived_test::test_derived(const Derived_expected & expected, const Derived_real & real) {
   print_and_assert(real.X, expected.X.second, expected.X.first);
   print_and_assert(real.pv_Y, expected.pv_Y.second, expected.pv_Y.first);
   print_and_assert(real.virt_area, expected.virt_area.second, expected.virt_area.first);
   print_and_assert(real.pv_number, expected.pv_number.second, expected.pv_number.first);
   print_and_assert(real.pv_char,expected.pv_char.second, expected.pv_char.first);
   print_and_assert(real.number, expected.number.second, expected.number.first);
   print_and_assert(real.Z, expected.Z.second, expected.Z.first);
}

#ifdef MANUAL_DLL_LOAD
void Derived_test::test_derived(int pv_n, char pv_c, double x, double pv_y, double z, double area, int n) {
   derived_real.set(d->pv_number(), d->pv_char(), d->X(), d->pv_Y(), d->virt_area(), d->number(), d->Z());
   derived_expected.set(pv_n, pv_c, x, pv_y, area, n, z);
   test_derived(derived_expected, derived_real);
}
#else
void Derived_test::test_derived(int pv_n, char pv_c, double x, double pv_y, double z, double area, int n) {
   derived_real.set(d.pv_number(), d.pv_char(), d.X(), d.pv_Y(), d.virt_area(), d.number(), d.Z());
   derived_expected.set(pv_n, pv_c, x, pv_y, area, n, z);
   test_derived(derived_expected, derived_real);
}
#endif

void Derived_test::print_assert() {
   test_interface(Derived::DERIVED, Derived::DERIVED_CHAR);
   test_abstract(Derived::DERIVED, Derived::DERIVED_CHAR, 4.0, 101.0, 4.0 * 101.0, Derived::DERIVED);
   test_base(Derived::DERIVED, Derived::DERIVED_CHAR, 4.0, 101.0, 4.0 * 101.0 * 7.0, Base::BASE);
   test_base_cutting(Base::BASE, Base::BASE_CHAR, 4.0, 101.0, 4.0 * 101.0, Base::BASE);
   test_derived(Derived::DERIVED, Derived::DERIVED_CHAR, 4.0, 101.0, 7.0, 4.0 * 101.0 * 7.0, Derived::DERIVED);
}

Result_codes test_derived_linking(Derived & d) {
   Result_codes result = bind_execute_member_function_assert(d, &Derived::X, 7.5, "x", __func__, &Derived::virt_set_X, 7.5);
   if (OK == result)
      result = incorrect_call(d, &Derived::X, numeric_limits<double>::infinity(), "x", __func__, &Derived::virt_set_X, numeric_limits<double>::infinity());
   if (OK == result) {
      (d.X(), 7.5, "x", __func__);
      result = bind_execute_member_function_assert(d, &Derived::pv_Y, 0.8, "y", __func__, &Derived::virt_set_Y, 0.8);
   }
   if (OK == result)
         result = incorrect_call(d, &Derived::pv_Y, -105.8, "y", __func__, &Derived::virt_set_Y, -105.8); 
   if (OK == result) {
      (d.pv_Y(), 0.8, "y", __func__);
      result = bind_execute_member_function_assert(d, &Derived::Z, -66.8, "z", __func__, &Derived::virt_set_Z, -66.8);
   }
   if (OK == result)
         result = incorrect_call(d, &Derived::Z, -0.0, "z", __func__, &Derived::virt_set_Z, -0.0); 
   if (OK == result)
         result = incorrect_call(d, &Derived::Z, 190.0, "z", __func__, &Derived::virt_set_Z, 190.0);
   if (OK == result) {
      (d.Z(), -66.8, "z", __func__);
      (d.virt_area(), 7.5 * 0.8 * -66.8, "virt_area", __func__);
   }
   assert_many(result == OK, "result == ", result);
   return result;
}

#ifdef MANUAL_DLL_LOAD
Result_codes Derived_test::test_derived_linking() {
   cerr << "\nMANUAL DLL LOAD\n";
   d = test_constructor(manual_interface.create);
   Result_codes result = (nullptr == d) ? INCORRECT_VALUE : OK;
   if (result == OK) {
         print_assert();
         result = tests::test_derived_linking(*d);
      if (result == OK)
         result = static_cast<Result_codes> (close_handle(&(manual_interface.handle)));
      else
         close_handle(&(manual_interface.handle));
      manual_interface.destroy(d);
   }
   assert_many(result == OK, "result == ", result);
   assert_many(d == nullptr, "d pointer == ", d);
   return result;
}
#else
Result_codes Derived_test::test_derived_linking() {
   cerr << "\nAUTOMATIC DLL LOAD\n";
   Result_codes result = test_constructor(d);
   assert_many(result == OK, "result == ", result);
   if (result == OK) {
      print_assert();
      result = tests::test_derived_linking(d);
   }
   assert_many(result == OK, "result == ", result);
   return result;
}
#endif

Result_codes test_derived() {
   function<Result_codes()> test = bind(static_cast<Result_codes(*)()> (&Derived_test::test_derived_linking));
   Result_codes result = call_catch_exception(test);
   return result; 
}

}
