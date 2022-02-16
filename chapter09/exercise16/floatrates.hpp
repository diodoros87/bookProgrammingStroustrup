#ifndef FLOAT_RATES_HPP
#define FLOAT_RATES_HPP

#include <string>
#include <map>
#include <iostream>

using std::string;
using std::map;

struct float_rates_info {
   string code;
   long double rate;
   long double inverse_rate;
};

class Float_rates {
public:   
   Float_rates(const string & DOCUMENT, bool setting_rates = true) { 
      document = DOCUMENT;
      if (setting_rates)
         set_rates_from_doc();
   }
   
   virtual void set_rates_from_doc() = 0;
   void set_document(const string & DOCUMENT) {
      if (DOCUMENT != document) {
         document = DOCUMENT;
         rates_valid = false;
         inverse_valid = false;
      }
   }
   
   map<string, long double> rates() const {  
      return get_rates(RATE, rates_map, rates_valid); }
      
   map<string, long double> inverse_rates() const { 
      map<string, long double> res = get_rates(INVERSE_RATE, inverse_rates_map, inverse_valid);
      //std::cout << "\n" << __func__ << '\n';
      //for (const auto& [code, rate] : res)
      //  std::cerr << "   " << code << " | " << rate << "\n";
      return res; }
      //return get_rates(INVERSE_RATE, inverse_rates_map, inverse_valid); }
      
   map<string, float_rates_info> float_rates() const { return float_rates_map; }
protected:
   string document;
   map<string, float_rates_info> float_rates_map;
private:
   mutable map<string, long double> rates_map;
   mutable map<string, long double> inverse_rates_map;
   mutable bool rates_valid = false;
   mutable bool inverse_valid = false;
   
   enum Rate_kind{ RATE, INVERSE_RATE };
   
   map<string, long double> get_rates(Rate_kind kind, map<string, long double>& a_rates, bool & valid ) const { 
      return valid ? a_rates : a_rates = rates(kind, valid); 
   }
     
   virtual map<string, float_rates_info> get_data() const = 0;
   
   map<string, long double> rates(Rate_kind, bool & valid) const;
   
public:
   virtual ~Float_rates() { };
protected:
   Float_rates(const Float_rates &) = default;
   Float_rates(Float_rates &&) = default;
   Float_rates & operator=(const Float_rates &) = default;
   Float_rates & operator=(Float_rates &&) = default;
};

#endif
