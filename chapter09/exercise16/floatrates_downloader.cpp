#include "floatrates_downloader.hpp"

using namespace network;
using namespace std;

// How do I prevent the “static initialization order problem” for my static data members?
// replace of const string Floatrates_downloader::HOST = "www.floatrates.com";
const string & static_host() {
  static const string host = "www.floatrates.com";
  return host;
}

//const string Floatrates_downloader::HOST = "www.floatrates.com";
const Method Floatrates_downloader::ST_METHOD = Method::get;
const Cache_control Floatrates_downloader::ST_CACHE_CONTROL = Cache_control::no_store;
const Connection Floatrates_downloader::ST_CONNECTION = Connection::close;

template <const bool FLAG>
string Floatrates_downloader::get_by_asio() {
   using Downloader = std::conditional_t<FLAG, Json_downloader, Xml_downloader>;
   cerr << __func__ << " currency = " << currency << '\n';
   const string DIRECTORY = "/daily/" + currency + (FLAG ? ".json" : ".xml");
   //const string DIRECTORY = "/";            // "/" is root (main page of host) and "" has result 400 Bad Request
   static const string & HOST = static_host();
   Downloader downloader(HOST, ST_METHOD, DIRECTORY, ST_CACHE_CONTROL, ST_CONNECTION);
   downloader.download();
   const string DOC = downloader.get();
   return DOC;
}

void Floatrates_downloader::set_format(const File_format & FORMAT) { 
   if (FORMAT == format)
      return;
   switch (FORMAT) {
      case File_format::JSON :
         getter = &Floatrates_downloader::get_by_asio<true>; 
         set_float_rates();
         break;
      case File_format::XML :
         getter = &Floatrates_downloader::get_by_asio<false>;
         set_float_rates();
         break;
      case File_format::NONE :
         throw invalid_argument(__func__ + string(" file format NONE is not allowed"));
      default:
         throw invalid_argument(__func__ + string(" Invalid file format ") + std::to_string(static_cast<int>(format)));
   }
   format = FORMAT; 
}

string Floatrates_downloader::format_currency(const string & CURRENCY) {
   if (CURRENCY.size() != 3)
      throw invalid_argument (" currency must be exactly 3 characters: " + CURRENCY);
   string result = CURRENCY;
   std::transform(result.begin(), result.end(), result.begin(),
      [](unsigned char c){ return std::tolower(c); });
   return result;
}

void Floatrates_downloader::download() {
   const string DOC = get_by_asio();
   float_rates->set_document(DOC);
   float_rates->set_rates_from_doc();
   map <string, float_rates_info> rates = float_rates->float_rates();
   for (const pair<string, float_rates_info> &p : rates) 
      cout << " 1 " << currency << " = " << p.second.rate << " " << p.second.code << " and "
         << " 1 " << p.second.code << " = " << p.second.inverse_rate << " " << currency << endl;
}
