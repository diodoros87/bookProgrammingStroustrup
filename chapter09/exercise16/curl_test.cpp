#include "xml_nbp.hpp"
#include "curl_manager.hpp"

using namespace std;
using namespace xml_NBP;

class Curl_dowloader  : public Download_Interface {
   static size_t writer(char *data, const size_t size, const size_t nmemb, string * writer_data) {
      if(writer_data == nullptr)
         throw invalid_argument("writer_data is nullptr");
      
      writer_data->append(data, size * nmemb);
      return size * nmemb;
   } 
   
   string get_document(const char* const URL, const curl_slist * const HEADER) {
      Curl_Manager manager;
      manager.set_option(CURLOPT_URL, URL);
      manager.set_option(CURLOPT_FOLLOWLOCATION, 1L);
      manager.set_option(CURLOPT_WRITEFUNCTION, &Curl_dowloader::writer);
      manager.set_option(CURLOPT_HTTPHEADER, HEADER);
      manager.set_option(CURLOPT_TIMEOUT, 10L);
      manager.set_option(CURLOPT_DNS_CACHE_TIMEOUT, 0L);
      string buffer;
      manager.set_option(CURLOPT_WRITEDATA, &buffer);
      manager.perform();
      return buffer;
   }
   
public:
   void download(const char* const URL) override {
      if (URL == nullptr)
         throw invalid_argument("URL is nullptr");
      struct curl_slist * header = nullptr;
      header = curl_slist_append(header, "Accept: application/xml");
      try {
         const string result = get_document(URL, header);
         set_doc(result);
         curl_slist_free_all(header);
      } catch (const exception & e) {
         cerr << __func__ << " Exception: " << typeid(e).name() << " : " << e.what() << endl;
         curl_slist_free_all(header);
         throw;
      }
      catch (...) {
         cerr << __func__ << " Unrecognized Exception: " <<  endl;
         curl_slist_free_all(header);
         throw;
      }
   }
};

int main() {
   Download_Interface * downloader = nullptr;
   try {
      downloader = new Curl_dowloader;
      int result = Xml_NBP_processing::download(downloader) ? 0 : 1;
      delete downloader;
      return result;
   }
   catch (const exception & e) {
      cerr << __func__ << " Exception: " << typeid(e).name() << " : " << e.what() << endl;
   }
   catch (...) {
      cerr << __func__ << " Unrecognized Exception: " <<  endl;
   }
   delete downloader;
   return 1;
}
