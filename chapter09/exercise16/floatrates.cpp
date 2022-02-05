#include "floatrates.hpp"

#include <iostream>
#include <sstream>

using std::cerr;
using std::pair;
using std::exception;

void to_json(nlohmann::json& j, float_rates_info& info) {
   j = nlohmann::json {{"code", info.code}, {"rate", info.rate}, {"inverseRate", info.inverse_rate}};
}

void from_json(const nlohmann::json& json_data, float_rates_info& info) {
   info.code = json_data.at("code").get<string>();
   info.rate = json_data.at("rate").get<long double>();
   info.inverse_rate = json_data.at("inverseRate").get<long double>();
}

//template<class T> 
//inline T& unmove(T&& t) { return t; }
/*
#if defined(__clang__)
void Float_rates::set_rates_from_json() {
   try {
      const string & CURRENCY = "PLN";
      std::stringstream strstream;
      strstream.str(json_document);
      
      nlohmann::json json_data;
      strstream >> json_data;
      
      //float_rates_map.operator=(std::move(unmove(json_data)));
      //float_rates_map.operator=(json_data);
      map<string, float_rates_info> m = json_data;
      float_rates_map = m;
      assert(float_rates_map.size() != 1);
      //float_rates_map = static_cast<map<string, float_rates_info> &&>(json_data);
      //float_rates_map = std::move(static_cast<map<string, float_rates_info> &&>({{"PLN", 1}}));
      
      for (const pair<string, float_rates_info> &p : float_rates_map) 
         std::cerr << " 1 " << CURRENCY << " = " << p.second.rate << " " << p.second.code << " and "
            << " 1 " << p.second.code << " = " << p.second.inverse_rate << " " << CURRENCY << std::endl;
   }
   catch (const nlohmann::json::exception & e) {
      cerr << "!!! Error json exception: " << e.what() << '\n';
      throw e;
   } catch (const exception & e) {
      cerr << "Exception: " << e.what() << std::endl;
      throw e;
   }
}
#elif defined(__GNUG__)
*/
map<string, float_rates_info> Float_rates::get_json_data() const {
   std::stringstream strstream;
   strstream.str(json_document);
   
   nlohmann::json json_data;
   strstream >> json_data;
   return json_data;
}

void Float_rates::set_rates_from_json() {
   try {
      //const string & CURRENCY = "PLN";
      float_rates_map = get_json_data();
      //assert(float_rates_map.size() == 1);
      //assert(float_rates_map["PLN"] == 1);
      /*
      for (const pair<string, float_rates_info> &p : float_rates_map) 
         std::cout << " 1 " << CURRENCY << " = " << p.second.rate << " " << p.second.code << " and "
            << " 1 " << p.second.code << " = " << p.second.inverse_rate << " " << CURRENCY << std::endl;*/
   }
   catch (const nlohmann::json::exception & e) {
      cerr << "!!! Error json exception: " << e.what() << '\n';
      throw e;
   } catch (const exception & e) {
      cerr << "Exception: " << e.what() << std::endl;
      throw e;
   }
}
//#endif

map<string, long double> Float_rates::rates(Rate_kind kind, bool & valid) const { 
   //std::cout << "\n" << __func__ << '\n';
   map<string, long double> result;
   for (const pair<string, float_rates_info> &p : float_rates_map) {
      switch (kind) {
         case RATE:
            result.insert({ p.second.code, p.second.rate});
            break;
         case INVERSE_RATE:
            result.insert({ p.second.code, p.second.inverse_rate});
      }
   }
   valid = true;
   //for (const auto& [code, rate] : result)
   //     std::cerr << "   " << code << " | " << rate << "\n";
   return result; 
}
