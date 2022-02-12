#include <iostream>
#include <cstdlib>
//#include <gmp.h>
#include <gmpxx.h>

#include "assertions.hpp"
#include "command_line.hpp"

#include "integer.hpp"

using namespace std;
using namespace integer_space;

#ifndef NDEBUG
#  define assert_mpz_Integer(Mpz, Integer) \
   ASSERT_MPZ_INTEGER(Mpz, Integer, __FILE__, __LINE__, __DATE__, __TIME__)
#else
#   define assert_mpz_Integer(Mpz, Integer) ;
#endif
   
void ASSERT_MPZ_INTEGER(const mpz_class & NUMBER, const Integer & OBJECT,
                     const char* file, const int line,
                     const char* date, const char* time) {
   ostringstream stream;
   if (0 != NUMBER)
      stream.setf(ios_base::showpos);
   stream << NUMBER;
   const string number_string = stream.str();
   const string object_string = string(OBJECT);
   if (number_string != object_string) {
      const string message = "\n number = " + number_string + "\n object = " + object_string;
      GENERAL_ASSERT(object_string.c_str(), number_string == object_string, file, line, date, time, message);
   }
} 

inline void print(const string & func_name, const exception& e) {
   cerr << func_name << " " << typeid(e).name() << " : " << e.what() << endl;
}

enum class Arithmetic { ADD, SUBTRACT, MULTIPLY, DIVIDE };

typedef void (* Operation_Ptr)(const mpz_class & , const mpz_class & , const Integer & , const Integer & );

struct Operations {
   static void add(const mpz_class & N_1, const mpz_class & N_2, const Integer & O_1, const Integer & O_2) {
      mpz_class number_result = N_1 + N_2;
      cerr << showpos << "\n number: " << N_1 << " + " << N_2 << " = " << number_result;
      Integer object_result   = O_1 + O_2;
      cerr << "\n object: " << O_1 << " + " << O_2 << " = " << object_result << '\n';
      assert_mpz_Integer(number_result, object_result);
   }
   
   static void subtract(const mpz_class & N_1, const mpz_class & N_2, const Integer & O_1, const Integer & O_2) {
      mpz_class number_result = N_1 - N_2;
      cerr << showpos << "\n number: " << N_1 << " - " << N_2 << " = " << number_result;
      Integer object_result   = O_1 - O_2;
      cerr << "\n object: " << O_1 << " - " << O_2 << " = " << object_result << '\n';
      assert_mpz_Integer(number_result, object_result);
   }
   
   static void multiply(const mpz_class & N_1, const mpz_class & N_2, const Integer & O_1, const Integer & O_2) {
      mpz_class number_result = N_1 * N_2;
      cerr << showpos << "\n number: " << N_1 << " * " << N_2 << " = " << number_result;
      Integer object_result   = O_1 * O_2;
      cerr << "\n object: " << O_1 << " * " << O_2 << " = " << object_result << '\n';
      assert_mpz_Integer(number_result, object_result);
   }
   
   static void divide(const mpz_class & N_1, const mpz_class & N_2, const Integer & O_1, const Integer & O_2) {
      if (N_2 == 0)
         return;
      mpz_class number_result = N_1 / N_2;
      cerr << showpos << "\n number: " << N_1 << " / " << N_2 << " = " << number_result;
      Integer object_result   = O_1 / O_2;
      cerr << "\n object: " << O_1 << " / " << O_2 << " = " << object_result << '\n';
      assert_mpz_Integer(number_result, object_result);
   }
   
   static void modulo(const mpz_class & N_1, const mpz_class & N_2, const Integer & O_1, const Integer & O_2) {
      if (N_2 == 0)
         return;
      mpz_class number_result = N_1 % N_2;
      cerr << showpos << "\n number: " << N_1 << " % " << N_2 << " = " << number_result;
      Integer object_result   = O_1 % O_2;
      cerr << "\n object: " << O_1 << " % " << O_2 << " = " << object_result << '\n';
      assert_mpz_Integer(number_result, object_result);
   }
   
   static void compare(const mpz_class & N_1, const mpz_class & N_2, const Integer & O_1, const Integer & O_2) {
      assert(O_1.is_zero() == (N_1 == 0));
      
      assert((O_1 == O_2) == (N_1 == N_2));
      assert((O_1 != O_2) == (N_1 != N_2));

      assert((O_1 > O_2) == (N_1 > N_2));
      assert((O_1 <= O_2) == (N_1 <= N_2));

      assert((O_1 <  O_2) == (N_1 < N_2));
      assert((O_1 >=  O_2) == (N_1 >= N_2));
   }
   
   static constexpr array<Operation_Ptr, 6> OPERATIONS = { add, subtract, multiply, divide, modulo, compare };
};

constexpr array<Operation_Ptr, 6> Operations::OPERATIONS;

void test_by_position(const mpz_class & N_1, const mpz_class & N_2, const Integer & O_1, const Integer & O_2) {
   for (auto operation : Operations::OPERATIONS) {
      operation(N_1, N_2, O_1, O_2);
      operation(-N_1, -N_2, -O_1, -O_2);
      operation(N_1, -N_2, O_1, -O_2);
      operation(-N_1, N_2, -O_1, O_2);
   }
}

