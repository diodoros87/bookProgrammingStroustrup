#include "floatrates_downloader.hpp"

using namespace std;
using namespace network;

void view_document(const string & CURRENCY, const string & JSON_DOC) {
   Float_rates floatrates = { "", false };
   floatrates.set_json_document(JSON_DOC);
   floatrates.set_rates_from_json();
   map <string, float_rates_info> rates = floatrates.float_rates();
   for (const pair<string, float_rates_info> &p : rates) 
      cout << " 1 " << CURRENCY << " = " << p.second.rate << " " << p.second.code << " and "
         << " 1 " << p.second.code << " = " << p.second.inverse_rate << " " << CURRENCY << endl;
}

int main() {
   try {
      const string CURRENCY = "PLN";
      Floatrates_downloader downloder(File_format::JSON, CURRENCY);
      const string DOC = downloder.get_by_asio();
      view_document(CURRENCY, DOC);
      return 0;
   }
   catch (const nlohmann::json::exception & e) {
      cerr << "!!! Error json exception: " << e.what() << '\n';
   }
   catch (const Asio_IO_Stream_Exception & e) {
      cerr << e.what() << endl;
   }
   catch (const asio::system_error &e) {
      cerr << "!!! System Error ! Error code = " << e.code()
           << "\n Message: " << e.what();
      return e.code().value();
   }
   catch (const exception & e) {
      cerr << "Exception: " << e.what() << endl;
   }
   catch (...) {
      cerr << "Unrecognized Exception: " <<  endl;
   }
   return 1;
}
