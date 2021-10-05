#include <iostream>
#include <string>

#include "curl_header.h"
#include "curl_easy.h"
#include "curl_exception.h"
#include "curl_ios.h"

#include "pugixml.hpp"

using namespace curl;
using namespace pugi;
using namespace std; 

stringstream get_document(const char* URL, const curl_header & HEADER) {
   stringstream stream;
   try {
      curl_ios<stringstream> writer(stream);
      curl_easy easy(writer);
      
      easy.add<CURLOPT_HTTPHEADER>(HEADER.get());
      easy.add<CURLOPT_URL>(URL);
      easy.add<CURLOPT_TIMEOUT>(10);
      easy.add<CURLOPT_FOLLOWLOCATION>(1);

      easy.perform();
      return stream;
   }
   catch (const curl_easy_exception & error) {
      curlcpp_traceback errors = error.get_traceback();
      error.print_traceback();
   }
}

string get_xml_document(const char* URL) {
   curl_header header;
   header.add("Accept: application/xml");
   header.add("Accept-Charset: utf-8");
   stringstream stream = get_document(URL, header);
   return stream.str();
}

void process_xml_document(const char * STR);
void process_xml_document();

int main() {
   string CURRENCY = "PLN";
   static constexpr char* URL = "http://api.nbp.pl/api/exchangerates/tables/a/";
   const string XML_DOC = get_xml_document(URL);
   process_xml_document();
   process_xml_document(XML_DOC.c_str());
   
/*
   nlohmann::json jdata;
   doc >> jdata;

   map<string, rates_info> rates = jdata;
   for (const pair<const string, rates_info> &p : rates) {
      cout << " 1 PLN = " << CURRENCY << " = " << p.second.inverse_rate 
         << " 1 " << p.second.code << " = " << p.second.rate << std::endl;
                
   }*/
}

inline void validate_xml(xml_document& XML_DOC, const char * STR) {
   xml_parse_result result = XML_DOC.load_string(STR);
   if (! result)  {
      cerr << "XML [" << STR << "] parsed with errors, attr value: [" 
         << XML_DOC.child("node").attribute("attr").value() << "]\n";
      cerr << "Error description: " << result.description() << "\n";
      cerr << "Error offset: " << result.offset << " (error at [..." << STR[result.offset] << "]\n\n";
      throw runtime_error("Error: Document is not XML");
   }
}

void process_xml_document(const char * STR) {
   static constexpr char* CURRENCY_NODE = "/ArrayOfExchangeRatesTable/ExchangeRatesTable/Rates/Rate/Currency";
   static constexpr char* CODE_NODE = "/ArrayOfExchangeRatesTable/ExchangeRatesTable/Rates/Rate/Code";
   static constexpr char* MID_RATE_NODE = "/ArrayOfExchangeRatesTable/ExchangeRatesTable/Rates/Rate/Mid";
   xml_document doc;
   //xml_parse_result result = XML_DOC.load_string(STR);
   validate_xml(doc, STR);
      
   //if (doc.load_string(STR)) {
      try {
         xpath_node_set titles = doc.select_nodes(CURRENCY_NODE);

         for (xpath_node it : titles)
         {
            cout << it.node().text().as_string() << endl;
         }
      }
      catch (xpath_exception const & e) {
         cout << e.result().description() << endl;
      }
      
      try {
         auto titles = doc.select_nodes(CODE_NODE);

         for (auto it : titles)
         {
            cout << it.node().text().as_string() << endl;
         }
      }
      catch (xpath_exception const & e) {
         cout << e.result().description() << endl;
      }
   //}
   /*
   stringstream strstream;
   strstream.str(DOC);
   
   nlohmann::json jdata;
   strstream >> jdata;

   map<string, float_rates_info> rates = jdata;
   for (const pair<string, float_rates_info> &p : rates) {
      cout << " 1 " << CURRENCY << " = " << p.second.rate << " " << p.second.code << " and "
           << " 1 " << p.second.code << " = " << p.second.inverse_rate << " " << CURRENCY << endl;
                  
   }*/
}




void process_xml_document()
{
   std::string text = R"(
<?xml version="1.0"?>
<movies>
	<movie id="11001" title="Matrix" year="1999" length="196">
		<cast>
			<role star="Keanu Reeves" name="Neo" />
			<role star="Laurence Fishburne" name="Morfeusz" />
			<role star="Carrie-Anne Moss" name="Trinity" />
			<role star="Hugo Weaving" name="Agent Smith" />
		</cast>
		<directors>
			<director name="Lana Wachowski" />
			<director name="Lilly Wachowski" />
		</directors>
		<writers>
			<writer name="Lana Wachowski" />
			<writer name="Lilly Wachowski" />
		</writers>
	</movie>
	<movie id="9871" title="Forrest Gump" year="1994" length="202">
		<cast>
			<role star="Tom Hanks" name="Forrest Gump" />
			<role star="Sally Field" name="Pani Gump" />
			<role star="Robin Wright" name="Jenny Curran" />
			<role star="Mykelti Williamson" name="Bubba Blue" />
		</cast>
		<directors>
			<director name="Robert Zemeckis" />
		</directors>
		<writers>
			<writer name="Winston Groom" />
			<writer name="Eric Roth" />
		</writers>
	</movie>
</movies>
)";

   pugi::xml_document doc;
   pugi::xml_parse_result result = doc.load_string(text.c_str());
   if (result)
   {
      try
      {
         auto titles = doc.select_nodes("/movies/movie[@year>1995]");

         for (auto it : titles)
         {
            std::cout << it.node().attribute("title").as_string() << std::endl;
         }
      }
      catch (pugi::xpath_exception const & e)
      {
         std::cout << e.result().description() << std::endl;
      }

      try
      {
         auto titles = doc.select_nodes("/movies/movie/cast/role[last()]");

         for (auto it : titles)
         {
            std::cout << it.node().attribute("star").as_string() << std::endl;
         }
      }
      catch (pugi::xpath_exception const & e)
      {
         std::cout << e.result().description() << std::endl;
      }
   }
   else  {
      std::cout << "XML [" << text << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
      std::cout << "Error description: " << result.description() << "\n";
      std::cout << "Error offset: " << result.offset << " (error at [..." << (text.c_str() + result.offset) << "]\n\n";
   }
      
}

