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

#include "nlohmann/json.hpp"

using namespace std;

template<class Type>
inline bool is_valid(const int INDEX, const Type& object) {
   constexpr size_t SIZE = std::tuple_size<Type>{};
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

enum class Method { get, head, options };
inline ostream& operator<<(ostream& os, const Method& x) {
   static const array<string, 3> AVAILABLE_METHODS = { "GET", "HEAD", "OPTIONS" };
   const int INDEX = static_cast<int>(x);
   if (is_valid(INDEX, AVAILABLE_METHODS))
      return os << AVAILABLE_METHODS[INDEX];
   return os << AVAILABLE_METHODS[0];
}

void process_document(const string & CURRENCY, const string & DOC);
string get_document(const string & HOST, const Method & METHOD, const string & DIRECTORY, 
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION);

class Asio_Exception : public exception { 
   string msg {"!!! Asio Exception: "};
public:
   Asio_Exception() {}
   Asio_Exception(const string& message) { msg += message; }
   const char* what() {
      return msg.c_str();
   }
};

int main() {
   try {
      const string CURRENCY = "PLN";
      const string HOST = "www.floatrates.com";
      const Method METHOD = Method::get;
      const string DIRECTORY = "/daily/" + CURRENCY + ".json";
      const Cache_control CACHE_CONTROL = Cache_control::no_store;
      const Connection CONNECTION = Connection::close;
      const string DOC = get_document(HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION);
      
      process_document(CURRENCY, DOC);
   }
   catch (Asio_Exception & e) {
      cerr << e.what() << endl;
   }
   catch (exception & e) {
      cerr << "Exception: " << e.what() << endl;
   }
   catch (...) {
      cerr << "Unrecognized Exception: " <<  endl;
   }
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
   asio::ip::tcp::iostream stream;
   stream.connect(HOST, "http");
   stream    << METHOD << " " << DIRECTORY << " HTTP/1.1\r\n";
   stream    << "Host: " + HOST + "\r\n";
   stream    << "Accept-Charset: utf-8\r\n";
   stream    << "Cache-Control: " << CACHE_CONTROL << "\r\n";
   stream    << "Connection: " << CONNECTION  << "\r\n\r\n";
   if (! stream)
      throw Asio_Exception(stream.error().message());
   ostringstream os;
   os << stream.rdbuf();
   stream.close();
   string result = os.str();
   //cout << result;
   //cout << "EEEEEEEEEEEEEEEEEEEE\n";
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
