//#define ASIO_STANDALONE 
#include "asio.hpp"
#include <iostream>
#include <sstream>
#include <utility>
#include <string>
#include <map>

#include "nlohmann/json.hpp"

using namespace std;

struct rates_info {
   string code;
   //string alpha_code;
   //string numeric_code;
   //string name;
   double rate;
   //string date;
   double inverse_rate;
};

void from_json(const nlohmann::json& jdata, rates_info& info) {
   info.code = jdata.at("code").get<string>();
   //info.alpha_code = jdata.at("alphaCode").get<string>();
   //info.numeric_code = jdata.at("numericCode").get<string>();
   //info.name = jdata.at("name").get<string>();
   info.rate = jdata.at("rate").get<double>();
   //info.date = jdata.at("date").get<string>();
   info.inverse_rate = jdata.at("inverseRate").get<double>();
}

void erase(string & result, char c, char a) {
   for (unsigned i = 0; i < result.size(); i++) {
      if (c == result[i] || a == result[i]) {
         result.erase(result.begin() + i);
         if (a == result[i]) {
            result.erase(result.begin() + i);
            while (i < result.size() && result[i] != a) 
               result.erase(result.begin() + i);
            if (i < result.size() && a == result[i]) 
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

string get_json_document(const string & CURRENCY, const string & HOST, const string & CACHE_CONTROL, const string & CONNECTION) {
   asio::ip::tcp::iostream stream;

   stream.connect(HOST, "http");
   stream    << "GET /daily/" << CURRENCY << ".json" << " HTTP/1.1\r\n";
   stream    << "Host: " + HOST + "\r\n";
   stream    << "Accept: application/json\r\n";
   stream    << "Cache-Control: " + CACHE_CONTROL + "\r\n";
   stream    << "Connection: " + CONNECTION + "\r\n\r\n" << flush;

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

   map<string, rates_info> rates = jdata;
   for (const pair<const string, rates_info> &p : rates)
   {
      cout << " 1 " << CURRENCY << " = " << p.second.inverse_rate
           << " 1 " << p.second.code << " = " << p.second.rate << endl;
                  
   }
}

int main() {
	try {
		const string CURRENCY = "USD";
      const string HOST = "www.floatrates.com";
      const string CONNECTION = "close";
      const string CACHE_CONTROL = "no-store";
		const string DOC = get_json_document(CURRENCY, HOST, CONNECTION, CACHE_CONTROL);
      
		process_json_document(CURRENCY, DOC);
	}
	catch (exception & e) {
		cerr << "Exception: " << e.what() << endl;
	}
	catch (...) {
		cerr << "Unrecognized Exception: " <<  endl;
	}
}



