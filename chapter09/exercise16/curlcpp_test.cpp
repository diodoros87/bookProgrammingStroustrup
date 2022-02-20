#include "xml_nbp.hpp"

#include <iostream>
#include <string>

#include "curl_header.h"
#include "curl_easy.h"
#include "curl_exception.h"
#include "curl_ios.h"

#include "curlcpp_dowloader.hpp"

using namespace curl;
using namespace std; 

using namespace xml_NBP;

int main() {
   try {
      Curlcpp_dowloader downloader { File_format::XML };
      Curl_interface * downloader_ptr = &downloader;
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
