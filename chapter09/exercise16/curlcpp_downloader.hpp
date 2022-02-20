#ifndef CURLCPP_DOWNLOADER_HPP
#define CURLCPP_DOWNLOADER_HPP

#include "curl_interface.hpp"

class Curlcpp_dowloader  : public Curl_interface {
   stringstream get_document(const char* const URL, const curl_header & HEADER) {
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
         throw;
      }
      return stream;
   }
public:
   void download(const char* const URL) override {
      if (URL == nullptr)
         throw invalid_argument("URL is nullptr");
      const string accept_content = get_accept_content();
      curl_header header;
      header.add(accept_content);
      stringstream stream = get_document(URL, header);
      set_doc(stream.str());
   }
   
   using Curl_interface::Curl_interface;
   
   Curlcpp_dowloader(const Curlcpp_dowloader &) = default;
   Curlcpp_dowloader(Curlcpp_dowloader &&) = default;
   Curlcpp_dowloader & operator=(const Curlcpp_dowloader &) = default;
   Curlcpp_dowloader & operator=(Curlcpp_dowloader &&) = default;
};

#endif
