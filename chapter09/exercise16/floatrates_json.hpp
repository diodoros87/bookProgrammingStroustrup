#ifndef FLOAT_RATES_JSON_HPP
#define FLOAT_RATES_JSON_HPP

#include "floatrates.hpp"

#include "nlohmann/json.hpp"

class Float_rates_json : public Float_rates {
public:   
   using Float_rates::Float_rates;
   
   void set_rates_from_doc() override;
private:
   map<string, float_rates_info> get_data() const override;
   
public:
   ~ Float_rates_json() = default;
   Float_rates_json(const Float_rates_json &) = default;
   Float_rates_json(Float_rates_json &&) = default;
   Float_rates_json & operator=(const Float_rates_json &) = default;
   Float_rates_json & operator=(Float_rates_json &&) = default;
};

#endif
