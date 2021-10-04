#include <iostream>
#include <string>
#include <map>

#include "curl_easy.h"
#include "curl_exception.h"
#include "curl_ios.h"

#include "pugixml.hpp"

using curl::curlcpp_traceback;
using curl::curl_ios;
using curl::curl_easy;
using curl::curl_easy_exception;

using namespace std; 
/*
struct rates_info {
   string code;
   //string alpha_code;
   //string numeric_code;
   //string name;
   double rate;
   //string date;
   double inverse_rate;
};

void from_json(const nlohmann::json& jdata, rates_info& info) {
   info.code = jdata.at("code").get<string>();
   //info.alpha_code = jdata.at("alphaCode").get<string>();
   //info.numeric_code = jdata.at("numericCode").get<string>();
   //info.name = jdata.at("name").get<string>();
   info.rate = jdata.at("rate").get<double>();
   //info.date = jdata.at("date").get<string>();
   info.inverse_rate = jdata.at("inverseRate").get<double>();
}

stringstream get_json_document(const string& url) {
   stringstream str;

   try {
      curl_ios<stringstream> writer(str);
      curl_easy easy(writer);

      easy.add<CURLOPT_URL>(url.data());
      easy.add<CURLOPT_FOLLOWLOCATION>(1L);

      easy.perform();
   }
   catch (const curl_easy_exception & error) 
   {
      curlcpp_traceback errors = error.get_traceback();
      error.print_traceback();
   }
   
   return str;
}

int main() {
   string CURRENCY = "USD";
   const string URL = "http://www.floatrates.com/daily/";
   stringstream doc = get_json_document(URL + CURRENCY + ".json");

   nlohmann::json jdata;
   doc >> jdata;

   map<string, rates_info> rates = jdata;
   //transform(CURRENCY.begin(), CURRENCY.end(),CURRENCY.begin(), ::toupper);
   for (const pair<const string, rates_info> &p : rates) {
      cout << " 1 " << CURRENCY << " = " << p.second.inverse_rate 
         << " 1 " << p.second.code << " = " << p.second.rate << std::endl;
                
   }
}
*/



int main()
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
   if (doc.load_string(text.c_str()))
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
}
