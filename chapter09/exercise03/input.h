using std::string;

namespace Input {
   string get_string (const string& PROMPT);
   string get_string ();
   void   read_string (const string& PROMPT, string& s);
   void   read_string (string& s);
   struct End_of_data {};
   double get_double ();
   double get_double (const string& PROMPT);
   //static void check_input();
};

//extern struct End_of_data;
