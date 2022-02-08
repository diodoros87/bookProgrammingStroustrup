#ifndef JSON_DOWNLOADER_HPP
#define JSON_DOWNLOADER_HPP

#include "asio_downloader.hpp"
#include "network.hpp"

class Json_downloader {
   Asio_downloader * downloader = nullptr;
   string doc;
public:
   Json_downloader(const string & HOST, const Method & METHOD, const string & DIRECTORY,
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION) {
      downloader = new Asio_downloader(HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION); }
      
   void download() {
      doc = downloader->download();
      modify_document();
   }
   
   ~Json_downloader() { delete downloader; }
   
   string get() const { return doc; };   
private:
   void erase(const char, const char);
   void modify_document();
};

#endif
