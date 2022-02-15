#include <iostream>
#include "network.hpp"
#include "floatrates.hpp"
#include "asio_downloader.hpp"
#include "json_downloader.hpp"

using namespace std;
using namespace network;

class Currency_rates {
   string currency;

   template <const bool FLAG>
   string get_by_asio() {
      using Downloader = std::conditional_t<FLAG, Json_downloader, Json_downloader>;
      cerr << __func__ << " currency = " << currency << '\n';
      const string HOST = "www.floatrates.com";
      const Method METHOD = Method::get;
      const string DIRECTORY = "/daily/" + currency + (FLAG ? ".json" : "xml");
      //const string DIRECTORY = "/";            // "/" is root (main page of host) and "" has result 400 Bad Request
      const Cache_control CACHE_CONTROL = Cache_control::no_store;
      const Connection CONNECTION = Connection::close;
      Downloader downloader(HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION);
      downloader.download();
      const string JSON_DOC = downloader.get();
      return JSON_DOC;
   }
public:
   Currency_rates(const string & CURRENCY = "usd") : currency(format_currency(CURRENCY)) { }

   static string format_currency(const string & CURRENCY) {
      if (CURRENCY.size() != 3)
         throw invalid_argument (" currency must be exactly 3 characters: " + CURRENCY);
      string result = CURRENCY;
      std::transform(result.begin(), result.end(), result.begin(),
         [](unsigned char c){ return std::tolower(c); });
      return result;
   }
   
   string get_by_asio(const File_format & format) {
      switch (format) {
         case File_format::JSON :
            return get_by_asio<true>(); 
         case File_format::XML :
            return get_by_asio<false>(); 
         default:
            throw invalid_argument(__func__ + string(" Invalid file format ") + std::to_string(static_cast<int>(format)));
      }
   }
   
   string get_currency() const { return currency; }
   void set_currency(const string & CURRENCY) { currency = format_currency(CURRENCY); }
};

void view_document(const string & CURRENCY, const string & JSON_DOC) {
   Float_rates floatrates = { JSON_DOC };
   //floatrates.set_json_document(JSON_DOC);
   //floatrates.set_rates_from_json();
   map <string, float_rates_info> rates = floatrates.float_rates();
   for (const pair<string, float_rates_info> &p : rates) 
      cout << " 1 " << CURRENCY << " = " << p.second.rate << " " << p.second.code << " and "
         << " 1 " << p.second.code << " = " << p.second.inverse_rate << " " << CURRENCY << endl;
}

int main() {
   try {
      const string CURRENCY = "PLN";
      Currency_rates rates(CURRENCY);
      const string DOC = rates.get_by_asio(File_format::JSON);
      view_document(CURRENCY, DOC);
      return 0;
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
