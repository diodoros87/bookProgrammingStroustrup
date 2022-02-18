#ifndef FLOATRATES_TEST_HPP
#define FLOATRATES_TEST_HPP

#include "floatrates_downloader.hpp"

using std::invalid_argument;
using std::cout;

namespace float_rates_test {
   
void test();
   
using network::File_format;

class Float_rates_test {
   File_format format = File_format::NONE;
   string currency;
   Float_rates * rates_ptr = nullptr;
public:   
   Float_rates_test(const File_format & f, const string & c) : currency(c) {
      if ( c.length() == 0)
         throw invalid_argument(string(__func__) + "empty string can not be accepted ");
      if (f == File_format::NONE)
         throw invalid_argument(__func__ + string(" file format NONE is not allowed"));
      set_format(f);
   }
   
   void run() {
      Floatrates_downloader downloder(format, currency);
      const string doc = downloder.get_by_asio();
      rates_ptr->set_document(doc);
      rates_ptr->set_rates_from_doc();
      map <string, float_rates_info> rates = rates_ptr->float_rates();
      for (const pair<string, float_rates_info> &p : rates) 
         cout << " 1 " << currency << " = " << p.second.rate << " " << p.second.code << " and "
            << " 1 " << p.second.code << " = " << p.second.inverse_rate << " " << currency << '\n';
   }
   
   void set_format(const File_format & FORMAT) { 
      if (FORMAT == format)
         return;
      switch (FORMAT) {
         case File_format::JSON :
            delete rates_ptr;
            rates_ptr = Float_rates_factory<Float_rates_json>().create(); 
            break;
         case File_format::XML :
            delete rates_ptr;
            rates_ptr = Float_rates_factory<Float_rates_xml>().create(); 
            break;
         default:
            throw invalid_argument(__func__ + string(" Invalid file format ") + std::to_string(static_cast<int>(format)));
      }
      format = FORMAT; 
   }
   
   Float_rates_test(Float_rates_test&& other) noexcept
		: currency(other.currency), format(other.format), rates_ptr(other.rates_ptr) {
      cerr << " MOVE Constructor " << __func__ << '\n';
		other.rates_ptr = nullptr;
      other.currency = "";
	}
	
	Float_rates_test& operator=(Float_rates_test&& other) noexcept {
      cerr << " MOVE " << __func__ << '\n';
		if (&other != this) {
         delete rates_ptr;
         rates_ptr = other.rates_ptr;
         other.rates_ptr = nullptr;
         currency = other.currency;
         other.currency = "";
         format = other.format;
      }
		return *this;
	}
   
   Float_rates_test(Float_rates_test const &) = delete;
   Float_rates_test& operator=(Float_rates_test const &) = delete;
   ~Float_rates_test() { delete rates_ptr; }
};

}

#endif
