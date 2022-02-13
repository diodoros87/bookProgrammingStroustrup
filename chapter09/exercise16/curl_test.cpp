#include <iostream>
#include <string>
#include <array>

#include <curl/curl.h>
/*
#include "curl_header.h"
#include "curl_easy.h"
#include "curl_exception.h"
#include "curl_ios.h"
*/
#include "pugixml.hpp"

//using namespace curl;
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

struct Curl_Error : public exception {
private:
   static string msg;
public:
   Curl_Error() { }
   Curl_Error(const string& message) {  msg += message; }
   const char* what() const noexcept {
      return msg.c_str();
   }
};
string Curl_Error::msg = {"!!! error with curl library: "};

class Curl_Manager {
   CURL * connection = nullptr;
   CURLcode code = CURLE_FAILED_INIT;
   
   string get_message(const CURLoption & option) {
      switch (option) {
         case CURLOPT_URL:
            return "Failed to set URL";
         case CURLOPT_FOLLOWLOCATION:
            return "Failed to set redirect option";
         case CURLOPT_WRITEFUNCTION:
            return "Failed to set write callback function";
         case CURLOPT_WRITEDATA:
            return "Failed to set write data";
         case CURLOPT_HTTPHEADER:
            return "Failed to set http header";
         case CURLOPT_TIMEOUT:
            return "Failed to set timeout";
         case CURLOPT_DNS_CACHE_TIMEOUT:
            return "Failed to set DNS_CACHE timeout";
         default:
            return "";
      }
   }
public:
   Curl_Manager() {
      code = curl_global_init(CURL_GLOBAL_DEFAULT);
      check_error("Failed to curl_global_init");
      connection = curl_easy_init();
      if (connection == nullptr) 
         throw Curl_Error("Failed to create CURL connection - connection is null pointer");
   }
   
   template <class T>
   inline void set_option(const CURLoption & option, const T & value) {
      code = curl_easy_setopt(connection, option, value);
      check_error(get_message(option));
   }
   
   inline void check_error(const string& message) {
      if(code != CURLE_OK)
         throw Curl_Error(message + " " + curl_easy_strerror(code));
   }
   
   inline void perform() {
      code = curl_easy_perform(connection);
      if(code != CURLE_OK) {
         curl_easy_cleanup(connection);
         throw Curl_Error(string("Failed to curl_easy_perform ") + curl_easy_strerror(code));
      }
   }
   
   ~Curl_Manager() { 
      curl_easy_cleanup(connection); 
      check_error("Failed to curl_easy_perform"); 
   }
};

size_t writer(char *data, const size_t size, const size_t nmemb, string *writer_data) {
  if(writer_data == nullptr)
    throw invalid_argument("nullptr");
 
  writer_data->append(data, size * nmemb);
  return size * nmemb;
} 

string get_document(const char* URL, const curl_slist * const HEADER) {
   Curl_Manager manager;
   manager.set_option(CURLOPT_URL, URL);
   manager.set_option(CURLOPT_FOLLOWLOCATION, 1L);
   manager.set_option(CURLOPT_WRITEFUNCTION, writer);
   manager.set_option(CURLOPT_HTTPHEADER, HEADER);
   manager.set_option(CURLOPT_TIMEOUT, 10L);
   manager.set_option(CURLOPT_DNS_CACHE_TIMEOUT, 0L);
   string buffer;
   manager.set_option(CURLOPT_WRITEDATA, &buffer);
   manager.perform();
   return buffer;
}

string get_xml_document(const char* URL) {
   struct curl_slist * header = nullptr;
   header = curl_slist_append(header, "Accept: application/xml");
   return get_document(URL, header);
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
