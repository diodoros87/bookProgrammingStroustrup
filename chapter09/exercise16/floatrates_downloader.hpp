#ifndef FLOATRATES_DOWNLOADER_HPP
#define FLOATRATES_DOWNLOADER_HPP

#include "network.hpp"
#include "floatrates.hpp"
#include "floatrates_json.hpp"
#include "asio_downloader.hpp"
#include "json_downloader.hpp"
#include "xml_downloader.hpp"

#include <functional>

using network::File_format;

class Floatrates_downloader {
// due to "static initialization order fiasco":
// replace of const string Floatrates_downloader::HOST = "www.floatrates.com" to function const string & static_host()
   static const Method ST_METHOD;
   static const Cache_control ST_CACHE_CONTROL;
   static const Connection ST_CONNECTION;
   
   string currency;
   File_format format = File_format::NONE;
   Float_rates * float_rates = nullptr;
   
   typedef  string (Floatrates_downloader::*Getter)();
   //string (Floatrates_downloader::*)<const bool>() getter = &Floatrates_downloader::get_by_asio<true>();
   Getter getter = &Floatrates_downloader::get_by_asio<true>;
   
   template <const bool FLAG>
   string get_by_asio(); 
public:
   Floatrates_downloader(const File_format & FORMAT, const string & CURRENCY = "usd") 
      : currency(format_currency(CURRENCY)) 
          { set_format(FORMAT); }

   static string format_currency(const string & CURRENCY);
   
   string get_by_asio(const File_format & format);
   
   string get_currency() const { return currency; }
   void set_currency(const string & CURRENCY) { currency = format_currency(CURRENCY); }
   File_format get_format() const { return format; }
   void set_format(const File_format & FORMAT);
   
   map<string, long double> rates() const {  
      return (float_rates == nullptr) ? map<string, long double> {} : float_rates->rates(); }
   
   map<string, long double> inverse_rates() const {  
      return (float_rates == nullptr) ? map<string, long double> {} : float_rates->inverse_rates(); }
   
   string get_by_asio() { 
      auto function = std::mem_fn(getter);
      return function(*this); 
   } 
   
   void download();
   
   ~Floatrates_downloader() { delete float_rates; }
;};

#endif
