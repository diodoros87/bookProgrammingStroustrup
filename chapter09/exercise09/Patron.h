#include "error.h"
using std::string;
using std::ostream;

namespace User {
   
class Patron {
public:
   class Invalid_Patron : public Invalid { 
      string msg {string(Invalid::what()) + " Invalid patron: "};
   public:
      Invalid_Patron(const string& message) { msg += message; }
      const char* what() {
         return msg.c_str();
      }
   };
   
   string get_user_name() const { return user_name; }
   unsigned int get_number() const { return number; }
   unsigned int get_charges() const { return charges; }

   bool are_charges() const { return charges > 0; }
   
   void set_charges(unsigned int charges) { this->charges = charges; };
   
   Patron(const string& name, unsigned int number, unsigned int charges);
   Patron(const string& name, unsigned int number);
private:
   Patron();
   
   const string user_name;
   const unsigned int number {0};
   unsigned int charges {0};
};

inline ostream& operator<<(ostream& os, const Patron& patron) {
   return os << " User: " << patron.get_user_name() << " Card number: " 
           << patron.get_number() << " Charges: " << patron.get_charges() << '\n';
}

}
