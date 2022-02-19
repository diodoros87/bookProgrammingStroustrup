#ifndef ASIO_DOWNLOADER_HPP
#define ASIO_DOWNLOADER_HPP

#define ASIO_STANDALONE 
#include "asio.hpp"
#include <iostream>


//#include <sstream>
//#include <utility>
#include <string>
//#include <map>    
//#include <array> 
//#define NDEBUG
//#include <cassert>
//#include <tuple>
//#include <chrono>

#include "network.hpp"

using std::string;
using std::exception;


using network::Method;
using network::Cache_control;
using network::Connection;

class Asio_IO_Stream_Exception : public std::exception { 
   string msg {"!!! error on the socket stream: "};
public:
   Asio_IO_Stream_Exception() {}
   Asio_IO_Stream_Exception(const string& message) { msg += message; }
   const char* what() const noexcept {
      return msg.c_str();
   }
};

class Asio_downloader {
   //static const string HTTP_VERSION;
   const string HTTP_VERSION = {"HTTP/1.1"};
   string host;
   Method method;
   string directory;
   //string DIRECTORY = "/";            // "/" is root (main page of host) and "" has result 400 Bad Request
   Cache_control cache_control = Cache_control::no_store;
   Connection connection = Connection::close;
public:
   Asio_downloader(const string & HOST, const Method & METHOD, const string & DIRECTORY,
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION) {
      assign_non_constans(*this, HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION);
   }
   string download() const;
   
   string get_host() const { return host; };
   Method get_method() const { return method; };
   string get_directory() const { return directory; };
   Cache_control get_cache_control() const { return cache_control; };
   Connection get_connection() const { return connection; };
   string get_HTTP_VERSION() const { return HTTP_VERSION; };
   
   ~ Asio_downloader() = default;
   Asio_downloader(const Asio_downloader &) = default;
   Asio_downloader(Asio_downloader &&) = default;
   
   // copy assignment operator of 'Asio_downloader' is implicitly deleted because field 'HTTP_VERSION' has no copy assignment operator
   Asio_downloader & operator=(const Asio_downloader & other) {
      assign_non_constans(*this, other.host, other.method, other.directory, other.cache_control, other.connection);
      return *this;
   }
   Asio_downloader & operator=(Asio_downloader &&) = default;
   
   bool operator==(const Asio_downloader & other) const {
      return get_host() == other.get_host() && get_method() == other.get_method()
         && get_directory() == other.get_directory() && get_cache_control() == other.get_cache_control() 
         && get_connection() == other.get_connection() && get_HTTP_VERSION() == other.get_HTTP_VERSION(); }
   
   bool operator!=(const Asio_downloader & other) const {
      return ! operator==(other); }
private:
   string get_document() const;
   void process_response_headers(asio::ip::tcp::iostream & socket_iostream) const;
   
   static void assign_non_constans(Asio_downloader & object, const string & HOST, const Method & METHOD, const string & DIRECTORY,
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION) {
      object.host = HOST;
      object.method = METHOD;
      object.directory = DIRECTORY;
      object.cache_control = CACHE_CONTROL;
      object.connection = CONNECTION;
   }
};

#endif
