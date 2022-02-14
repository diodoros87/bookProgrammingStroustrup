#include "xml_processing.hpp"

#include <iostream>
#include <string>

#include "curl_header.h"
#include "curl_easy.h"
#include "curl_exception.h"
#include "curl_ios.h"

using namespace curl;
using namespace xml_NBP;
using namespace std; 

using namespace xml_NBP;

class Curl_dowloader  : public Download_Interface {
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
public:
   void download(const char* URL) override {
      if (URL == nullptr)
         throw invalid_argument("URL is nullptr");
      curl_header header;
      header.add("Accept: application/xml");
      stringstream stream = get_document(URL, header);
      set_doc(stream.str());
   }
};

int main() {
   try {
      Curl_dowloader downloader;
      Download_Interface * downloader_ptr = &downloader;
      return Xml_NBP_processing::download(downloader_ptr) ? 0 : 1;
   }
   catch (const exception & e) {
      cerr << "Exception: " << typeid(e).name() << " : " << e.what() << endl;
   }
   catch (...) {
      cerr << "Unrecognized Exception: " <<  endl;
   }
   return 1;
}
