#include "floatrates_test.hpp"

using namespace std;

namespace downloaders_test {
   extern void downloaders_test();
}

namespace float_rates_test {
   
inline static bool assert_floatrates(const Float_rates & a1, const Float_rates & a2) {
   return (a1.get_document() == a2.get_document()) && (a1.float_rates() == a2.float_rates()) &&
   (a1.inverse_rates() == a2.inverse_rates()) && (a1.rates() == a2.rates());
}

static void validate_format(const File_format & FORMAT) { 
   switch (FORMAT) {
      case File_format::JSON :
      case File_format::XML :
         break;
      default:
         throw invalid_argument(__func__ + string(" Invalid file format ") + std::to_string(static_cast<int>(FORMAT)));
   }
}

template <class Document, enable_if_t<is_base_of<Float_rates, Document>::value, bool>  = true>
Document * get_check_rates_ptr(Float_rates * rates_ptr) {
   if (rates_ptr == nullptr)
      throw invalid_argument(string(__func__) + " Type of rates_ptr can not be null ");
   Document * document_ptr = dynamic_cast<Document *>(rates_ptr);
   if (document_ptr == nullptr)
      throw invalid_argument(string(__func__) + " Type of rates_ptr is " + typeid(document_ptr).name()
         + " Required type: " + typeid(Document).name());
   return document_ptr;
}

template <class Document, enable_if_t<is_base_of<Float_rates, Document>::value, bool>  = true>
Document create_rates(const File_format & format, const string & currency) {
   //validate_format();
   Float_rates_test test { format, currency };
   test.run();
   Float_rates * rates_ptr = test.get_float_rates();
   Document * doc_ptr = get_check_rates_ptr<Document>(rates_ptr);
   Document & doc_ref = *doc_ptr;
   assert(assert_floatrates(doc_ref, *doc_ptr));
   Document result(doc_ref);
   assert(assert_floatrates(doc_ref, result));
   assert(assert_floatrates(*doc_ptr, result));
   return result;
}

template <class Document, enable_if_t<is_base_of<Float_rates, Document>::value, bool>  = true>
constexpr File_format get_format() {
   /*
   if (is_same<Document, Float_rates_json>::value)
      return File_format::JSON;
   if (is_same<Document, Float_rates_xml>::value)
      return File_format::XML;
   */
   return is_same<Document, Float_rates_json>::value ? File_format::JSON : File_format::XML; 
   //static_assert(false && "Not supported type");
}

template <class Document, enable_if_t<is_base_of<Float_rates, Document>::value, bool>  = true>
static void constructors_test() {
   Document * document_ptr = nullptr;
#ifdef __clang__
   Document float_rates_doc { "", false };
#elif defined(__GNUG__)
   Document float_rates_doc { "" };
#endif
   //static const File_format format = is_same<Document, Float_rates_json>::value ? File_format::JSON : File_format::XML; 
   static constexpr File_format format = get_format<Document>();
   string currency { "pln" };
   Document object = create_rates<Document>(format, currency);
   //document_ptr = get_check_rates_ptr<Document>(rates_ptr);
   Document & obj_ref = object;
   assert(assert_floatrates(obj_ref, object));
   Document obj1(obj_ref);
   assert(assert_floatrates(obj_ref, obj1));
   assert(assert_floatrates(object, obj1));
   Document obj2 = obj1;
   assert(assert_floatrates(obj2, obj1));
   Document obj3(move(obj2));
   assert( ! assert_floatrates(obj3, obj2));
   assert(assert_floatrates(obj3, obj1));
   assert(assert_floatrates(obj3, obj_ref));
   assert(assert_floatrates(obj3, object));
   currency = { "usd" };
   Document other = create_rates<Document>(format, currency);
   assert( ! assert_floatrates(other, obj2));
   assert( ! assert_floatrates(other, obj1));
   assert( ! assert_floatrates(other, obj_ref));
   assert( ! assert_floatrates(other, object));
   assert( ! assert_floatrates(other, obj3));
   
   obj1 = other;
   assert( assert_floatrates(other, obj1));
   assert( ! assert_floatrates(obj1, obj2));
   assert( ! assert_floatrates(obj1, obj_ref));
   assert( ! assert_floatrates(obj1, object));
   assert( ! assert_floatrates(obj1, obj3));
   
   obj3 = move(other);
   assert( assert_floatrates(other, obj2));
   assert( assert_floatrates(obj3, obj1));
   assert( ! assert_floatrates(obj3, obj2));
   assert( ! assert_floatrates(obj3, obj_ref));
   assert( ! assert_floatrates(obj3, object));
   assert( ! assert_floatrates(obj3, other));
   
   /*
   assert(dynamic_cast<Float_rates_json *>(rates_ptr))
   Document = * rates_ptr;
   
   Document j1("www.floatrates.com", Method::get, "/daily/" + currency + ".json", Cache_control::no_store, Connection::close);
   Json_downloader j2(j1);
   assert(assert_json(j1, j2));
   
   Json_downloader j3("https://github.com", Method::get, "/diodoros87/bookProgrammingStroustrup/blob/master/chapter02/exercise01/compile.sh", Cache_control::no_store, Connection::close);
   Json_downloader j4 = j3;
   assert(assert_json(j3, j4));
   assert( ! assert_json(j3, j2));
   assert( ! assert_json(j3, j1));
   
   j3 = j2;
   assert(assert_json(j3, j1));
   assert(assert_json(j3, j2));
   assert( ! assert_json(j3, j4));
   
   j4 = std::move(j1);
   assert(assert_json(j4, j2));
   assert(assert_json(j4, j3));
   assert(nullptr == j1.get_asio_downloader());
   
   j1 = std::move(j4);
   assert(assert_json(j1, j2));
   assert(assert_json(j1, j3));
   assert(nullptr == j4.get_asio_downloader());
   
   Json_downloader j5 (std::move(Json_downloader("https://github.com", Method::get, "/diodoros87/bookProgrammingStroustrup/blob/master/chapter02/exercise01/compile.sh", Cache_control::no_store, Connection::close)));
   Json_downloader j6 (std::move(j2));
   assert(assert_json(j6, j1));
   assert(assert_json(j6, j3));
   assert(nullptr == j2.get_asio_downloader());
   
   j6 = (std::move(j6));
   assert(assert_json(j6, j1));
   assert(assert_json(j6, j3));
   assert(nullptr != j6.get_asio_downloader());
   
   j6 = j6;
   assert(assert_json(j6, j1));
   assert(assert_json(j6, j3));
   assert(nullptr != j6.get_asio_downloader());
   */
}

void test() {
   Float_rates_test test { File_format::JSON, "PLN" };
   test.run();
   //test.set_format(File_format::XML);
   //test.run();
   
   downloaders_test::downloaders_test();
   
   constructors_test<Float_rates_json>();
}

}
