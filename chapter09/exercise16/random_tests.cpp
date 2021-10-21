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

#define MIN_LL numeric_limits<long long>::min()
#define MAX_LL numeric_limits<long long>::max()

using namespace std;
using namespace integer_space;

namespace random_tests {
typedef void (* Operation_Ptr)(const long long & , const long long & , const Integer & , const Integer & );

enum class Arithmetic { ADD, SUBTRACT, MULTIPLY, DIVIDE };

bool is_overflow(const long long & N_1, const long long & N_2, const Arithmetic & arithmetic) {
   switch (arithmetic) {
      case Arithmetic::ADD:
         return N_1 > 0 && N_2 > 0 && MAX_LL - N_1 < N_2   
            ||  N_1 < 0 && N_2 < 0 && MIN_LL - N_1 > N_2 ;
      case Arithmetic::SUBTRACT:
         return N_1 < 0 && N_2 > 0 && MIN_LL + N_2 > N_1   
            ||  N_1 > 0 && N_2 < 0 && MAX_LL + N_2 < N_1 
            ||  N_1 == 0 && N_2 == MIN_LL ;
      case Arithmetic::MULTIPLY:
         return N_1 > +1 && N_2 > +1 && MAX_LL / N_2 < N_1   
            ||  N_1 < -1 && N_2 < -1 && MAX_LL / N_2 > N_1 
            ||  N_1 > +1 && N_2 < -1 && MIN_LL / N_2 < N_1   
            ||  N_1 < -1 && N_2 > +1 && MIN_LL / N_2 > N_1 
            ||  N_1 == -1 && N_2 == MIN_LL 
            ||  N_1 == MIN_LL && N_2 == -1 ;
      case Arithmetic::DIVIDE:
         return N_1 == MIN_LL && N_2 == -1 ;
   }
}

struct Operations {
   static void add(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      if (is_overflow(N_1, N_2, Arithmetic::ADD))
         return;
      long long number_result = N_1 + N_2;
      cerr << showpos << "\n number: " << N_1 << " + " << N_2 << " = " << number_result;
      Integer object_result   = O_1 + O_2;
      cerr << "\n object: " << O_1 << " + " << O_2 << " = " << object_result << '\n';
      assert_number_Integer(number_result, object_result);
   }
   
   static void subtract(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      if (is_overflow(N_1, N_2, Arithmetic::SUBTRACT))
         return;
      long long number_result = N_1 - N_2;
      cerr << showpos << "\n number: " << N_1 << " - " << N_2 << " = " << number_result;
      Integer object_result   = O_1 - O_2;
      cerr << "\n object: " << O_1 << " - " << O_2 << " = " << object_result << '\n';
      assert_number_Integer(number_result, object_result);
   }
   
   static void multiply(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      if (is_overflow(N_1, N_2, Arithmetic::MULTIPLY))
         return;
      long long number_result = N_1 * N_2;
      cerr << showpos << "\n number: " << N_1 << " * " << N_2 << " = " << number_result;
      Integer object_result   = O_1 * O_2;
      cerr << "\n object: " << O_1 << " * " << O_2 << " = " << object_result << '\n';
      assert_number_Integer(number_result, object_result);
   }
   
