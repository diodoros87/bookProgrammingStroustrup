#include <iostream>
//#include <ostream>
#include <string>
#include <map>
//#include <algorithm>

#include "curl_easy.h"
#include "curl_exception.h"

#include "curl_ios.h"

#include "nlohmann/json.hpp"

using curl::curlcpp_traceback;
using curl::curl_ios;
using curl::curl_easy;
using curl::curl_easy_exception;

using namespace std;

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

void session_info(string url) {
// Let's declare a stream
   ostringstream stream;

   // We are going to put the request's output in the previously declared stream
   curl_ios<ostringstream> ios(stream);

   // Declaration of an easy object
   curl_easy easy(ios);

   // Add some option to the curl_easy object.
   easy.add<CURLOPT_URL>(url.c_str());
   easy.add<CURLOPT_FOLLOWLOCATION>(1L);

   try {
      easy.perform();

   // Retrieve information about curl current session.
   auto x = easy.get_info<CURLINFO_CONTENT_TYPE>();

   /**
      * get_info returns a curl_easy_info object. With the get method we retrieve
      * the std::pair object associated with it: the first item is the return code of the
      * request. The second is the element requested by the specified libcurl macro.
      */
   std::cout<<x.get()<<std::endl;

   } catch (curl_easy_exception &error) {
   // If you want to print the last error.
   std::cerr<<error.what()<<std::endl;

   // If you want to print the entire error stack you can do
   error.print_traceback();
   }
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
   //session_info(URL);
}
