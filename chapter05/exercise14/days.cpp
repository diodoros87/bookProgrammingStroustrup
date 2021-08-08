#include <iostream>
#include <vector>

using namespace std;

vector<string> MONDAY = {"Monday", "Mon", "monday", "mon"};
constexpr int MONDAY_NO = 1;
vector<string> TUESDAY = {"Tuesday", "Tue", "tuesday", "tue"};
constexpr int TUESDAY_NO = 2;
vector<string> WEDNESDAY = {"Wednesday", "Wed", "wednesday", "wed"};
constexpr int WEDNESDAY_NO = 3;
vector<string> THURSDAY = {"Thursday", "Thu", "thursday", "thu"};
constexpr int THURSDAY_NO = 4;
vector<string> FRIDAY = {"Friday", "Fri", "friday", "fri"};
constexpr int FRIDAY_NO = 5;
vector<string> SATURDAY = {"Saturday", "Sat", "saturday", "sat"};
constexpr int SATURDAY_NO = 6;
vector<string> SUNDAY = {"Sunday", "Sun", "sunday", "sun"};
constexpr int SUNDAY_NO = 7;

vector<int> mon, tue, wed, thu, fri, sat, sun;

void check_preconditions(vector<int>& vec) {
	const unsigned int size = vec.size();
	if (0 == size)
		throw runtime_error("Precondition: Vector can not be empty");
}

void print(vector<int>& vec, string title) {
	cout << title << endl;
	for (unsigned int i = 0; i < vec.size(); i++)
		cout << vec[i] << "\t";
	
	cout << endl;		
}

int calculate_sum(vector<int>& vec) {
	check_preconditions(vec);
	
	int sum = 0;
	for (unsigned int i = 0; i < vec.size(); i++)
		sum += vec[i];
			
	return sum;
}

bool exist(string s, vector<string> vec) {
	for (unsigned int i = 0; i < vec.size(); i++)
		if (s == vec[i])
			return true;
			
	return false;
}

int get_number_of_day(string s) {
	if (exist(s, MONDAY))
		return MONDAY_NO;
	else if (exist(s, TUESDAY))
		return TUESDAY_NO;
	else if (exist(s, WEDNESDAY))
		return WEDNESDAY_NO;
	else if (exist(s, THURSDAY))
		return THURSDAY_NO;
	else if (exist(s, FRIDAY))
		return FRIDAY_NO;
	else if (exist(s, SATURDAY))
		return SATURDAY_NO;
	else if (exist(s, SUNDAY))
		return SUNDAY_NO;
			
	throw runtime_error("Entered string is unrecognized name (or shortcut) of day");
}

class End_Of_Data {};

int get_number() {
	int number;
	cin >> number;
	
	if(!cin)
		throw End_Of_Data();
	
	return number;
}

string get_day() {
	string day;
	cin >> day;
	
	if(!cin)
		throw End_Of_Data();
	
	return day;
}

void save_number(int day_number, int number) {
	switch(day_number) {
		case MONDAY_NO:
			mon.push_back(number);
			break;
		case TUESDAY_NO:
			tue.push_back(number);
			break;
		case WEDNESDAY_NO:
			wed.push_back(number);
			break;
		case THURSDAY_NO:
			thu.push_back(number);
			break;
		case FRIDAY_NO:
			fri.push_back(number);
			break;
		case SATURDAY_NO:
			sat.push_back(number);
			break;
		case SUNDAY_NO:
			sun.push_back(number);
			break;
		default:
			throw runtime_error("Unrecognized number of day");
	}
}

bool save_number(string day) {
	try {
	   int number_of_day = get_number_of_day(day);
	   int number = get_number();
	   save_number(number_of_day, number);
	   return true;
	}
	catch(runtime_error& e) {
		cout << e.what() << endl;
		return false;
	}
}

void get_records() {
	const string quit = "Q";
	cout << "Enter (name or shortcut) day of week and then number alternately.\n";
	cout << "To end of data enter " << quit << " as day's name.\n";
	cout << "To end of data enter non-integer as number.\n";
	
	string day = get_day();	
	while(day != quit) {
		save_number(day);
		day = get_day();
	}
}

void print_sum(vector<int>& vec, string title) {
	if(vec.size() > 0) {
		int sum = calculate_sum(vec);
		cout << title << " - sum: " << sum << endl;
	}
	else
		cout << title << " is empty " << endl;
}

void print_results() {
	const string separation = "-----------------";
	print(mon, "Monday: ");
	print_sum(mon, "Monday");
	cout << separation << endl;
	print(tue, "Tuesday: ");
	print_sum(tue, "Tuesday");
	cout << separation << endl;
	print(wed, "Wednesday: ");
	print_sum(wed, "Wednesday");
	cout << separation << endl;
	print(thu, "Thursday: ");
	print_sum(thu, "Thursday");	
	cout << separation << endl;
	print(fri, "Friday: ");
	print_sum(fri, "Friday");
	cout << separation << endl;
	print(sat, "Saturday: ");
	print_sum(sat, "Saturday");
	cout << separation << endl;
	print(sun, "Sunday: ");
	print_sum(sun, "Sunday");	
}

int main() {
	try {
		get_records();
	}
	catch (End_Of_Data& e) {
	}
	
	print_results();
}
