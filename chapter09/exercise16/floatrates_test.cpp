#include "floatrates_test.hpp"

using namespace std;

namespace float_rates_test {
   
namespace downloaders_test {
   extern void downloaders_test();
}

static string currency = "pln";
static void operations_test() {
   Json_downloader j1("www.floatrates.com", Method::get, "/daily/" + currency + ".json", Cache_control::no_store, Connection::close);
   Json_downloader j2(j1);
   assert(assert_json(j1, j2));
   
   Json_downloader j3("https://github.com", Method::get, "/diodoros87/bookProgrammingStroustrup/blob/master/chapter02/exercise01/compile.sh", Cache_control::no_store, Connection::close);
   Json_downloader j4 = j3;
   assert(assert_json(j3, j4));
   assert( ! assert_json(j3, j2));
   assert( ! assert_json(j3, j1));
   
   j3 = j2;
   assert(assert_json(j3, j1));
   assert(assert_json(j3, j2));
   assert( ! assert_json(j3, j4));
   
   j4 = std::move(j1);
   assert(assert_json(j4, j2));
   assert(assert_json(j4, j3));
   assert(nullptr == j1.get_asio_downloader());
   
   j1 = std::move(j4);
   assert(assert_json(j1, j2));
   assert(assert_json(j1, j3));
   assert(nullptr == j4.get_asio_downloader());
   
   Json_downloader j5 (std::move(Json_downloader("https://github.com", Method::get, "/diodoros87/bookProgrammingStroustrup/blob/master/chapter02/exercise01/compile.sh", Cache_control::no_store, Connection::close)));
   Json_downloader j6 (std::move(j2));
   assert(assert_json(j6, j1));
   assert(assert_json(j6, j3));
   assert(nullptr == j2.get_asio_downloader());
   
   j6 = (std::move(j6));
   assert(assert_json(j6, j1));
   assert(assert_json(j6, j3));
   assert(nullptr != j6.get_asio_downloader());
   
   j6 = j6;
   assert(assert_json(j6, j1));
   assert(assert_json(j6, j3));
   assert(nullptr != j6.get_asio_downloader());
}

void test() {
   Float_rates_test test { File_format::JSON, "PLN" };
   test.run();
   test.set_format(File_format::XML);
   test.run();
   
   Float_rates_test
   
   downloaders_test::downloaders_test();
}

}
