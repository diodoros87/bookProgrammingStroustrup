#include "integer.hpp"
#include "money.hpp"


namespace money {
   
const Integer CENTS_PER_DOLLAR_INTEGER = Integer::create_Integer(CENTS_PER_DOLLAR);

template<>
Integer Money<Integer>::get_amount(const string & STR) {
   return Integer::parse_create(STR);
}

template <>
//template<typename Integer>
Integer Money<Integer>::calculate_by_Integer(const Integer & dollars, const long double cents /*  = INCORRECT_CENTS */) const {
   cerr << __func__ << '\n';
   Integer amount_in_cents = dollars * CENTS_PER_DOLLAR_INTEGER;
   cerr << __func__ << " amount_in_cents = " << amount_in_cents << '\n';
   if (INCORRECT_CENTS != cents) {
      cerr << __func__ << " cents = " << cents << '\n';
      Integer cents_round = dollars >= Integer::ZERO ? Money<Integer>::round(cents) : -Money<Integer>::round(cents);
      amount_in_cents += cents_round;
   }
   cerr << __func__ << " result = " << amount_in_cents << '\n';
   return amount_in_cents;
}

template<>
Money<Integer>::Money(const string & dollars, const long double cents, const string & currency /* = "PLN" */) {
   cerr << __func__ << " TYPE_NAME = '" << TYPE_NAME << "' " << dollars << '\n';
   validate_cents(cents);
   validate_currency(currency);
   if (! regex_match(dollars, INTEGER_REGEX)) 
      throw invalid_argument("Regex: dollars must be integer number ");
   string dollars_string = dollars;
   if (regex_match(dollars, MINUS_ZERO_REGEX))
      dollars_string = dollars.substr(1);
   Integer amount = Money<Integer>::get_amount(dollars_string);
   this->amount_in_cents = calculate_by_Integer(amount, cents);
   cerr << __func__ << " amount = '" << amount << "' std::signbit = '" << std::boolalpha << std::signbit(amount) << "' \n";  
   if (dollars[0] == '-' && Integer::ZERO == amount)
      this->amount_in_cents = -this->amount_in_cents;
   
   cerr << __func__ << " this->amount_in_cents = '" << TYPE_NAME << "' " << this->amount_in_cents << '\n';   
}

template<>
Money<Integer>::Money(const string & dollars, const string & currency /* = "PLN" */) {   // accept floating-point arguments
   cerr << __func__ << " TYPE_NAME = " << TYPE_NAME << ' ' << dollars << '\n';
   validate_currency(currency);
   if (! regex_match(dollars, FLOAT_POINT_REGEX))
      throw invalid_argument(string(__func__) +  " Regex: entered string '"
               + dollars + "' is not non-exponent floating-point format "); 
   this->amount_in_cents = calculate_amount_in_cents(dollars);
}

template<>
Money<Integer>::operator string() const {
   Integer dollars = get_dollars(TYPE_DEFAULT_OBJECT);
   Integer cents = get_cents(TYPE_DEFAULT_OBJECT);
   if (cents < Integer::ZERO) 
      cents = -cents;
   string out = (dollars == Integer::ZERO && signbit(get_amount_in_cents())) ? "-" : "";
   out += formatted_string(dollars, cents);
   out +=  " " + get_currency();
   return out;
}

Money<Integer> operator+(const Money<Integer>& a, const Money<Integer>& b) {
   const Integer sum = a.amount_in_cents + b.amount_in_cents;
   cerr << __func__ << " sum = " << sum << '\n';
   const Constructor_Args args {sum};
   Money<Integer> result = Money<Integer>(args.DOLLARS, args.CENTS);//::create
   cerr << __func__ << " result = " << result << '\n';
   return result;
}

Money<long double> operator+(const Money<long double>& a, const Money<long double>& b) {
   long double sum = a.amount_in_cents + b.amount_in_cents;
   sum = Money<long double>::round(sum);
   cerr << __func__ << " sum = " << sum << '\n';
   const string dollars_string = dollars_from_amount(sum);
   Money<long double> result = Money<long double>(dollars_string);//::create
   cerr << __func__ << " result = " << result << '\n';
   return result;
}
   
string formatted_string(const Integer & dollars, const Integer & cents) {
   ostringstream stream;
   stream << dollars;
   string dollars_string = stream.str();
   if (dollars > Integer::ZERO)
      dollars_string = dollars_string.substr(1);
   reset(stream);
   stream << cents;
   string cents_string = stream.str();
   if (! cents.is_zero())
      cents_string = cents_string.substr(1);
   cents_string = cents_string.length() == 1 ? ("0" + cents_string) : cents_string;
   string out = dollars_string + "," + cents_string;
   return out;
}

map <string, long double> get_by_asio(const File_format & format) {
   const string CURRENCY = "PLN";
   const string HOST = "www.floatrates.com";
   const Method METHOD = Method::get;
   const string DIRECTORY = "/daily/" + CURRENCY + ".json";
   //const string DIRECTORY = "/";            // "/" is root (main page of host) and "" has result 400 Bad Request
   const Cache_control CACHE_CONTROL = Cache_control::no_store;
   const Connection CONNECTION = Connection::close;
   const string DOC;// = get_document(HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION);
   
   //Float_rates floatrates { DOC };
   Float_rates floatrates = { DOC };
   
   floatrates.set_rates_from_json();
   map <string, long double> rates = floatrates.inverse_rates();
   return rates;
}

map <string, long double> get_by_curl(const File_format & format) {
   const string CURRENCY = "PLN";
   const string HOST = "www.floatrates.com";
   const Method METHOD = Method::get;
   const string DIRECTORY = "/daily/" + CURRENCY + ".json";
   //const string DIRECTORY = "/";            // "/" is root (main page of host) and "" has result 400 Bad Request
   const Cache_control CACHE_CONTROL = Cache_control::no_store;
   const Connection CONNECTION = Connection::close;
   const string DOC = R"( {"usd":{"code":"USD","alphaCode":"USD","numericCode":"840","name":"U.S. Dollar","rate":0.25054962245482,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":3.9912253317417},"eur":{"code":"EUR","alphaCode":"EUR","numericCode":"978","name":"Euro","rate":0.21965193489885,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":4.5526573688526},"gbp":{"code":"GBP","alphaCode":"GBP","numericCode":"826","name":"U.K. Pound Sterling","rate":0.18447112652335,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":5.4209025490686},"jpy":{"code":"JPY","alphaCode":"JPY","numericCode":"392","name":"Japanese Yen","rate":28.789036158552,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.034735445622168},"aud":{"code":"AUD","alphaCode":"AUD","numericCode":"036","name":"Australian Dollar","rate":0.35153549263367,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":2.8446629741654},"chf":{"code":"CHF","alphaCode":"CHF","numericCode":"756","name":"Swiss Franc","rate":0.23076416382752,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":4.3334284813279},"cad":{"code":"CAD","alphaCode":"CAD","numericCode":"124","name":"Canadian Dollar","rate":0.3179379091488,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":3.1452682150337},"twd":{"code":"TWD","alphaCode":"TWD","numericCode":"901","name":"New Taiwan Dollar ","rate":6.9630566546842,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.14361508883132},"tmt":{"code":"TMT","alphaCode":"TMT","numericCode":"934","name":"New Turkmenistan Manat","rate":0.87411557093163,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.1440134843202},"afn":{"code":"AFN","alphaCode":"AFN","numericCode":"971","name":"Afghan afghani","rate":24.532236377687,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.040762692182012},"srd":{"code":"SRD","alphaCode":"SRD","numericCode":"968","name":"Surinamese dollar","rate":5.1993963253223,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.19233002014672},"ghs":{"code":"GHS","alphaCode":"GHS","numericCode":"936","name":"Ghanaian Cedi","rate":1.5804113850311,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.63274664398873},"cup":{"code":"CUP","alphaCode":"CUP","numericCode":"192","name":"Cuban peso","rate":0.24665606776284,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":4.0542282582786},"dkk":{"code":"DKK","alphaCode":"DKK","numericCode":"208","name":"Danish Krone","rate":1.6395645304757,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.60991804922118},"inr":{"code":"INR","alphaCode":"INR","numericCode":"356","name":"Indian Rupee","rate":18.737966042633,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.053367585239763},"try":{"code":"TRY","alphaCode":"TRY","numericCode":"949","name":"Turkish Lira","rate":3.3965429181322,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.29441700696952},"ngn":{"code":"NGN","alphaCode":"NGN","numericCode":"566","name":"Nigerian Naira","rate":103.26775883873,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.00968356446625},"irr":{"code":"IRR","alphaCode":"IRR","numericCode":"364","name":"Iranian rial","rate":10402.299679955,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":9.613258902039e-5},"svc":{"code":"SVC","alphaCode":"SVC","numericCode":"222","name":"Salvadoran colon","rate":2.158133444329,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.46336337663814},"sbd":{"code":"SBD","alphaCode":"SBD","numericCode":"090","name":"Solomon Islands dollar","rate":1.9796864669636,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.50513049247326},"zmw":{"code":"ZMW","alphaCode":"ZMW","numericCode":"967","name":"Zambian kwacha","rate":4.4616283689676,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.22413341437297},"yer":{"code":"YER","alphaCode":"YER","numericCode":"886","name":"Yemeni rial","rate":61.69158312238,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.016209666690126},"lbp":{"code":"LBP","alphaCode":"LBP","numericCode":"422","name":"Lebanese Pound","rate":376.14197799458,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0026585705890407},"huf":{"code":"HUF","alphaCode":"HUF","numericCode":"348","name":"Hungarian Forint","rate":78.05375551727,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.012811683350441},"isk":{"code":"ISK","alphaCode":"ISK","numericCode":"352","name":"Icelandic Krona","rate":31.58214992198,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.031663455542779},"gip":{"code":"GIP","alphaCode":"GIP","numericCode":"292","name":"Gibraltar pound","rate":0.18323870706927,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":5.4573622352726},"gel":{"code":"GEL","alphaCode":"GEL","numericCode":"981","name":"Georgian lari","rate":0.75205344637308,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.3296927297158},"mkd":{"code":"MKD","alphaCode":"MKD","numericCode":"807","name":"Macedonian denar","rate":13.496578841998,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.074092850618426},"bif":{"code":"BIF","alphaCode":"BIF","numericCode":"108","name":"Burundian franc","rate":492.37098006397,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0020309889097649},"all":{"code":"ALL","alphaCode":"ALL","numericCode":"008","name":"Albanian lek","rate":26.515214628375,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.037714195944311},"mur":{"code":"MUR","alphaCode":"MUR","numericCode":"480","name":"Mauritian Rupee","rate":10.84721220272,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.092189585795075},"ves":{"code":"VES","alphaCode":"VES","numericCode":"928","name":"Venezuelan Bolivar","rate":1.1298994088668,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.88503453683801},"npr":{"code":"NPR","alphaCode":"NPR","numericCode":"524","name":"Nepalese Rupee","rate":29.777004732383,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.033582961382025},"ils":{"code":"ILS","alphaCode":"ILS","numericCode":"376","name":"Israeli New Sheqel","rate":0.79862091352987,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.2521585436325},"mro":{"code":"MRO","alphaCode":"MRO","numericCode":"478","name":"Mauritanian Ouguiya","rate":9.0973468015764,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.10992215882401},"cop":{"code":"COP","alphaCode":"COP","numericCode":"170","name":"Colombian Peso","rate":977.64084679998,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0010228705186298},"bzd":{"code":"BZD","alphaCode":"BZD","numericCode":"084","name":"Belize dollar","rate":0.49695140430582,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":2.0122691903786},"gnf":{"code":"GNF","alphaCode":"GNF","numericCode":"324","name":"Guinean franc","rate":2217.8674950203,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.00045088356371391},"szl":{"code":"SZL","alphaCode":"SZL","numericCode":"748","name":"Swazi lilangeni","rate":3.7650800976472,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.26559859925023},"sos":{"code":"SOS","alphaCode":"SOS","numericCode":"706","name":"Somali shilling","rate":142.6158515529,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0070118432776673},"aed":{"code":"AED","alphaCode":"AED","numericCode":"784","name":"U.A.E Dirham","rate":0.91680354378707,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.0907462201436},"php":{"code":"PHP","alphaCode":"PHP","numericCode":"608","name":"Philippine Peso","rate":12.791468991429,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.078177103870563},"myr":{"code":"MYR","alphaCode":"MYR","numericCode":"458","name":"Malaysian Ringgit","rate":1.047342811975,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.95479721497709},"kzt":{"code":"KZT","alphaCode":"KZT","numericCode":"398","name":"Kazakhstani Tenge","rate":108.22718352823,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0092398228189976},"pab":{"code":"PAB","alphaCode":"PAB","numericCode":"590","name":"Panamanian Balboa","rate":0.24860284510883,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":4.0224801110471},"gyd":{"code":"GYD","alphaCode":"GYD","numericCode":"328","name":"Guyanese dollar","rate":51.57831383768,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.019387993239699},"rwf":{"code":"RWF","alphaCode":"RWF","numericCode":"646","name":"Rwandan franc","rate":256.1496874888,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0039039672849249},"ern":{"code":"ERN","alphaCode":"ERN","numericCode":"232","name":"Eritrean nakfa","rate":3.7157989445365,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.26912112709175},"wst":{"code":"WST","alphaCode":"WST","numericCode":"882","name":"Samoan tala","rate":0.64637304044305,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.5470942279934},"cny":{"code":"CNY","alphaCode":"CNY","numericCode":"156","name":"Chinese Yuan","rate":1.5929140154749,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.62778027582479},"sar":{"code":"SAR","alphaCode":"SAR","numericCode":"682","name":"Saudi Riyal","rate":0.9391462837721,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.064796845049},"xof":{"code":"XOF","alphaCode":"XOF","numericCode":"952","name":"West African CFA Franc","rate":145.58929556569,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0068686368466478},"tjs":{"code":"TJS","alphaCode":"TJS","numericCode":"972","name":"Tajikistan Ruble","rate":2.8237787433341,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.35413539476512},"bob":{"code":"BOB","alphaCode":"BOB","numericCode":"068","name":"Bolivian Boliviano","rate":1.7080396616045,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.58546649851247},"nad":{"code":"NAD","alphaCode":"NAD","numericCode":"516","name":"Namibian dollar","rate":3.7650800976472,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.26559859925023},"syp":{"code":"SYP","alphaCode":"SYP","numericCode":"760","name":"Syrian pound","rate":621.00289860568,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0016102984418354},"mop":{"code":"MOP","alphaCode":"MOP","numericCode":"446","name":"Macanese pataca","rate":1.9800809839957,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.50502984882065},"bam":{"code":"BAM","alphaCode":"BAM","numericCode":"977","name":"Bosnia and Herzegovina convertible mark","rate":0.42709965516207,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":2.3413739344288},"idr":{"code":"IDR","alphaCode":"IDR","numericCode":"360","name":"Indonesian Rupiah","rate":3605.4776523964,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.00027735576154115},"tnd":{"code":"TND","alphaCode":"TND","numericCode":"788","name":"Tunisian Dinar","rate":0.71877674609514,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.391252576593},"pgk":{"code":"PGK","alphaCode":"PGK","numericCode":"598","name":"Papua New Guinean kina","rate":0.8779066604026,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.1390732581314},"uah":{"code":"UAH","alphaCode":"UAH","numericCode":"980","name":"Ukrainian Hryvnia","rate":7.0703515620449,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.14143568268489},"etb":{"code":"ETB","alphaCode":"ETB","numericCode":"230","name":"Ethiopian birr","rate":12.297087101102,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.081320071312693},"xcd":{"code":"XCD","alphaCode":"XCD","numericCode":"951","name":"East Caribbean Dollar","rate":0.6682390461827,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.4964704707282},"mwk":{"code":"MWK","alphaCode":"MWK","numericCode":"454","name":"Malawian kwacha","rate":201.41995495014,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0049647513834839},"gtq":{"code":"GTQ","alphaCode":"GTQ","numericCode":"320","name":"Guatemalan Quetzal","rate":1.8958302571438,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.52747338335371},"kwd":{"code":"KWD","alphaCode":"KWD","numericCode":"414","name":"Kuwaiti Dinar","rate":0.075379612428923,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":13.26618654272},"czk":{"code":"CZK","alphaCode":"CZK","numericCode":"203","name":"Czech Koruna","rate":5.3448440950188,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.18709619630102},"byn":{"code":"BYN","alphaCode":"BYN","numericCode":"933","name":"Belarussian Ruble","rate":0.64596614085766,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.548068755852},"rsd":{"code":"RSD","alphaCode":"RSD","numericCode":"941","name":"Serbian Dinar","rate":25.863893969986,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.038663938274741},"gmd":{"code":"GMD","alphaCode":"GMD","numericCode":"270","name":"Gambian dalasi","rate":13.125556641599,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.076187245029339},"awg":{"code":"AWG","alphaCode":"AWG","numericCode":"533","name":"Aruban florin","rate":0.4461428035423,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":2.2414347873823},"mmk":{"code":"MMK","alphaCode":"MMK","numericCode":"104","name":"Myanma Kyat","rate":438.29053276096,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0022815916047755},"mvr":{"code":"MVR","alphaCode":"MVR","numericCode":"462","name":"Maldivian rufiyaa","rate":3.8112951199427,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.26237800236656},"sek":{"code":"SEK","alphaCode":"SEK","numericCode":"752","name":"Swedish Krona","rate":2.2906677891366,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.43655391879279},"mad":{"code":"MAD","alphaCode":"MAD","numericCode":"504","name":"Moroccan Dirham","rate":2.3339656640348,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.42845531766361},"ron":{"code":"RON","alphaCode":"RON","numericCode":"946","name":"Romanian New Leu","rate":1.0905189909787,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.91699457622699},"rub":{"code":"RUB","alphaCode":"RUB","numericCode":"643","name":"Russian Rouble","rate":19.102196713525,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.052350000107157},"lyd":{"code":"LYD","alphaCode":"LYD","numericCode":"434","name":"Libyan Dinar","rate":1.144899112614,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.8734394052563},"clp":{"code":"CLP","alphaCode":"CLP","numericCode":"152","name":"Chilean Peso","rate":204.99004921508,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0048782855744904},"bsd":{"code":"BSD","alphaCode":"BSD","numericCode":"044","name":"Bahamian Dollar","rate":0.24665606776284,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":4.0542282582786},"djf":{"code":"DJF","alphaCode":"DJF","numericCode":"262","name":"Djiboutian franc","rate":43.889069206638,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.022784716515445},"sll":{"code":"SLL","alphaCode":"SLL","numericCode":"694","name":"Sierra Leonean leone","rate":2814.7440163429,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.00035527209372996},"kes":{"code":"KES","alphaCode":"KES","numericCode":"404","name":"Kenyan shilling","rate":28.006219002454,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.035706355074649},"bhd":{"code":"BHD","alphaCode":"BHD","numericCode":"048","name":"Bahrain Dinar","rate":0.093878133859629,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":10.652107779382},"omr":{"code":"OMR","alphaCode":"OMR","numericCode":"512","name":"Omani Rial","rate":0.095946614475718,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":10.422462589892},"mxn":{"code":"MXN","alphaCode":"MXN","numericCode":"484","name":"Mexican Peso","rate":5.1581670782937,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.19386731465294},"amd":{"code":"AMD","alphaCode":"AMD","numericCode":"051","name":"Armenia Dram","rate":120.7351642879,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0082825911232905},"htg":{"code":"HTG","alphaCode":"HTG","numericCode":"332","name":"Haitian gourde","rate":25.346561351835,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.039453083442721},"fjd":{"code":"FJD","alphaCode":"FJD","numericCode":"242","name":"Fiji Dollar","rate":0.52862557872371,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.8916980945461},"cdf":{"code":"CDF","alphaCode":"CDF","numericCode":"976","name":"Congolese franc","rate":493.59395815653,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0020259567271342},"mzn":{"code":"MZN","alphaCode":"MZN","numericCode":"943","name":"Mozambican metical","rate":15.796576117156,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.063304857494655},"ugx":{"code":"UGX","alphaCode":"UGX","numericCode":"800","name":"Ugandan shilling","rate":861.75597378063,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0011604213146477},"bdt":{"code":"BDT","alphaCode":"BDT","numericCode":"050","name":"Bangladeshi taka","rate":21.418592241677,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.046688409243544},"qar":{"code":"QAR","alphaCode":"QAR","numericCode":"634","name":"Qatari Rial","rate":0.90891273575728,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.1002156319956},"xaf":{"code":"XAF","alphaCode":"XAF","numericCode":"950","name":"Central African CFA Franc","rate":145.82638951932,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.006857469373659},"mdl":{"code":"MDL","alphaCode":"MDL","numericCode":"498","name":"Moldova Lei","rate":4.4913391326982,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.22265074412211},"uyu":{"code":"UYU","alphaCode":"UYU","numericCode":"858","name":"Uruguayan Peso","rate":10.896060578826,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.091776288573806},"lrd":{"code":"LRD","alphaCode":"LRD","numericCode":"430","name":"Liberian dollar","rate":37.636539309435,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.026569924290284},"sdg":{"code":"SDG","alphaCode":"SDG","numericCode":"938","name":"Sudanese pound","rate":109.18732283177,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0091585723879391},"top":{"code":"TOP","alphaCode":"TOP","numericCode":"776","name":"Tongan pa\u02bbanga","rate":0.56777407872517,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.7612639207576},"vuv":{"code":"VUV","alphaCode":"VUV","numericCode":"548","name":"Vanuatu vatu","rate":27.971528567341,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.035750638281798},"hkd":{"code":"HKD","alphaCode":"HKD","numericCode":"344","name":"Hong Kong Dollar","rate":1.9524721163762,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.51217120675505},"thb":{"code":"THB","alphaCode":"THB","numericCode":"764","name":"Thai Baht","rate":8.2872209885221,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.12066771254019},"vnd":{"code":"VND","alphaCode":"VND","numericCode":"704","name":"Vietnamese Dong","rate":5672.4018372617,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.00017629216488702},"uzs":{"code":"UZS","alphaCode":"UZS","numericCode":"860","name":"Uzbekistan Sum","rate":2708.4070546192,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.00036922071898111},"mga":{"code":"MGA","alphaCode":"MGA","numericCode":"969","name":"Malagasy ariary","rate":983.76696808819,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0010165008914086},"ttd":{"code":"TTD","alphaCode":"TTD","numericCode":"780","name":"Trinidad Tobago Dollar","rate":1.6707661640643,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.59852780209972},"lak":{"code":"LAK","alphaCode":"LAK","numericCode":"418","name":"Lao kip","rate":2798.8863035749,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.00035728496678223},"bwp":{"code":"BWP","alphaCode":"BWP","numericCode":"072","name":"Botswana Pula","rate":2.8470046927482,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.35124634762534},"jod":{"code":"JOD","alphaCode":"JOD","numericCode":"400","name":"Jordanian Dinar","rate":0.17693053206951,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":5.6519357529945},"bgn":{"code":"BGN","alphaCode":"BGN","numericCode":"975","name":"Bulgarian Lev","rate":0.43118168944819,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":2.3192079452162},"pen":{"code":"PEN","alphaCode":"PEN","numericCode":"604","name":"Peruvian Nuevo Sol","rate":0.96651348109575,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.0346467168428},"iqd":{"code":"IQD","alphaCode":"IQD","numericCode":"368","name":"Iraqi dinar","rate":361.64582777764,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0027651362830456},"nio":{"code":"NIO","alphaCode":"NIO","numericCode":"558","name":"Nicaraguan C\u00f3rdoba","rate":8.738827069209,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.11443183302293},"cve":{"code":"CVE","alphaCode":"CVE","numericCode":"132","name":"Cape Verde escudo","rate":24.175295322848,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.041364541224648},"aoa":{"code":"AOA","alphaCode":"AOA","numericCode":"973","name":"Angolan kwanza","rate":130.73745233804,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0076489175987183},"khr":{"code":"KHR","alphaCode":"KHR","numericCode":"116","name":"Cambodian riel","rate":1002.6282924007,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0009973785974118},"nok":{"code":"NOK","alphaCode":"NOK","numericCode":"578","name":"Norwegian Krone","rate":2.1963869205368,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.45529318657369},"lkr":{"code":"LKR","alphaCode":"LKR","numericCode":"144","name":"Sri Lanka Rupee","rate":50.277392745173,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.019889655079539},"hrk":{"code":"HRK","alphaCode":"HRK","numericCode":"191","name":"Croatian Kuna","rate":1.6680299712416,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.59950961148238},"dzd":{"code":"DZD","alphaCode":"DZD","numericCode":"012","name":"Algerian Dinar","rate":34.964646564318,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.02860031769978},"ars":{"code":"ARS","alphaCode":"ARS","numericCode":"032","name":"Argentine Peso","rate":26.164235298146,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.038220111866631},"stn":{"code":"STN","alphaCode":"STN","numericCode":"930","name":"S\u00e3o Tom\u00e9 and Pr\u00edncipe Dobra","rate":5.4059011848155,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.18498303350584},"xpf":{"code":"XPF","alphaCode":"XPF","numericCode":"953","name":"CFP Franc","rate":26.079546386232,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.038344225209682},"hnl":{"code":"HNL","alphaCode":"HNL","numericCode":"340","name":"Honduran Lempira","rate":6.0696679155104,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.16475365933029},"scr":{"code":"SCR","alphaCode":"SCR","numericCode":"690","name":"Seychelles rupee","rate":3.2555853137913,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.3071644277801},"dop":{"code":"DOP","alphaCode":"DOP","numericCode":"214","name":"Dominican Peso","rate":14.362763571993,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.069624483824964},"nzd":{"code":"NZD","alphaCode":"NZD","numericCode":"554","name":"New Zealand Dollar","rate":0.37642061702181,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":2.6566026269014},"krw":{"code":"KRW","alphaCode":"KRW","numericCode":"410","name":"South Korean Won","rate":300.81038473528,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.00332435331606},"azn":{"code":"AZN","alphaCode":"AZN","numericCode":"944","name":"Azerbaijan Manat","rate":0.42482302226406,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":2.3539213921848},"crc":{"code":"CRC","alphaCode":"CRC","numericCode":"188","name":"Costa Rican Col\u00f3n","rate":160.75336594194,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0062207095580267},"bnd":{"code":"BND","alphaCode":"BND","numericCode":"096","name":"Brunei Dollar","rate":0.3324204207993,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":3.0082387766537},"kmf":{"code":"KMF","alphaCode":"KMF","numericCode":"174","name":"\tComoro franc","rate":107.93011489996,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0092652546597109},"lsl":{"code":"LSL","alphaCode":"LSL","numericCode":"426","name":"Lesotho loti","rate":3.7650800976472,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.26559859925023},"tzs":{"code":"TZS","alphaCode":"TZS","numericCode":"834","name":"Tanzanian shilling","rate":570.05429590883,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0017542188650744},"bbd":{"code":"BBD","alphaCode":"BBD","numericCode":"052","name":"Barbadian Dollar","rate":0.50079685684405,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":1.9968176443875},"ang":{"code":"ANG","alphaCode":"ANG","numericCode":"532","name":"Neth. Antillean Guilder","rate":0.44528063873599,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":2.2457747160054},"pkr":{"code":"PKR","alphaCode":"PKR","numericCode":"586","name":"Pakistani Rupee","rate":43.910066683762,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.022773821028375},"zar":{"code":"ZAR","alphaCode":"ZAR","numericCode":"710","name":"South African Rand","rate":3.8318598526035,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.26096987845747},"kgs":{"code":"KGS","alphaCode":"KGS","numericCode":"417","name":"Kyrgyzstan Som","rate":21.208675279622,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.047150516796343},"pyg":{"code":"PYG","alphaCode":"PYG","numericCode":"600","name":"Paraguayan Guaran\u00ed","rate":1743.9434323171,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.00057341309440947},"jmd":{"code":"JMD","alphaCode":"JMD","numericCode":"388","name":"Jamaican Dollar","rate":38.511807665468,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.025966062374596},"ssp":{"code":"SSP","alphaCode":"SSP","numericCode":"728","name":"South Sudanese pound","rate":106.50708948109,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.0093890463524267},"mru":{"code":"MRU","alphaCode":"MRU","numericCode":"929","name":"Mauritanian ouguiya","rate":8.9513931330548,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.11171445440233},"mnt":{"code":"MNT","alphaCode":"MNT","numericCode":"496","name":"Mongolian togrog","rate":706.18666508108,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.001416056192289},"brl":{"code":"BRL","alphaCode":"BRL","numericCode":"986","name":"Brazilian Real","rate":1.3269317928557,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.75361823824258},"egp":{"code":"EGP","alphaCode":"EGP","numericCode":"818","name":"Egyptian Pound","rate":3.9282240191533,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":0.25456796636958},"sgd":{"code":"SGD","alphaCode":"SGD","numericCode":"702","name":"Singapore Dollar","rate":0.33701260245423,"date":"Fri, 4 Feb 2022 11:55:01 GMT","inverseRate":2.967248087216}} )" ;
   // = get_document(HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION);
   //const string DOC ;
   //Float_rates floatrates { DOC };
   Float_rates floatrates = { DOC };
   floatrates.set_rates_from_json();
   map <string, long double> RES;
   
   map <string, long double> rates = floatrates.inverse_rates();
   //rates["PLN"] = 1;
   rates.insert({{"PLN", 1}});
   return rates;
}

