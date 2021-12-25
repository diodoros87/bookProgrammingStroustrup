#include "derived_test.hpp"
#include "print.hpp"
#include "utility.hpp"
#include "derived.hpp"

#include <functional>

using namespace std::placeholders;
using std::function;
using std::numeric_limits;
using std::bind;
using std::move;
using std::invalid_argument;
using std::is_base_of;
   
namespace tests {
   
using namespace Hierarchy;

void derived_print_and_assert(const Derived_expected & expected, const Derived_real & real) {
   //Abstract_expected & abstract_expected = reinterpret_cast <Abstract_expected &> (const_cast < Derived_expected& >(expected));
   //Abstract_real & abstract_real = reinterpret_cast <Abstract_real &> (const_cast < Derived_real& >(real));
   abstract_print_and_assert(expected, real);
   print_and_assert(real.Z, expected.Z.second, expected.Z.first);
}

#ifdef MANUAL_DLL_LOAD
Derived * Derived_test::construct(const double a, const double b, const double c) {
   double x = a * 2;
   double y = 2 * b;
   double z = c * 2;
   Derived * result = manual_interface.create(x, y, z);
   if (result == nullptr)
      throw invalid_argument(__func__ + string(" result is null"));
   return result;
}
#else
Derived Derived_test::construct(const double a, const double b, const double c) {
   double x = a * 2;
   double y = 2 * b;
   double z = c * 2;
   Derived result (x, y, z);
   return result;
}
#endif

//#ifndef NDEBUG 
static void assert(const Derived & object, const double a, const double b, const double c) {
   double x = object.X();
   double y = object.pv_Y();
   double z = object.Z();
   assert_many(x == a, "object.X() == ", x);
   assert_many(y == b, "object.pv_Y() == ", y);
   assert_many(z == c, "object.Z() == ", z);
}
//#endif

#ifdef MANUAL_DLL_LOAD
Result_codes Derived_test::test_move() {
   //if (d == nullptr)
   //   return INVALID_ARG;
   //manual_interface.destroy(d);
   d = construct(1, 2, 3);
   test_derived(Derived::DERIVED, Derived::DERIVED_CHAR, 2.0, 4.0, 6.0, 48.0, Derived::DERIVED);
   
   Derived object = Derived(11, 12, 13);
   manual_interface.destroy(d);
   //d = reinterpret_cast<Derived *>(operator new(sizeof(Derived)));
   d = &object;
   test_derived(Derived::DERIVED, Derived::DERIVED_CHAR, 11.0, 12.0, 13.0, 11.0 * 12.0 * 13.0, Derived::DERIVED);
   
   Derived other = Derived(7, 6, 9);
   //manual_interface.destroy(unmove(&object));
   d = move(&other);
   test_derived(Derived::DERIVED, Derived::DERIVED_CHAR, 7.0, 6.0, 9.0, 7.0 * 6.0 * 9.0, Derived::DERIVED);
   
   Derived from_move = move(Derived(57, 6, 9));
   //manual_interface.destroy(d);
   d  = move(&from_move);
   assert(from_move, 57, 6, 9);
   test_derived(Derived::DERIVED, Derived::DERIVED_CHAR, 57.0, 6.0, 9.0, 57.0 * 6.0 * 9.0, Derived::DERIVED);
}
#else
Result_codes Derived_test::test_move() {
   d  = construct(1, 2, 3);
   test_derived(Derived::DERIVED, Derived::DERIVED_CHAR, 2.0, 4.0, 6.0, 48.0, Derived::DERIVED);
   
   Derived object = construct(11, 12, 13);
   d  = object;
   test_derived(Derived::DERIVED, Derived::DERIVED_CHAR, 22.0, 24.0, 26.0, 8 * 11.0 * 12.0 * 13.0, Derived::DERIVED);
   
   Derived other = construct(7, 6, 9);
   d  = move(other);
   test_derived(Derived::DERIVED, Derived::DERIVED_CHAR, 14.0, 12.0, 18.0, 8 * 7.0 * 6.0 * 9.0, Derived::DERIVED);
   
   Derived from_move = move(construct(57, 6, 9));
   d  = move(from_move);
   assert(from_move, 0, 0, 0);
   test_derived(Derived::DERIVED, Derived::DERIVED_CHAR, 114.0, 12.0, 18.0, 8 * 57.0 * 6.0 * 9.0, Derived::DERIVED);
}
#endif
   
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
 
void Derived_test::test_interface(int n, char c) {
#ifdef MANUAL_DLL_LOAD
   Interface * inter = d;
   interface_real.set(inter->pv_number(), inter->pv_char());
#else
   Interface & inter = d;
   interface_real.set(inter.pv_number(), inter.pv_char());
#endif
   interface_expected.set(n, c);
   interface_print_and_assert(interface_expected, interface_real);
}

// void Derived_test::test_abstract(const Abstract_expected & expected, const Abstract_real & real) {
//    print_and_assert(real.X, expected.X.second, expected.X.first);
//    print_and_assert(real.pv_Y, expected.pv_Y.second, expected.pv_Y.first);
//    print_and_assert(real.virt_area, expected.virt_area.second, expected.virt_area.first);
//    print_and_assert(real.pv_number, expected.pv_number.second, expected.pv_number.first);
//    print_and_assert(real.pv_char,expected.pv_char.second, expected.pv_char.first);
//    print_and_assert(real.number, expected.number.second, expected.number.first);
// }

void Derived_test::test_abstract(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
#ifdef MANUAL_DLL_LOAD
   Abstract * abs = d;
   abstract_real.set(abs->pv_number(), abs->pv_char(), abs->X(), abs->pv_Y(), abs->virt_area(), abs->number());
#else
   Abstract & abs = d;
   abstract_real.set(abs.pv_number(), abs.pv_char(), abs.X(), abs.pv_Y(), abs.virt_area(), abs.number());
#endif
   abstract_expected.set(pv_n, pv_c, x, pv_y, area, n);
   abstract_print_and_assert(abstract_expected, abstract_real);
}

// void Derived_test::test_base(const Base_expected & expected, const Base_real & real) {
//    print_and_assert(real.X, expected.X.second, expected.X.first);
//    print_and_assert(real.pv_Y, expected.pv_Y.second, expected.pv_Y.first);
//    print_and_assert(real.virt_area, expected.virt_area.second, expected.virt_area.first);
//    print_and_assert(real.pv_number, expected.pv_number.second, expected.pv_number.first);
//    print_and_assert(real.pv_char,expected.pv_char.second, expected.pv_char.first);
//    print_and_assert(real.number, expected.number.second, expected.number.first);
// }

void Derived_test::test_base(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
#ifdef MANUAL_DLL_LOAD
   Base * b = d;
   base_real.set(b->pv_number(), b->pv_char(), b->X(), b->pv_Y(), b->virt_area(), b->number());
#else
   Base & b = d;
   base_real.set(b.pv_number(), b.pv_char(), b.X(), b.pv_Y(), b.virt_area(), b.number());
#endif
   base_expected.set(pv_n, pv_c, x, pv_y, area, n);
   abstract_print_and_assert(base_expected, base_real);
}

void Derived_test::test_base_cutting(int pv_n, char pv_c, double x, double pv_y, double area, int n) {
#ifdef MANUAL_DLL_LOAD
   Base b = *d;
#else   
   Base b = d;
#endif
   base_real.set(b.pv_number(), b.pv_char(), b.X(), b.pv_Y(), b.virt_area(), b.number());
   base_expected.set(pv_n, pv_c, x, pv_y, area, n);
   abstract_print_and_assert(base_expected, base_real);
}

// void Derived_test::test_derived(const Derived_expected & expected, const Derived_real & real) {
//    print_and_assert(real.X, expected.X.second, expected.X.first);
//    print_and_assert(real.pv_Y, expected.pv_Y.second, expected.pv_Y.first);
//    print_and_assert(real.virt_area, expected.virt_area.second, expected.virt_area.first);
//    print_and_assert(real.pv_number, expected.pv_number.second, expected.pv_number.first);
//    print_and_assert(real.pv_char,expected.pv_char.second, expected.pv_char.first);
//    print_and_assert(real.number, expected.number.second, expected.number.first);
//    print_and_assert(real.Z, expected.Z.second, expected.Z.first);
// }

#ifdef MANUAL_DLL_LOAD
void Derived_test::test_derived(int pv_n, char pv_c, double x, double pv_y, double z, double area, int n) {
   derived_real.set(d->pv_number(), d->pv_char(), d->X(), d->pv_Y(), d->virt_area(), d->number(), d->Z());
   derived_expected.set(pv_n, pv_c, x, pv_y, area, n, z);
   derived_print_and_assert(derived_expected, derived_real);
}
#else
void Derived_test::test_derived(int pv_n, char pv_c, double x, double pv_y, double z, double area, int n) {
   derived_real.set(d.pv_number(), d.pv_char(), d.X(), d.pv_Y(), d.virt_area(), d.number(), d.Z());
   derived_expected.set(pv_n, pv_c, x, pv_y, area, n, z);
   derived_print_and_assert(derived_expected, derived_real);
}
#endif

void Derived_test::print_assert() {
   test_interface(Derived::DERIVED, Derived::DERIVED_CHAR);
   test_abstract(Derived::DERIVED, Derived::DERIVED_CHAR, 4.0, 101.0, 4.0 * 101.0 * 7.0, Abstract::ABSTRACT);
   test_base(Derived::DERIVED, Derived::DERIVED_CHAR, 4.0, 101.0, 4.0 * 101.0 * 7.0, Base::BASE);
   test_base_cutting(Base::BASE, Base::BASE_CHAR, 4.0, 101.0, 4.0 * 101.0, Base::BASE);
   test_derived(Derived::DERIVED, Derived::DERIVED_CHAR, 4.0, 101.0, 7.0, 4.0 * 101.0 * 7.0, Derived::DERIVED);
}

Result_codes test_derived_linking(Derived & d) {
   Result_codes result = bind_execute_member_function_assert(d, &Derived::X, 7.5, "x", __func__, &Derived::virt_set_X, 7.5);
   if (OK == result)
      result = incorrect_call(d, &Derived::X, numeric_limits<double>::infinity(), "x", __func__, &Derived::virt_set_X, numeric_limits<double>::infinity());
   if (OK == result) {
      print_and_assert(d.X(), 7.5, "x");
      result = bind_execute_member_function_assert(d, &Derived::pv_Y, 0.8, "y", __func__, &Derived::virt_set_Y, 0.8);
   }
   if (OK == result)
         result = incorrect_call(d, &Derived::pv_Y, -105.8, "y", __func__, &Derived::virt_set_Y, -105.8); 
   if (OK == result) {
      print_and_assert(d.pv_Y(), 0.8, "y");
      result = bind_execute_member_function_assert(d, &Derived::Z, -66.8, "z", __func__, &Derived::virt_set_Z, -66.8);
   }
   if (OK == result)
         result = incorrect_call(d, &Derived::Z, -0.0, "z", __func__, &Derived::virt_set_Z, -0.0); 
   if (OK == result)
         result = incorrect_call(d, &Derived::Z, 190.0, "z", __func__, &Derived::virt_set_Z, 190.0);
   if (OK == result) {
      print_and_assert(d.Z(), -66.8, "z");
      print_and_assert(d.virt_area(), 7.5 * 0.8 * -66.8, "virt_area");
   }
   assert_many(result == OK, "result == ", result);
   return result;
}
/*
static Result_codes test_try_catch() {
   auto test = bind(static_cast<Result_codes(*)()> (&Derived_test::test_derived_linking));
   Result_codes result = call_catch_exception(test);
   if (OK == result) {
      test = bind(static_cast<Result_codes(*)()> (&Derived_test::test_move));
      result = call_catch_exception(test);
   }
   return result; 
}
*/
#ifdef MANUAL_DLL_LOAD
Result_codes Derived_test::test_derived_linking() {
   cerr << "\nMANUAL DLL LOAD\n";
   Result_codes result = execute_function(&Derived_test::test_move);
   if (result == OK) {
      d = test_constructor(manual_interface.create);
      result = (nullptr == d) ? INCORRECT_VALUE : OK;
      if (result == OK) {
         print_assert();
         result = tests::test_derived_linking(*d);
         if (result == OK)
            result = static_cast<Result_codes> (close_handle(&(manual_interface.handle)));
         else
            close_handle(&(manual_interface.handle));
         manual_interface.destroy(d);
      }
      assert_many(d == nullptr, "d pointer == ", d);
   }
   assert_many(result == OK, "result == ", result);
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
      if (OK == result)
         result = execute_function(&Derived_test::test_move);
   }
   assert_many(result == OK, "result == ", result);
   return result;
}
#endif

Result_codes test_derived() {
   function<Result_codes()> test = bind(static_cast<Result_codes(*)()> (&Derived_test::test_derived_linking));
   Result_codes result = call_catch_exception(test);
//    if (OK == result) {
//       test = bind(static_cast<Result_codes(*)()> (&Derived_test::test_move));
//       result = call_catch_exception(test);
//    }
   return result; 
}

}
