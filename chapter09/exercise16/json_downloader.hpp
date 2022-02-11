#ifndef JSON_DOWNLOADER_HPP
#define JSON_DOWNLOADER_HPP

#include "asio_downloader.hpp"
#include "network.hpp"

using std::cerr;

class Json_downloader {
   Asio_downloader * downloader = nullptr;
   string doc;
public:
   Json_downloader(const string & HOST, const Method & METHOD, const string & DIRECTORY,
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION) {
      cerr << " Constructor " << __func__ << '\n';
      downloader = new Asio_downloader(HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION); }
      
   void download() {
      doc = downloader->download();
      modify_document();
   }
   
   Json_downloader(const Json_downloader& other) {
      cerr << " COPY Constructor " << __func__ << '\n';
      if (nullptr != other.downloader)
         downloader = new Asio_downloader(*(other.downloader));
      doc = other.doc;
	}

	Json_downloader& operator=(const Json_downloader& other) {
      cerr << " COPY " << __func__ << '\n';
		if (&other != this) {
         delete downloader;
         if (nullptr == other.downloader)
            downloader = nullptr;
         else
            downloader = new Asio_downloader(*(other.downloader));
         doc = other.doc;
      }
		return *this;
	}
	
	Json_downloader(Json_downloader&& other) noexcept
		: downloader(other.downloader), doc(other.doc) {
      cerr << " MOVE Constructor " << __func__ << '\n';
		other.downloader = nullptr;
      other.doc = "";
	}
	
	Json_downloader& operator=(Json_downloader&& other) noexcept {
      cerr << " MOVE " << __func__ << '\n';
		if (&other != this) {
         delete downloader;
         downloader = other.downloader;
         other.downloader = nullptr;
         doc = other.doc;
         other.doc = "";
      }
		return *this;
	}

   ~Json_downloader() { cerr << __func__ << '\n'; delete downloader; }
   
   string get() const { return doc; };   
   Asio_downloader * get_asio_downloader() const { return downloader; }; 
private:
   void erase(const char, const char);
   void modify_document();
};

#endif
