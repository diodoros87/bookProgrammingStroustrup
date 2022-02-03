#include "asio_downloader.hpp"

using namespace std;

string get_document(const string & HOST, const Method & METHOD, const string & DIRECTORY,
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION) {
   asio::basic_socket_iostream<asio::ip::tcp> socket_iostream;
   const string HTTP_VERSION("HTTP/1.1");
   socket_iostream.connect(HOST, "http");
   socket_iostream.expires_from_now(chrono::seconds(10));
   socket_iostream    << METHOD << " " << DIRECTORY << " " << HTTP_VERSION << "\r\n";
   socket_iostream    << "Host: " + HOST + "\r\n";
   socket_iostream    << "Cache-Control: " << CACHE_CONTROL << "\r\n";
   socket_iostream    << "Connection: " << CONNECTION  << "\r\n\r\n";
   socket_iostream.flush();
   
   process_response_headers(socket_iostream, HTTP_VERSION);
   
   stringstream strstream;
   strstream << socket_iostream.rdbuf();
   
   socket_iostream.close();
   if (! socket_iostream)
      throw Asio_IO_Stream_Exception(socket_iostream.error().message());
   string result = strstream.str();
   modify_document(result);
   return result;
}

void process_response_headers(asio::ip::tcp::iostream & socket_iostream, const string& HTTP_VERSION) {
   string http_version;
   socket_iostream >> http_version;
   cout << " http_version = " << http_version;
   unsigned int status_code;
   socket_iostream >> status_code;
   cout << "\n status code of response = " << status_code;
   string status_message;
   getline(socket_iostream, status_message);
   cout << "\n status message = " << status_message;
   if (socket_iostream.fail() || http_version != HTTP_VERSION) 
      throw Asio_IO_Stream_Exception("Invalid response ");
   // Process the response headers, which are terminated by a blank line.
   string header;
   while (getline(socket_iostream, header) && header != "\r")
      cout << header << "\n";
   cout << "\n";
}

void erase(string & result, const char C, const char A) {
   for (unsigned i = 0; i < result.size(); i++) {
      if (C == result[i] || A == result[i]) {
         result.erase(result.begin() + i);
         if (A == result[i]) {
            result.erase(result.begin() + i);
            while (i < result.size() && result[i] != A) 
               result.erase(result.begin() + i);
            if (i < result.size() && A == result[i]) 
               result.erase(result.begin() + i);
         }
      }
   }
}

void modify_document(string & doc) {
   size_t first = doc.find("{");
   doc = doc.substr(first);
   size_t last = doc.rfind("}");
   doc = doc.substr(0, last + 1);
   erase(doc, '\r', '\n');
}
