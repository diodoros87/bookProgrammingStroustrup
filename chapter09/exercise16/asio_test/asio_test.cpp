//#define ASIO_STANDALONE 
#include "asio.hpp"
//#include "asio/system_error.hpp"
#include <iostream>
#include <sstream>
#include <utility>
#include <string>
#include <map>    
#include <array> 
//#define NDEBUG
#include <cassert>

#include "nlohmann/json.hpp"

using namespace std;

template<class Type>
inline bool is_valid(const int INDEX, const Type& object) {
   constexpr size_t SIZE = tuple_size<Type>{};
   if (INDEX < 0 || INDEX >= SIZE) {
      assert(0 && "Unavailable index ");
      return false;
   }
   return true;
}

enum class Cache_control { no_store, no_cache };
inline ostream& operator<<(ostream& os, const Cache_control& x) {
   static const array<string, 2> AVAILABLE_CACHE_CONTROLS = { "no-store", "no-cache" };
   const int INDEX = static_cast<int>(x);
   if (is_valid(INDEX, AVAILABLE_CACHE_CONTROLS))
      return os << AVAILABLE_CACHE_CONTROLS[INDEX];
   return os << AVAILABLE_CACHE_CONTROLS[0];
}

enum class Connection { keep_alive, close };
inline ostream& operator<<(ostream& os, const Connection& x) {
   static const array<string, 2> AVAILABLE_CONNECTIONS = { "keep-alive", "close" };
   const int INDEX = static_cast<int>(x);
   if (is_valid(INDEX, AVAILABLE_CONNECTIONS))
      return os << AVAILABLE_CONNECTIONS[INDEX];
   return os << AVAILABLE_CONNECTIONS[1];
}

enum class Method { get, options, post };
inline ostream& operator<<(ostream& os, const Method& x) {
   static const array<string, 3> AVAILABLE_METHODS = { "GET", "OPTIONS", "POST" };
   const int INDEX = static_cast<int>(x);
   if (is_valid(INDEX, AVAILABLE_METHODS))
      return os << AVAILABLE_METHODS[INDEX];
   return os << AVAILABLE_METHODS[0];
}

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

int main() {
   try {
      const string CURRENCY = "PLN";
      const string HOST = "www.floatrates.com";
      const Method METHOD = Method::get;
      const string DIRECTORY = "/daily/" + CURRENCY + ".json";
      //const string DIRECTORY = "/";            // "/" is root and "" has result 400 Bad Request
      const Cache_control CACHE_CONTROL = Cache_control::no_store;
      const Connection CONNECTION = Connection::close;
      const string DOC = get_document(HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION);
      
      process_document(CURRENCY, DOC);
      return 0;
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

struct float_rates_info {
   string code;
   double rate;
   double inverse_rate;
};

void from_json(const nlohmann::json& json_data, float_rates_info& info) {
   info.code = json_data.at("code").get<string>();
   info.rate = json_data.at("rate").get<double>();
   info.inverse_rate = json_data.at("inverseRate").get<double>();
}

void modify_document(string & doc);

string get_document(const string & HOST, const Method & METHOD, const string & DIRECTORY,
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION) {
   asio::basic_socket_iostream<asio::ip::tcp> socket_iostream;
   socket_iostream.connect(HOST, "http");
   
   socket_iostream    << METHOD << " " << DIRECTORY << " HTTP/1.1\r\n";
   socket_iostream    << "Host: " + HOST + "\r\n";
   socket_iostream    << "Accept-Charset: utf-8\r\n";
   socket_iostream    << "Cache-Control: " << CACHE_CONTROL << "\r\n";
   socket_iostream    << "Connection: " << CONNECTION  << "\r\n\r\n";
   socket_iostream.flush();
   
   if (socket_iostream.fail())
      throw Asio_IO_Stream_Exception(socket_iostream.error().message());
   stringstream strstream;
   strstream << socket_iostream.rdbuf();
   
   socket_iostream.close();
   if (! socket_iostream)
      throw Asio_IO_Stream_Exception(socket_iostream.error().message());
   string result = strstream.str();
   modify_document(result);
   return result;
}

void process_document(const string & CURRENCY, const string & DOC) {
   stringstream strstream;
   strstream.str(DOC);
   
   nlohmann::json json_data;
   strstream >> json_data;

   map<string, float_rates_info> rates = json_data;
   for (const pair<string, float_rates_info> &p : rates) {
      cout << " 1 " << CURRENCY << " = " << p.second.rate << " " << p.second.code << " and "
           << " 1 " << p.second.code << " = " << p.second.inverse_rate << " " << CURRENCY << endl;
                  
   }
}

void erase(string & result, const char C, const char A) {
   for (unsigned i = 0; i < result.size(); i++) {
      if (C == result[i] || A == result[i]) {
         result.erase(result.begin() + i);
         if (A == result[i]) {
            result.erase(result.begin() + i);
            while (i < result.size() && result[i] != A) 
               result.erase(result.begin() + i);
            if (i < result.size() && A == result[i]) 
               result.erase(result.begin() + i);
         }
      }
   }
}

void modify_document(string & doc) {
   size_t first = doc.find("{");
   doc = doc.substr(first);
   size_t last = doc.rfind("}");
   doc = doc.substr(0, last + 1);
   erase(doc, '\r', '\n');
}
