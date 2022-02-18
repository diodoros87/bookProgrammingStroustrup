#ifndef FLOATRATES_DOWNLOADER_HPP
#define FLOATRATES_DOWNLOADER_HPP

#include "network.hpp"
#include "floatrates.hpp"
#include "floatrates_json.hpp"
#include "floatrates_xml.hpp"
#include "factory.hpp"
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
   
   inline void set_float_rates() {
      delete float_rates;
#ifdef __clang__
      float_rates = new Float_rates_json { "", false };
#elif defined(__GNUG__)
      float_rates = new Float_rates_json {""};
#endif
   }
   
   template <const bool FLAG>
   string get_by_asio(); 
public:
   Floatrates_downloader(const File_format & FORMAT, const string & CURRENCY = "usd") 
            : currency(format_currency(CURRENCY)) { 
      if (FORMAT == File_format::NONE)
         throw std::invalid_argument(__func__ + string(" file format NONE is not allowed"));
      set_format(FORMAT); 
   }

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
   
   Floatrates_downloader(const Floatrates_downloader &);
   Floatrates_downloader(Floatrates_downloader &&) noexcept;
   Floatrates_downloader & operator=(const Floatrates_downloader &);
   Floatrates_downloader & operator=(Floatrates_downloader &&) noexcept;
   ~Floatrates_downloader() { delete float_rates; }
};

#endif
