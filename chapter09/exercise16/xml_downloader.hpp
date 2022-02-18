#ifndef XML_DOWNLOADER_HPP
#define XML_DOWNLOADER_HPP

#include "asio_downloader.hpp"
#include "network.hpp"
#include "doc_downloader.hpp"

#include <memory>

using std::cerr;
using std::unique_ptr;

//template<class T> 
//inline T& unmove(T&& t) { return t; }

class Xml_downloader : Doc_downloader {
   unique_ptr<Asio_downloader> downloader;
   //Asio_downloader & downloader;
   //Asio_downloader && downloader;
   string doc;
public:
   Xml_downloader(const string & HOST, const Method & METHOD, const string & DIRECTORY,
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION) {
      //Asio_downloader d = { HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION };
      downloader.reset(new Asio_downloader{ HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION });
      cerr << " Constructor " << __func__ << '\n'; }
      
   //Xml_downloader(Asio_downloader & d) : downloader (d) {
   //   cerr << " Constructor " << __func__ << '\n'; }
   /*
   Xml_downloader(const Xml_downloader& other) = default;

	Xml_downloader& operator=(const Xml_downloader& other) = default; // explicitly defaulted function was implicitly deleted 
	
	Xml_downloader(Xml_downloader&& other)   = default;
	
	Xml_downloader& operator=(Xml_downloader&& other)  = default; // explicitly defaulted function was implicitly deleted 
	*/
   Xml_downloader(const Xml_downloader& other) {
      cerr << " COPY Constructor " << __func__ << '\n';
      if (nullptr != other.downloader.get())
         downloader.reset(new Asio_downloader(*(other.downloader)));
      doc = other.doc;
	}

	Xml_downloader& operator=(const Xml_downloader& other) {
      cerr << " COPY " << __func__ << '\n';
		if (&other != this) {
         if (nullptr == other.downloader.get())
            downloader.reset(nullptr);
         else
            downloader.reset(new Asio_downloader(*(other.downloader)));
         doc = other.doc;
      }
		return *this;
	}
	
	Xml_downloader(Xml_downloader&& other) noexcept
		: doc(other.doc) {
      cerr << " MOVE Constructor " << __func__ << '\n';
		//other.downloader = nullptr;
		downloader.reset(other.downloader.release());
      other.doc = "";
      assert(other.downloader.get() == nullptr);
      assert(other.downloader == nullptr);
	}
	
	Xml_downloader& operator=(Xml_downloader&& other) noexcept {
      cerr << " MOVE " << __func__ << '\n';
		if (&other != this) {
         downloader.reset(other.downloader.release());
         assert(other.downloader.get() == nullptr);
         assert(other.downloader == nullptr);
         doc = other.doc;
         other.doc = "";
      }
		return *this;
	}

   ~Xml_downloader() { cerr << __func__ << '\n'; }
   
   void download() override {
      doc = downloader->download();
      cerr << __func__ << doc << '\n';
      modify_document();
      
   }
   
   string get() const { return doc; };   
   Asio_downloader * get_asio_downloader() const { return downloader.get(); }; 
private:
   void erase(const char, const char);
   void modify_document();
};

#endif
