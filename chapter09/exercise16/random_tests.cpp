#include "integer.hpp"
#include "integer_parsing.hpp"
#include "assertions.hpp"
#include "parsing_test.hpp"
#include "random_numbers.hpp"

#include<iostream>
#include<limits>
#include<array>
//#define NDEBUG
#include <cassert>

using namespace std;
using namespace integer_space;

namespace random_tests {
typedef void (* Operation_Ptr)(const long long & , const long long & , const Integer & , const Integer & );

struct Operations {
   static void add(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      long long number_result = N_1 + N_2;
      cerr << showpos << "\n number: " << N_1 << " + " << N_2 << " = " << number_result;
      Integer object_result   = O_1 + O_2;
      cerr << "\n object: " << O_1 << " + " << O_2 << " = " << object_result << '\n';
      assert_number_Integer(number_result, object_result);
   }
   
   static void subtract(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      long long number_result = N_1 - N_2;
      cerr << showpos << "\n number: " << N_1 << " - " << N_2 << " = " << number_result;
      Integer object_result   = O_1 - O_2;
      cerr << "\n object: " << O_1 << " - " << O_2 << " = " << object_result << '\n';
      assert_number_Integer(number_result, object_result);
   }
   
   static void multiply(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      long long number_result = N_1 * N_2;
      cerr << showpos << "\n number: " << N_1 << " * " << N_2 << " = " << number_result;
      Integer object_result   = O_1 * O_2;
      cerr << "\n object: " << O_1 << " * " << O_2 << " = " << object_result << '\n';
      assert_number_Integer(number_result, object_result);
   }
   
   static void divide(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      if (N_2 == 0)
         return;
      long long number_result = N_1 / N_2;
      cerr << showpos << "\n number: " << N_1 << " / " << N_2 << " = " << number_result;
      Integer object_result   = O_1 / O_2;
      cerr << "\n object: " << O_1 << " / " << O_2 << " = " << object_result << '\n';
      assert_number_Integer(number_result, object_result);
   }
   
   static void modulo(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      if (N_2 == 0)
         return;
      long long number_result = N_1 % N_2;
      cerr << showpos << "\n number: " << N_1 << " % " << N_2 << " = " << number_result;
      Integer object_result   = O_1 % O_2;
      cerr << "\n object: " << O_1 << " % " << O_2 << " = " << object_result << '\n';
      assert_number_Integer(number_result, object_result);
   }
   
   static constexpr array<Operation_Ptr, 5> OPERATIONS = { add, subtract, multiply, divide, modulo };
};

constexpr array<Operation_Ptr, 5> Operations::OPERATIONS;

template <typename T>
Integer construct_Integer (const T& NUMBER) { 
   const string STR = to_string(NUMBER);
   Integer i = Integer::parse_create(STR);
   assert_number_Integer(NUMBER, i);
   return i;
}

void test_by_position(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      for (auto operation : Operations::OPERATIONS) {
         operation(N_1, N_2, O_1, O_2);
         operation(-N_1, -N_2, -O_1, -O_2);
         operation(N_1, -N_2, O_1, -O_2);
         operation(-N_1, N_2, -O_1, O_2);
      }
   }
   
inline void validate(const long long NUMBER) {
   if (NUMBER < numeric_limits<int>::min() || NUMBER > numeric_limits<int>::max())
      throw invalid_argument(" number is " + NUMBER);
}

inline void check_unary(const long long & number, const Integer & object) {
   assert_number_Integer(number, object);
   assert_number_Integer(+number, +object);
   assert_number_Integer(-number, -object);
}
   
template <typename T>
class Random_Operations {
private:
   Generator<T> generator;
   bool overflow = false;
public:
   explicit Random_Operations(const Generator<T> & gen) : generator(gen) { }
   
   ~ Random_Operations() = default;
   Random_Operations(const Random_Operations &) = default;
   Random_Operations(Random_Operations &&) = default;
   Random_Operations & operator=(const Random_Operations &) = default;
   Random_Operations & operator=(Random_Operations &&) = default;
   
