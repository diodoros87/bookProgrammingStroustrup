#include "asio_downloader.hpp"

class Json_downloader {
   Asio_downloader downloader;
   string doc;
public:
   Json_downloader(const string & HOST, const Method & METHOD, const string & DIRECTORY,
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION) {
      downloader(HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION); }
      
   void download();
      
   string get() const { return doc; };   
   
   Asio_downloader(const string & HOST, const Method & METHOD, const string & DIRECTORY,
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION) {
      host = HOST;
      method = METHOD;
      directory = DIRECTORY;
      cache_control = CACHE_CONTROL;
      connection = CONNECTION;
   }
   string download() const;
private:
   string get_document() const;
   void process_response_headers(asio::ip::tcp::iostream & socket_iostream, const string& HTTP_VERSION) const;
}
