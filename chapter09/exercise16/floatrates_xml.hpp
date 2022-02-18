#ifndef FLOAT_RATES_XML_HPP
#define FLOAT_RATES_XML_HPP

#include "floatrates.hpp"

#include "nlohmann/json.hpp"

class Float_rates_xml : public Float_rates {
public:   
   using Float_rates::Float_rates;
   
   void set_rates_from_doc() override;
private:
   map<string, float_rates_info> get_data() const override;
   
public:
   ~ Float_rates_xml() = default;
   Float_rates_xml(const Float_rates_xml &) = default;
   Float_rates_xml(Float_rates_xml &&) = default;
   Float_rates_xml & operator=(const Float_rates_xml &) = default;
   Float_rates_xml & operator=(Float_rates_xml &&) = default;
   
   Float_rates_xml * clone() const override { return new Float_rates_xml(*this); }
};

#endif