   const T get_random_number() { return generator(); }
   Generator<T> get_generator() { return generator(); }
   void set_generator(const Generator<T> &  gen) { generator = gen; }
   
//    constexpr bool may_overflow() {
//       constexpr MIN = 
//    }
   
   
   
   void run_one_random (const unsigned long long REPETITIONS) {
      cout << "\n\n ----------- Test run for type: " << typeid(T).name() << '\n';
      T number;
      Integer object;
      static const Integer ZERO = construct_Integer(0);
      assert_number_Integer(0, ZERO);
      assert_number_Integer(-0, construct_Integer(-0));
      assert_number_Integer(+0, construct_Integer(+0));
      for (unsigned long long i = 0; i < REPETITIONS; i++) {
         cerr << "--- REPETITION " << i << '\n';
         number = generator();
         validate(number);
         object = construct_Integer(number);
         check_unary(number, object);
         test_by_position(number, number, object, object);
         test_by_position(number, 0, object, ZERO);
      }
   }
};

void run_two_random (const Generator_Interface * const gen_1, const Generator_Interface * const gen_2, const unsigned long long REPETITIONS) {
   if (gen_1 == nullptr || gen_2 == nullptr )
      throw invalid_argument("Can not accept null pointer");
   cout << "\n\n ----------- Test run for types: " << typeid(*gen_1).name() << " and " << typeid(*gen_2).name() << '\n';
   long long number_T;
   long long number_U;
   Integer object_T;
   Integer object_U;
   for (unsigned long long i = 0; i < REPETITIONS; i++) {
      cerr << "--- REPETITION " << i << '\n';
      number_T = gen_1->operator()('A');
      number_U = gen_2->operator()('A');
      validate(number_T);
      validate(number_U);
      object_T = construct_Integer(number_T);
      object_U = construct_Integer(number_U);
      check_unary(number_T, object_T);
      check_unary(number_U, object_U);
      test_by_position(number_T, number_U, object_T, object_U);
   }
}
/*
template <typename T>
void run_by_one_type(const Generator<T> & gen, const unsigned long long REPETITIONS) {
   Random_Operations<T> operations (gen); 
   operations.run_one_random(REPETITIONS);
   run_two_random(&gen, &gen, REPETITIONS);
}
*/
template <typename T>
void run_by_one_type(const Base_Generator<T> * const gen, const unsigned long long REPETITIONS) {
   Base_Generator<T> * ptr_base = const_cast<Base_Generator<T> *> (gen);
   const Generator<T> * const ptr_generator = dynamic_cast<Generator<T> *> (ptr_base);
   //if (ptr_generator = dynamic_cast<Generator<T> *> (gen)) {
   if (ptr_generator) {
      Random_Operations<T> operations (* ptr_generator); 
      operations.run_one_random(REPETITIONS);
      run_two_random(ptr_generator, ptr_generator, REPETITIONS);
   }
//    Random_Operations<T> operations (gen); 
//    operations.run_one_random(REPETITIONS);
//    run_two_random(&gen, &gen, REPETITIONS);
}

void run_by_one_type(const Generator_set & generator_set, const unsigned long long REPETITIONS) {
   run_by_one_type<long long>(& generator_set.gen_long.GENERATOR, REPETITIONS);
   run_by_one_type<int>(& generator_set.gen_int.GENERATOR, REPETITIONS);
   run_by_one_type<short>(& generator_set.gen_short.GENERATOR, REPETITIONS);
   run_by_one_type<char>(& generator_set.gen_char.GENERATOR, REPETITIONS);
   
   run_by_one_type<long long>(& generator_set.gen_long.GENERATOR_64, REPETITIONS);
   run_by_one_type<int>(& generator_set.gen_int.GENERATOR_64, REPETITIONS);
   run_by_one_type<short>(& generator_set.gen_short.GENERATOR_64, REPETITIONS);
   run_by_one_type<char>(& generator_set.gen_char.GENERATOR_64, REPETITIONS);
} 

void run_by_many_types(const unsigned long long REPETITIONS) {  
   static const Generator_set generator_set;
   const Generator_set * p = & generator_set;
  //static const Generator_set_64 generator_set_64;
//    static const Generator_MINMAX<int> gen_int;
//    static const Generator_MINMAX<long long> gen_long;
//    static const Generator_MINMAX<short> gen_short;
//    static const Generator_MINMAX<char> gen_char;
//    static const Generator_MINMAX_64<int> gen_int;
//    static const Generator_MINMAX<long long> gen_long;
//    static const Generator_MINMAX<short> gen_short;
//    static const Generator_MINMAX<char> gen_char;
   
   static constexpr array<const Generator_Interface*, 8> GENERATORS = {
      & generator_set.gen_int.GENERATOR,
      & generator_set.gen_long.GENERATOR,
      & generator_set.gen_short.GENERATOR,
      & generator_set.gen_char.GENERATOR,
      & generator_set.gen_int.GENERATOR_64,
      & generator_set.gen_long.GENERATOR_64,
      & generator_set.gen_short.GENERATOR_64,
      & generator_set.gen_char.GENERATOR_64
   };
   
   //for (int_fast8_t first = 0; first < GENERATORS.size(); first++) 
   /*
   static constexpr array<const Generator_Interface*, 8> GENERATORS = {
      & Generator_MINMAX<long long>::GENERATOR,
      & Generator_MINMAX<int>::GENERATOR,
      & Generator_MINMAX<short>::GENERATOR,
      & Generator_MINMAX<char>::GENERATOR,
      & Generator_MINMAX<long long>::GENERATOR_64,
      & Generator_MINMAX<int>::GENERATOR_64,
      & Generator_MINMAX<short>::GENERATOR_64,
      & Generator_MINMAX<char>::GENERATOR_64
   };
      */
   for (int_fast8_t first = 0; first < GENERATORS.size() - 1; first++) 
      for (int_fast8_t second = first + 1; second < GENERATORS.size(); second++) 
         run_two_random(GENERATORS[first], GENERATORS[second], REPETITIONS);
      
   random_tests::run_by_one_type(generator_set, REPETITIONS);
}

} // end of namespace random_tests

