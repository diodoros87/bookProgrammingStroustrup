#include <string>
#include <map>

using std::string;
using std::map;

#include "nlohmann/json.hpp"

struct float_rates_info {
   string code;
   long double rate;
   long double inverse_rate;
};

class Float_rates {
public:   
   Float_rates(const string & JSON_DOCUMENT) { json_document = JSON_DOCUMENT; }
   
   void set_rates_from_json();
   void set_json_document(const string & JSON_DOCUMENT) {
      if (JSON_DOCUMENT != json_document) {
         json_document = JSON_DOCUMENT;
         cache_valid = false;
      }
   }
   
   map<string, long double> rates() const { return cache_valid ? rates_map : rates(RATE); }
   map<string, long double> inverse_rates() const { return cache_valid ? rates_map : rates(INVERSE_RATE); }
   map<string, float_rates_info> float_rates() const { return float_rates_map; }
private:
   map<string, float_rates_info> float_rates_map;
   string json_document;
   mutable map<string, long double> rates_map;
   mutable map<string, long double> inverse_rates_map;
   mutable bool cache_valid = true;
   
   enum Rate_kind{ RATE, INVERSE_RATE };
   
   map<string, float_rates_info> get_json_data() const;
   map<string, long double> rates(Rate_kind) const;
   
public:
   ~ Float_rates() = default;
   Float_rates(const Float_rates &) = default;
   Float_rates(Float_rates &&) = default;
   Float_rates & operator=(const Float_rates &) = default;
   Float_rates & operator=(Float_rates &&) = default;
};
