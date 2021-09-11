#include <string>
#include <stdexcept>

 void error(const std::string& errormessage) {
   throw std::runtime_error("!!!!! Error: " + errormessage);
}

 void error(const std::string& s, const std::string& s2) {
	error(s + s2);
}