   static void divide(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      if (N_2 == 0)
         return;
      if (is_overflow(N_1, N_2, Arithmetic::DIVIDE))
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
   static_assert(is_integral<T>::value && "Integral required.");
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

inline void check_unary(const long long & number, const Integer & object) {
   assert_number_Integer(number, object);
   assert_number_Integer(+number, +object);
   if (number != numeric_limits<long long>::min()) // llabs(numeric_limits<long long>::min()) - 1 == llabs(numeric_limits<long long>::max())
      assert_number_Integer(-number, -object);
}
   
template <typename Number, template<typename> class Generator_Type>
class Random_Operations {
   static_assert(is_integral<Number>::value && "Integral required.");
private:
   Generator_Type<Number> generator;
   bool overflow = false;
public:
   explicit Random_Operations(const Generator_Type<Number> & gen) : generator(gen) { }
   
   ~ Random_Operations() = default;
   Random_Operations(const Random_Operations &) = default;
   Random_Operations(Random_Operations &&) = default;
   Random_Operations & operator=(const Random_Operations &) = default;
   Random_Operations & operator=(Random_Operations &&) = default;
   
   const Number get_random_number() { return generator(); }
   Generator_Type<Number> get_generator() { return generator(); }
   void set_generator(const Generator_Type<Number> &  gen) { generator = gen; }
   
   void run_one_random (const unsigned long long REPETITIONS) {
      cout << "\n\n ----------- Test run for integer type: " << typeid(Number).name() 
           << " and generator type: " << typeid(Generator_Type<Number>).name() << '\n';
      Number number;
      Integer object;
      static const Integer ZERO = construct_Integer(0);
      assert_number_Integer(0, ZERO);
      assert_number_Integer(-0, construct_Integer(-0));
      assert_number_Integer(+0, construct_Integer(+0));
      for (unsigned long long i = 0; i < REPETITIONS; i++) {
         cerr << "--- REPETITION " << i << '\n';
         number = generator();
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
      object_T = construct_Integer(number_T);
      object_U = construct_Integer(number_U);
      check_unary(number_T, object_T);
      check_unary(number_U, object_U);
      test_by_position(number_T, number_U, object_T, object_U);
   }
}

template <typename Number, template<typename> class Generator_Type>
void random_operations(const Generator_Type<Number> & gen, const unsigned long long REPETITIONS) {
   Random_Operations<Number, Generator_Type> operations (gen); 
   operations.run_one_random(REPETITIONS);
   run_two_random(&gen, &gen, REPETITIONS);
}

template <typename Number>
void run_by_one_type(const Base_Generator<Number> * const gen, const unsigned long long REPETITIONS) {
   Base_Generator<Number> * const ptr_base = const_cast<Base_Generator<Number> *> (gen);
   
   if (const Generator<Number> * const ptr_generator = dynamic_cast< Generator<Number> *> (ptr_base))
      random_operations(*ptr_generator, REPETITIONS);
   else if (const Generator_64<Number> * const ptr_generator = dynamic_cast<Generator_64<Number> *> (ptr_base))
      random_operations(*ptr_generator, REPETITIONS);
   else
      throw invalid_argument("Pointer to Base_Generator<Number> is unacceptable - pointer value: "
         + to_string(reinterpret_cast<unsigned long long> (gen)));
      
   cerr << hex << "Pointer to Base_Generator<Number> pointer value: "
         << reinterpret_cast<unsigned long long> (gen) << '\n';
   cerr << dec;
}

void run_by_one_type(const unsigned long long REPETITIONS) {
   run_by_one_type<long long>(& Generator_set::generators.gen_long.GENERATOR, REPETITIONS);
   run_by_one_type<int>(& Generator_set::generators.gen_int.GENERATOR, REPETITIONS);
   run_by_one_type<short>(& Generator_set::generators.gen_short.GENERATOR, REPETITIONS);
   run_by_one_type<char>(& Generator_set::generators.gen_char.GENERATOR, REPETITIONS);
   
   run_by_one_type<int>(& Generator_set::generators.gen_int.GENERATOR_64, REPETITIONS);
   run_by_one_type<long long>(& Generator_set::generators.gen_long.GENERATOR_64, REPETITIONS);
   run_by_one_type<short>(& Generator_set::generators.gen_short.GENERATOR_64, REPETITIONS);
   run_by_one_type<char>(& Generator_set::generators.gen_char.GENERATOR_64, REPETITIONS);
} 

void run_different_types(const unsigned long long REPETITIONS) {  
   static constexpr array<const Generator_Interface*, 8> GENERATORS = {
      & Generator_set::generators.gen_int.GENERATOR,
      & Generator_set::generators.gen_long.GENERATOR,
      & Generator_set::generators.gen_short.GENERATOR,
      & Generator_set::generators.gen_char.GENERATOR,
      
      & Generator_set::generators.gen_int.GENERATOR_64,
      & Generator_set::generators.gen_long.GENERATOR_64,
      & Generator_set::generators.gen_short.GENERATOR_64,
      & Generator_set::generators.gen_char.GENERATOR_64
   };
     /*
   static constexpr array<const Generator_Interface*, 8> GENERATORS = {
      & Generator_set::generators->gen_int.GENERATOR,
      & Generator_set::generators->gen_long.GENERATOR,
      & Generator_set::generators->gen_short.GENERATOR,
      & Generator_set::generators->gen_char.GENERATOR,
      & Generator_set::generators->gen_int.GENERATOR_64,
      & Generator_set::generators->gen_long.GENERATOR_64,
      & Generator_set::generators->gen_short.GENERATOR_64,
      & Generator_set::generators->gen_char.GENERATOR_64
   }; */

   for (int_fast8_t first = 0; first < GENERATORS.size() - 1; first++) 
      for (int_fast8_t second = first + 1; second < GENERATORS.size(); second++) 
         run_two_random(GENERATORS[first], GENERATORS[second], REPETITIONS);
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
   long long repetitions = 100;
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
      random_tests::run_by_one_type(REPETITIONS);
      random_tests::run_different_types(REPETITIONS);
      
      cerr << " Generator_set::generators.gen_int.GENERATOR.min = " << Generator_set::generators.gen_int.GENERATOR_64.min << '\n';
      cerr << " Generator_set::generators.gen_int.GENERATOR.max = " << Generator_set::generators.gen_int.GENERATOR_64.max << '\n';
      cerr << " Generator_set::generators.gen_long.GENERATOR.min = " << Generator_set::generators.gen_long.GENERATOR_64.min << '\n';
      cerr << " Generator_set::generators.gen_long.GENERATOR.max = " << Generator_set::generators.gen_long.GENERATOR_64.max << '\n';
      cerr << "mt19937_64.min = " << mt19937_64::min();
      cerr << "\nmt19937_64.max = " << mt19937_64::max();
      cerr << "\nmt19937.min = " << mt19937::min();
      cerr << "\nmt19937.max = " << mt19937::max();
      cerr << '\n';
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
