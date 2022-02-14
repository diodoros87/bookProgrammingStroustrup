#include "floatrates.hpp"

#include <iostream>
#include <sstream>

using std::cerr;
using std::pair;
using std::exception;
/*
void to_json(nlohmann::json& j, float_rates_info& info) {
   j = nlohmann::json {{"code", info.code}, {"rate", info.rate}, {"inverseRate", info.inverse_rate}};
}
*/
void from_json(const nlohmann::json& json_data, float_rates_info& info) {
   info.code = json_data.at("code").get<string>();
   info.rate = json_data.at("rate").get<long double>();
   info.inverse_rate = json_data.at("inverseRate").get<long double>();
}

map<string, float_rates_info> Float_rates::get_json_data() const {
   std::stringstream strstream;
   strstream.str(json_document);
   
   nlohmann::json json_data;
   strstream >> json_data;
   return json_data;
}

void Float_rates::set_rates_from_json() {
   try {
      float_rates_map = get_json_data();
   }
   catch (const nlohmann::json::exception & e) {
      cerr << "!!! Error json exception: " << e.what() << '\n';
      throw;
   } catch (const exception & e) {
      cerr << "Exception: " << e.what() << std::endl;
      throw;
   }
}

map<string, long double> Float_rates::rates(Rate_kind kind, bool & valid) const { 
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
   return result; 
}
