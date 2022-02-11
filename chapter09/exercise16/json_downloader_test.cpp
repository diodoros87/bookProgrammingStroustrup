#include <iostream>
#include <cassert>
#include "json_downloader.hpp"

using namespace std;

namespace test_json_downloader {
   
   inline static bool assert_json(const Json_downloader & j1, const Json_downloader & j2) {
      const Asio_downloader * const a1 = j1.get_asio_downloader();
      const Asio_downloader * const a2 = j2.get_asio_downloader();
      if (a1 == nullptr || a2 == nullptr)
         return false;
      return (a1 != a2) && (a1->get_host() == a2->get_host()) &&
      (a1->get_method() == a2->get_method()) && (a1->get_directory() == a2->get_directory()) &&
      (a1->get_cache_control() == a2->get_cache_control()) && (a1->get_connection() == a2->get_connection())
      && (a1->get_HTTP_VERSION() == a2->get_HTTP_VERSION());
      /*
      assert(a1->get_host() == a2->get_host());
      assert(a1->get_method() == a2->get_method());
      assert(a1->get_directory() == a2->get_directory());
      assert(a1->get_cache_control() == a2->get_cache_control());
      assert(a1->get_connection() == a2->get_connection());
      assert(a1->get_HTTP_VERSION() == a2->get_HTTP_VERSION());
      */
   }

   inline static bool assert_asio(const Asio_downloader & a1, const Asio_downloader & a2) {
      return (a1.get_host() == a2.get_host()) && (a1.get_method() == a2.get_method()) &&
      (a1.get_directory() == a2.get_directory()) && (a1.get_cache_control() == a2.get_cache_control()) && 
      (a1.get_connection() == a2.get_connection()) && (a1.get_HTTP_VERSION() == a2.get_HTTP_VERSION());
      /*
      assert(a1.get_host() == a2.get_host());
      assert(a1.get_method() == a2.get_method());
      assert(a1.get_directory() == a2.get_directory());
      assert(a1.get_cache_control() == a2.get_cache_control());
      assert(a1.get_connection() == a2.get_connection());
      assert(a1.get_HTTP_VERSION() == a2.get_HTTP_VERSION());
      */
   }
   
   static void test() {
      const string & currency = "pln";
      Asio_downloader a1("www.floatrates.com", Method::get, "/daily/" + currency + ".json", Cache_control::no_store, Connection::close);
      Asio_downloader a2 { a1 };
      assert(assert_asio(a1, a2));
      Asio_downloader a3 { std::move(a2) };
      assert(assert_asio(a1, a3));
      //assert_asio(a1, a2);
      a3 = a1;
      assert(assert_asio(a1, a3));
      a3 = std::move(a1);
      assert(assert_asio(a1, a3));
      
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
   
   void test_json_downloader() {
      cerr << '\n' << '\n' << __func__ << '\n';
      test();
   }
}
