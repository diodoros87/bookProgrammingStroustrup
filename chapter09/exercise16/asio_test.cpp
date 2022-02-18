

#include "floatrates_test.hpp"

using namespace std;
using namespace float_rates_test;

/*
void view_document(const File_format & format, const string & CURRENCY, const string & JSON_DOC) {
   using Downloader = std::conditional_t<FLAG, Float_rates_json, Float_rates_xml>;
#ifdef __clang__
   Downloader  float_rates = {"", false };
#elif defined(__GNUG__)
   Downloader  float_rates = {""};
#endif
   float_rates.set_document(JSON_DOC);
   float_rates.set_rates_from_doc();
   map <string, float_rates_info> rates = float_rates.float_rates();
   for (const pair<string, float_rates_info> &p : rates) 
      cout << " 1 " << CURRENCY << " = " << p.second.rate << " " << p.second.code << " and "
         << " 1 " << p.second.code << " = " << p.second.inverse_rate << " " << CURRENCY << endl;
}
*/

int main() {
   try {
      float_rates_test::test();
      return 0;
   }
   catch (const nlohmann::json::exception & e) {
      cerr  << __func__ << " " << typeid(e).name() << "  Error json exception: " << e.what() << '\n';
   }
   catch (const Asio_IO_Stream_Exception & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
   }
   catch (const asio::system_error &e) {
      cerr << "!!! System Error ! Error code = " << e.code()
           << "\n Message: " << e.what();
      return e.code().value();
   } 
   catch (const std::invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
   catch (const std::bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } 
   catch (const std::bad_alloc & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
   }
   catch (const exception & e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
   catch (...) {
      cerr << __func__ << " " << "Unrecognized Exception: " <<  endl;
   }
   return 1;
}
