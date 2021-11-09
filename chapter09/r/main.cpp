#include "utility.hpp"
#include "print.hpp"
#include "variadic_template.hpp"
#include "demo.hpp"

using namespace demo;

static Demo human = {"Leibniz"};

int main() {
   //<T, long long, long long(const string& , size_t*, int)>
   call(Constructor<int, Money>(), "-20.8", 5.0L);
   call(Constructor<long long, Money>(), "78");
   call(Creation<unsigned long long>(), "2", 5);
   call(Creation<long double>(), "7");
   cerr << '\n';

   cerr << TIE( unmove(__cplusplus), __func__, unmove(human.get_name())) << '\n';
   human.set_name("Spinoza");
   cerr << TIE( unmove(__cplusplus), __func__, unmove(human.get_name())) << '\n';
}
