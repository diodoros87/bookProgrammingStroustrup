#include "floatrates_xml.hpp"

#include <iostream>
#include <sstream>

using std::cerr;
using std::pair;
using std::exception;

using namespace pugi;
using namespace xml_processing;

map<string, float_rates_info> get_data(const xpath_node_set & INVERSE_RATES, const xpath_node_set & CODES, const xpath_node_set & RATES) {
   const size_t SIZE = INVERSE_RATES.size();
   if (SIZE != CODES.size() || SIZE != RATES.size()) {
      throw Xml_processing::Exception(
         "Different size of xpath node sets:\nINVERSE_RATES = " + std::to_string(SIZE)
         + "\nCODES = " + std::to_string(CODES.size()) + "\nRATES = " + std::to_string(RATES.size()));
   }
   map<string, float_rates_info> result; 
   for (size_t i = 0; i < SIZE; i++) {
      xpath_node  code_xpathnode = CODES[i];
      xml_node    code_xmlnode   = code_xpathnode.node();
      xml_text    code_xmltext   = code_xmlnode.text();
      xpath_node  rate_xpathnode = RATES[i];
      xml_node    rate_xmlnode   = rate_xpathnode.node();
      xml_text    rate_xmltext   = rate_xmlnode.text();
      xpath_node  inverse_xpathnode = INVERSE_RATES[i];
      xml_node    inverse_xmlnode   = rate_xpathnode.node();
      xml_text    inverse_xmltext   = rate_xmlnode.text();
      const double rate                = std::stod(rate_xmltext.as_string());
      const double inverse_rate        = std::stod(inverse_xmltext.as_string());
      std::cout << " 1 PLN = " << inverse_rate << " " << code_xmltext.get() << " and"
         << " 1 " << code_xmltext.as_string() << " = " << rate << " PLN\n";
      result[code_xmltext.get()] = { code_xmltext.as_string(), rate, inverse_rate };
   }
   return result;
}

map<string, float_rates_info> Float_rates_xml::get_data() const {
   //document = "";
   xml_document xml_doc;
   Xml_processing::load_validate_xml_throw(xml_doc, document);
   try {
      const xpath_node_set INVERSE_RATES = xml_doc.select_nodes(INVERSE_RATE_NODE);
      const xpath_node_set CODES = xml_doc.select_nodes(CODE_NODE);
      const xpath_node_set RATES = xml_doc.select_nodes(RATE_NODE);
      
      return ::get_data(INVERSE_RATES, CODES, RATES);
   }
   catch (pugi::xpath_exception const & e) {
      cerr << "!!! Error xpath_exception exception: "  << typeid(e).name() << e.result().description() << '\n';
      throw;
   }
}

void Float_rates_xml::set_rates_from_doc() {
   try {
      float_rates_map = get_data();
   }
   catch (const Xml_processing::Exception & e) {
      cerr << "!!! Error xml exception: "  << typeid(e).name() << " : " << e.what() << '\n';
      throw;
   } catch (const exception & e) {
      cerr << "Exception: " << e.what() << std::endl;
      throw;
   }
}