vector<string> to_vector_string(const int argc, const char * argv[]) {
   vector<string> vec;
   for (int i = 0; i < argc; i++)
      vec.push_back(argv[i]);
   if (argc != vec.size())
      throw runtime_error("argc must be equal to vec.size()");
   return vec;
}

unsigned long long examine_command_line(const int argc, const char * argv[]) {
   long long repetitions = 0;
   switch (argc) {
      case 1:
         return repetitions;
      case 2: {
         string repetitions_string = to_vector_string(argc, argv)[1];
         repetitions = stoull(repetitions_string);
         return repetitions;
      }
      default :
         throw invalid_argument ("Number of command line arguments can be 1 or 2");
   }
}

int main(const int argc, const char * argv[]) {
   try {
      const unsigned long long REPETITIONS = examine_command_line(argc, argv);
      random_tests::run_by_many_types(REPETITIONS);
      return 0;
   }
   catch (const Arithmetic_Error & e) {
      cerr << __func__ << " " << typeid(Arithmetic_Error).name() << " : " << e.what() << endl;
   }
   catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(invalid_argument).name() << " : " << e.what() << endl;
   }
   catch (const runtime_error& e) {
      cerr << __func__ << " " << typeid(runtime_error).name() << " : " << e.what() << endl;
   }
   catch (const exception& e) {
      cerr << __func__ << " " << typeid(exception).name() << " : " << e.what() << endl;
   }
   catch (...) {
      cerr << __func__ << " unrecognized exception\n";
   }
   return 1;
}
