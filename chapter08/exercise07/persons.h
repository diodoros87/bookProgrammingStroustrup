using std::string;
using std::vector;

void validate_name (const string& s);
void validate_age (double age);

void print(const string& LABEL);
void print_sorted(const string& LABEL);

extern vector<string> age_ordered_names;
extern vector<double> global_vec_age;