//#if defined(__clang__)
//#elif defined(__GNUG__)
map <string, long double> & set_rates_per_PLN(const Network_library & library, const File_format & format) {
   /*static map<string, long double> m = { { "PLN", 1 } };
   if (true)
      return m;*/
   //cerr << " Network_library::ASIO = " << static_cast<int>(library) << '\n';
   //cerr << " Network_library::CURL = " << static_cast<int>(format) << '\n';
/*   if (format == File_format::JSON)
            return get_by_asio(format);
      case Network_library::CURL :
         if (format == File_format::XML)
            return get_by_curl(format);
   throw invalid_argument(__func__ + string(" Invalid network library ") + std::to_string(static_cast<int>(library)));      
   cerr << __func__ << '\n';
   if (library == Network_library::ASIO && format == File_format::JSON)
      return get_by_asio(format);
   else if (library == Network_library::CURL && format == File_format::XML)
      return get_by_curl(format);
   else 
      throw invalid_argument(__func__ + string(" Invalid network library ") + std::to_string(static_cast<int>(library))
         + " Invalid file format " + std::to_string(static_cast<int>(format)));
      
   cerr << __func__ << '\n';*/
   //map <string, long double> result;
   
   //cerr << "fff" << '\n';
   /*
   static map <string, long double> result = { {"PLN", 1} };
   cerr << __func__ << '\n';
   return result;
   */
   static map <string, long double> result;
   switch (library) {
      case Network_library::ASIO :
         if (format == File_format::JSON) {
            result = get_by_asio(format);
            return result;
         }
      case Network_library::CURL :
         if (format == File_format::XML) {
            result = get_by_curl(format);
            return result;
         }
      default:
         throw invalid_argument(__func__ + string(" Invalid network library ") + std::to_string(static_cast<int>(library)));
   }
   throw invalid_argument(__func__ + string(" Invalid file format "));
}
//#endif

#ifndef DEBUG_OSTREAM
   ostream& operator<<(ostream& os, const Money<Integer>& money) {
      Integer dollars = money.get_dollars(Money<Integer>::TYPE_DEFAULT_OBJECT);
      Integer cents = money.get_cents(Money<Integer>::TYPE_DEFAULT_OBJECT);
      start_settings(os, money);
      os << dollars.string_without_signum() << ",";
      os << setw(2) << cents.string_without_signum();
      os << ' ' << money.get_currency();
      return os;
   }
#endif

}
