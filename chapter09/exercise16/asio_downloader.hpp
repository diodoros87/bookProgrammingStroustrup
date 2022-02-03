//#define ASIO_STANDALONE 
#include "asio.hpp"
#include <iostream>
#include <sstream>
#include <utility>
#include <string>
#include <map>    
#include <array> 
//#define NDEBUG
#include <cassert>
#include <tuple>
#include <chrono>

void process_document(const string & CURRENCY, const string & DOC);
string get_document(const string & HOST, const Method & METHOD, const string & DIRECTORY, 
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION);

class Asio_IO_Stream_Exception : public exception { 
   string msg {"!!! error on the socket stream: "};
public:
   Asio_IO_Stream_Exception() {}
   Asio_IO_Stream_Exception(const string& message) { msg += message; }
   const char* what() const noexcept {
      return msg.c_str();
   }
};

class Asio_downloader {
   try {
      const string CURRENCY = "PLN";
      const string HOST = "www.floatrates.com";
      const Method METHOD = Method::get;
      const string DIRECTORY = "/daily/" + CURRENCY + ".json";
      //const string DIRECTORY = "/";            // "/" is root (main page of host) and "" has result 400 Bad Request
      const Cache_control CACHE_CONTROL = Cache_control::no_store;
      const Connection CONNECTION = Connection::close;
   }
   catch (const nlohmann::json::exception & e) {
      cerr << "!!! Error json exception: " << e.what() << '\n';
   }
   catch (const Asio_IO_Stream_Exception & e) {
      cerr << e.what() << endl;
   }
   catch (const asio::system_error &e) {
      cerr << "!!! System Error ! Error code = " << e.code()
           << "\n Message: " << e.what();
      return e.code().value();
   }
   catch (const exception & e) {
      cerr << "Exception: " << e.what() << endl;
   }
   catch (...) {
      cerr << "Unrecognized Exception: " <<  endl;
   }
   return 1;
}
