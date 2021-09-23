using std::string;
using std::vector;

class Name_pairs {
   vector<string> vec_names;
   vector<double> vec_age;

   int get_min_name_index(unsigned int begin, unsigned int end);
public:
   vector<string> get_names() const { return vec_names; }
   vector<double> get_ages()  const { return vec_age; }
   bool size_equal() const { return vec_age.size() == vec_names.size(); }
   void sort();
   void read_ages(unsigned int n);
   void read_names();
   void read_names(unsigned int n);
   unsigned int get_names_size() const { return vec_names.size(); }
};

std::ostream& operator<<(std::ostream& os, const Name_pairs& pairs);
bool operator==(const Name_pairs& a, const Name_pairs& b);
bool operator!=(const Name_pairs& a, const Name_pairs& b);