inline void check_unary(const mpz_class & number, const Integer & object) {
   assert_mpz_Integer(number, object);
   assert_mpz_Integer(+number, +object);
   assert_mpz_Integer(-number, -object);
}

namespace gmp_random_tests {
   
class Gmp_test {
   const unsigned long long REPETITIONS;
   vector<mpz_class> rand_mpzs;
   vector<Integer> rand_integers;
   
   mpz_class random_mpz(const Arithmetic & arithmetic, gmp_randstate_t & state, const int operand);
   int upper_exponent_2(const Arithmetic & arithmetic, const int operand);
   
   Integer get_integer(const mpz_class & mpz) {
      string s = mpz.get_str();
      Integer result = Integer::parse_create(s);
      //cout << __func__ << " " << result << '\n';
      return result;
   }
   
   void generate(const Arithmetic & arithmetic, const int ELEMENTS);
   void operate(const Arithmetic & arithmetic) {
      generate(arithmetic, REPETITIONS);
      operate();
      rand_mpzs.clear();
      rand_integers.clear();
   }
   void operate ();
public:
   Gmp_test(const unsigned long long repetitions) : REPETITIONS(repetitions) {  
      if (repetitions % 2 == 1)
         throw invalid_argument("repetitions must be even number - is: " + to_string(repetitions));
   }
   void run();
};

int Gmp_test::upper_exponent_2(const Arithmetic & arithmetic, const int operand) {
   switch (arithmetic) {
      case Arithmetic::ADD:
      case Arithmetic::SUBTRACT:
         return 128 ;
      case Arithmetic::MULTIPLY:
         return 64 ;
      case Arithmetic::DIVIDE:
         switch (operand) {
            case 1 :
               return 128 ;
            case 2 :
               return rand() % 32 ;
            default:
               throw invalid_argument("operand must be 1 or 2 - is: " + to_string(operand));
         }
         
   }
}

mpz_class Gmp_test::random_mpz(const Arithmetic & arithmetic, gmp_randstate_t & state, const int operand) {
   mpz_class signum, result;
   mpz_urandomb(signum.get_mpz_t(), state, 1);
   mpz_urandomb(result.get_mpz_t(), state, upper_exponent_2(arithmetic, operand));
   if (signum < 0.5)
      result *= -1;
   //gmp_printf("%Zd --------\n", result);
   return result;
}

void Gmp_test::operate () {
   const size_t SIZE = rand_mpzs.size();
   if (SIZE != rand_integers.size())
      throw runtime_error("rand_mpzs.size() != rand_integers.size() ");
   if (SIZE % 2 == 1)
      throw runtime_error(" size of rand_mpzs must be even number - is: " + to_string(SIZE));
   mpz_class number_T;
   mpz_class number_U;
   Integer object_T;
   Integer object_U;
   for (unsigned long long i = 0; i < SIZE ; i += 2) {
      cerr << "--- REPETITION " << i << '\n';
      number_T = rand_mpzs[i];
      number_U = rand_mpzs[i + 1];
      object_T = rand_integers[i];
      object_U = rand_integers[i + 1];
      check_unary(number_T, object_T);
      check_unary(number_U, object_U);
      test_by_position(number_T, number_U, object_T, object_U);
   }
}
   
void Gmp_test::generate(const Arithmetic & arithmetic, const int ELEMENTS)  { 
   //if (ELEMENTS % 2 == 1)
   //   throw invalid_argument("ELEMENTS must be even number - is: " + ELEMENTS);
   //const int first = upper_exponent_2(arithmetic, 1);
   vector<mpz_class> vec_mpzs(ELEMENTS);
   vector<Integer> vec_integers(ELEMENTS);
   mpz_class random;
   cout << __func__ << '\n';
   gmp_randstate_t state;
   gmp_randinit_mt(state);
   for(int i = 0 ; i < ELEMENTS;){
      random = random_mpz(arithmetic, state, 1);
      vec_mpzs[i] = random;
      vec_integers[i++] = get_integer(random);
      if (Arithmetic::DIVIDE == arithmetic) {
         random = random_mpz(arithmetic, state, 2);
         vec_mpzs[i] = random;
         vec_integers[i++] = get_integer(random);
      }
   }
   rand_mpzs = std::move(vec_mpzs);
   rand_integers = std::move(vec_integers);
}
   
void Gmp_test::run () {
   operate(Arithmetic::ADD);
   operate(Arithmetic::SUBTRACT);
   operate(Arithmetic::MULTIPLY);
   operate(Arithmetic::DIVIDE);
}

}

int main(const int argc, const char * argv[]) {
   try {
      const unsigned long long REPETITIONS = command_line::examine_command_line(argc, argv);
      gmp_random_tests::Gmp_test test(REPETITIONS);
      test.run();
      
      cerr << "\n END of ALL TESTS\n";
      return 0;
   }
   catch (const Arithmetic_Error & e) {
      print(__func__, e);
   }
   catch (const out_of_range & e) {
      print(__func__, e);
   }
   catch (const invalid_argument& e) {
      print(__func__, e);
   }
   catch (const runtime_error& e) {
      print(__func__, e);
   }
   catch (const exception& e) {
      print(__func__, e);
   }
   catch (...) {
      cerr << __func__ << " unrecognized exception\n";
   }
   return 1;
}
