#include "error.h"
using std::string;
using std::ostream;

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
   
   static string status(const Patron& patron);
   
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
   return os << " User: " << patron.get_user_name() << "\t Card number: " 
           << patron.get_number() << "\t Charges: " << patron.get_charges() << '\n';
}

inline bool operator==(const Patron& a, const Patron& b) {
   return a.get_user_name() == b.get_user_name() && a.get_number() == b.get_number() 
      && a.get_charges() == b.get_charges();
}

inline bool operator!=(const Patron& a, const Patron& b) {
   return !(a==b);
}
