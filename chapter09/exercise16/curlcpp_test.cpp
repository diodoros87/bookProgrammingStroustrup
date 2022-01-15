#include <iostream>
#include <string>
#include <array>

#include "curl_header.h"
#include "curl_easy.h"
#include "curl_exception.h"
#include "curl_ios.h"
#include "pugixml.hpp"

using namespace curl;
using namespace pugi;
using namespace std; 

bool process_xml_document(const char * STR);
string get_xml_document(const char* URL);

int main() {
   try {
      static const array<const char*, 2> URL_ARRAY = { "http://api.nbp.pl/api/exchangerates/tables/a/",
                                                       "http://api.nbp.pl/api/exchangerates/tables/b/" };
      string xml_doc;
      for (const char* URL :  URL_ARRAY) {
         xml_doc = get_xml_document(URL);
         if (xml_doc.empty()) {
            cerr << "Error: Xml document can not be empty\n";
            return 1;
         }
         if (! process_xml_document(xml_doc.c_str()))
            return 1;
      }
      return 0;
   }
   catch (const exception & e) {
      cerr << "Exception: " << e.what() << endl;
   }
   catch (...) {
      cerr << "Unrecognized Exception: " <<  endl;
   }
   return 1;
}

stringstream get_document(const char* URL, const curl_header & HEADER) {
   stringstream stream;
   try {
      curl_ios<stringstream> writer(stream);
      curl_easy easy(writer);
      
      easy.add<CURLOPT_HTTPHEADER>(HEADER.get());
      easy.add<CURLOPT_URL>(URL);
      easy.add<CURLOPT_TIMEOUT>(10);
      easy.add<CURLOPT_DNS_CACHE_TIMEOUT>(0);
      easy.add<CURLOPT_FOLLOWLOCATION>(1);

      easy.perform();
   }
   catch (const curl_easy_exception & e) {
      curlcpp_traceback errors = e.get_traceback();
      e.print_traceback();
   }
   return stream;
}

string get_xml_document(const char* URL) {
   curl_header header;
   header.add("Accept: application/xml");
   stringstream stream = get_document(URL, header);
   //curl_global_cleanup();
   return stream.str();
}

inline bool load_validate_xml(xml_document& xml_doc, const char * STR) {
   xml_parse_result result = xml_doc.load_string(STR);
   if (result.status != xml_parse_status::status_ok)  {
      cerr << "XML [" << STR << "] parsed with errors, attr value: [" 
         << xml_doc.child("node").attribute("attr").value() << "]\n";
      cerr << "Error description: " << result.description() << "\n";
      cerr << "Error offset: " << result.offset << " (error at [..." << STR[result.offset] << "]\n\n";
   }
   return result;
}

bool print(const xpath_node_set & CURRENCIES, const xpath_node_set & CODES, const xpath_node_set & RATES) {
   const size_t SIZE = CURRENCIES.size();
   if (SIZE != CODES.size() || SIZE != RATES.size()) {
      cerr << "Different size of xpath node sets:\n";
      cerr << "CURRENCIES = " << SIZE << "\n";
      cerr << "CODES = " << CODES.size() << "\n";
      cerr << "RATES = " << RATES.size() << "\n";
      return false;
   }
   for (size_t i = 0; i < SIZE; i++) {
      xpath_node  code_xpathnode = CODES[i];
      xml_node    code_xmlnode   = code_xpathnode.node();
      xml_text    code_xmltext   = code_xmlnode.text();
      xpath_node  rate_xpathnode = RATES[i];
      xml_node    rate_xmlnode   = rate_xpathnode.node();
      xml_text    rate_xmltext   = rate_xmlnode.text();
      double rate                = stod(rate_xmltext.as_string());
      double inverse_rate        = 1.0 / rate;
      cout << " 1 PLN = " << inverse_rate << " " << code_xmltext.get() << " and"
           << " 1 " << code_xmltext.as_string() << " = " << rate << " PLN\n";
   }
   return true;
}

bool process_xml_document(const char * STR) {
   xml_document doc;
   if (! load_validate_xml(doc, STR))
      return false;
   try {
      static constexpr char* CURRENCY_NODE = "/ArrayOfExchangeRatesTable/ExchangeRatesTable/Rates/Rate/Currency";
      static constexpr char* CODE_NODE = "/ArrayOfExchangeRatesTable/ExchangeRatesTable/Rates/Rate/Code";
      static constexpr char* MID_RATE_NODE = "/ArrayOfExchangeRatesTable/ExchangeRatesTable/Rates/Rate/Mid";
      const xpath_node_set CURRENCIES = doc.select_nodes(CURRENCY_NODE);
      const xpath_node_set CODES = doc.select_nodes(CODE_NODE);
      const xpath_node_set RATES = doc.select_nodes(MID_RATE_NODE);
      return print(CURRENCIES, CODES, RATES);
   }
   catch (xpath_exception const & e) {
      cerr << e.result().description() << endl;
      return false;
   }
}
