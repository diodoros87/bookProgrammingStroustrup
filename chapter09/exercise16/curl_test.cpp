#include "xml_processing.hpp"
#include "curl_manager.hpp"

using namespace std; 

size_t writer(char *data, const size_t size, const size_t nmemb, string * writer_data) {
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

int main() {
   try {
      return Xml_NBP_processing::download(get_xml_document) ? 0 : 1;
   }
   catch (const exception & e) {
      cerr << "Exception: " << e.what() << endl;
   }
   catch (...) {
      cerr << "Unrecognized Exception: " <<  endl;
   }
   return 1;
}
