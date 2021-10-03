//#define ASIO_STANDALONE 
#include "asio.hpp"
#include <iostream>
#include <sstream>
#include <utility>
#include <string>
#include <map>      
#include <array> 

#include "nlohmann/json.hpp"

using namespace std;

enum class Cache_control { no_store, no_cache };
inline ostream& operator<<(ostream& os, const Cache_control& x) {
   static const array<string, 2> AVAILABLE_CACHE_CONTROLS = { "no-store", "no-cache" };
   return os << AVAILABLE_CACHE_CONTROLS[static_cast<int>(x)];
}

enum class Connection { keep_alive, close };
inline ostream& operator<<(ostream& os, const Connection& x) {
   static const array<string, 2> AVAILABLE_CONNECTIONS = { "keep-alive", "close" };
   return os << AVAILABLE_CONNECTIONS[static_cast<int>(x)];
}

void process_json_document(const string & CURRENCY, const string & DOC);
string get_json_document(const string & HOST, const string & DIRECTORY, const Cache_control & CACHE_CONTROL, const Connection & CONNECTION);

int main() {
   try {
      const string CURRENCY = "USD";
      const string HOST = "www.floatrates.com";
      const string DIRECTORY = "/daily/" + CURRENCY + ".json";
      const Connection CONNECTION = Connection::close;
      const Cache_control CACHE_CONTROL = Cache_control::no_store;
      const string DOC = get_json_document(HOST, DIRECTORY, CACHE_CONTROL, CONNECTION);
      
      process_json_document(CURRENCY, DOC);
   }
   catch (const exception & e) {
      cerr << "Exception: " << e.what() << endl;
   }
   catch (...) {
      cerr << "Unrecognized Exception: " <<  endl;
   }
}

struct float_rates_info {
   string code;
   //string alpha_code;    // unused
   //string numeric_code;  // unused
   //string name;          // unused
   double rate;
   //string date;          // unused
   double inverse_rate;
};

void from_json(const nlohmann::json& jdata, float_rates_info& info) {
   info.code = jdata.at("code").get<string>();
   //info.alpha_code = jdata.at("alphaCode").get<string>();
   //info.numeric_code = jdata.at("numericCode").get<string>();
   //info.name = jdata.at("name").get<string>();
   info.rate = jdata.at("rate").get<double>();
   //info.date = jdata.at("date").get<string>();
   info.inverse_rate = jdata.at("inverseRate").get<double>();
}

void prepare_json_document(string & doc);

string get_json_document(const string & HOST, const string & DIRECTORY, const Cache_control & CACHE_CONTROL, const Connection & CONNECTION) {
   asio::ip::tcp::iostream stream;
   stream.connect(HOST, "http");
   stream    << "GET " << DIRECTORY << " HTTP/1.1\r\n";
   stream    << "Host: " + HOST + "\r\n";
   stream    << "Accept: application/json\r\n";
   stream    << "Cache-Control: " << CACHE_CONTROL << "\r\n";
   stream    << "Connection: " << CONNECTION << "\r\n\r\n" << flush;

   ostringstream os;
   os << stream.rdbuf();
   string result = os.str();
   prepare_json_document(result);
   return result;
}

void process_json_document(const string & CURRENCY, const string & DOC) {
   stringstream strstream;
   strstream.str(DOC);
   
   nlohmann::json jdata;
   strstream >> jdata;

   map<string, float_rates_info> rates = jdata;
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

void prepare_json_document(string & doc) {
   size_t first = doc.find("{");
   doc = doc.substr(first);
   size_t last = doc.rfind("}");
   doc = doc.substr(0, last + 1);
   erase(doc, '\r', '\n');
}
