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
   string host;
   Method method;
   string directory;
   //string DIRECTORY = "/";            // "/" is root (main page of host) and "" has result 400 Bad Request
   Cache_control cache_control = Cache_control::no_store;
   Connection connection = Connection::close;
public:
   Asio_downloader(const string & HOST, const Method & METHOD, const string & DIRECTORY,
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION) {
      host = HOST;
      method = METHOD;
      directory = DIRECTORY;
      cache_control = CACHE_CONTROL;
      connection = CONNECTION;
   }
   string download() const;
private:
   string get_document() const;
   void process_response_headers(asio::ip::tcp::iostream & socket_iostream, const string& HTTP_VERSION) const;
}
