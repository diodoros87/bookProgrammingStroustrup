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
/*
template<class T> size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
   if(userp == nullptr)
      throw invalid_argument("nullptr");
   const size_t realsize = size*nmemb;
   T *const stream = static_cast<T*>(userp);
   stream->write(static_cast<const char *>(contents),realsize);
   return realsize;
}

template<class T> curl_easy::curl_easy(curl_ios<T> &writer) : curl_interface() {
   this->curl = curl_easy_init();
   if (this->curl == nullptr) {
      throw curl_easy_exception("Null pointer intercepted",__FUNCTION__);
   }
   this->add(curl_pair<CURLoption,curlcpp_callback_type>(CURLOPT_WRITEFUNCTION,writer.get_function()));
   this->add(curl_pair<CURLoption,void *>(CURLOPT_WRITEDATA, static_cast<void*>(writer.get_stream())));
}

*/

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
   char buffer[9 * CURL_ERROR_SIZE];
public:
   Curl_Manager(CURL *& connection) {
      CURLcode code = curl_easy_setopt(connection, CURLOPT_ERRORBUFFER, buffer);
      if(code != CURLE_OK)
         throw Curl_Error("Failed to set error buffer code is " + to_string(code));
   }
   
   static void st_check_error(CURLcode & code, const string& message) {
      if(code != CURLE_OK)
         throw Curl_Error(message + " " + curl_easy_strerror(code));
   }

   void check_error(CURLcode & code, const string& message) {
      if(code != CURLE_OK)
         throw Curl_Error(message + " " + string(buffer));
   }
};

int writer(char *data, const size_t size, const size_t nmemb, string *writer_data) {
  if(writer_data == nullptr)
    throw invalid_argument("nullptr");
 
  writer_data->append(data, size * nmemb);
 
  return size * nmemb;
} 

string get_document(const char* URL, const curl_slist * const HEADER) {
   //stringstream stream;
   string buffer;
   {
      CURLcode code = curl_global_init(CURL_GLOBAL_DEFAULT);
      Curl_Manager::st_check_error(code, "Failed to curl_global_init");
      CURL *connection = curl_easy_init();
      if (connection == nullptr) 
         throw runtime_error("Failed to create CURL connection");
      
      //init(CURL_GLOBAL_ALL);
      Curl_Manager manager { connection };
      code = curl_easy_setopt(connection, CURLOPT_URL, URL);
      manager.check_error(code, "Failed to set URL");
      code = curl_easy_setopt(connection, CURLOPT_FOLLOWLOCATION, 1L);
      manager.check_error(code, "Failed to set redirect option");
      code = curl_easy_setopt(connection, CURLOPT_WRITEFUNCTION, writer);
      manager.check_error(code, "Failed to set write callback function");
      code = curl_easy_setopt(connection, CURLOPT_WRITEDATA, &buffer);
      manager.check_error(code, "Failed to set write data");
      code = curl_easy_setopt(connection, CURLOPT_HTTPHEADER, HEADER);
      manager.check_error(code, "Failed to http header");
      code = curl_easy_setopt(connection, CURLOPT_TIMEOUT, 10L);
      manager.check_error(code, "Failed to set timeout");
      code = curl_easy_setopt(connection, CURLOPT_DNS_CACHE_TIMEOUT, 0L);
      manager.check_error(code, "Failed to set DNS_CACHE timeout");

      code = curl_easy_perform(connection);
      curl_easy_cleanup(connection);
      manager.check_error(code, "Failed to curl_easy_perform");
   }
   return buffer;
}

string get_xml_document(const char* URL) {
   struct curl_slist * header = nullptr;
   header = curl_slist_append(header, "Accept: application/xml");
   //header.add("Accept: application/xml");
   return get_document(URL, header);
   //stringstream stream = get_document(URL, header);
   //return stream.str();
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
