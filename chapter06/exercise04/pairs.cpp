#include <iostream>
#include <vector>

using namespace std;

constexpr int no_name = -2;

class Name_value {
public:
   const string name;       
   const int value;     
   
   Name_value(string s, int n)    
		:name(s), value(n) { }
};

bool exist_name(string s, vector<Name_value>& vec) {
	for (unsigned int i = 0; i < vec.size(); i++)
		if (vec[i].name == s)
			return true;
			
	return false;
}

vector<string> get_names_for_value(int value, vector<Name_value>& vec) {
	vector<string> result;
	for (unsigned int i = 0; i < vec.size(); i++)
		if (vec[i].value == value)
			result.push_back(vec[i].name);
			
	return result;
}

int get_value_for_name(string name, vector<Name_value>& vec) {
	for (unsigned int i = 0; i < vec.size(); i++)
		if (vec[i].name == name)
			return vec[i].value;
			
	return no_name;
}

void print(vector<Name_value>& vec) {
	for (unsigned int i = 0; i < vec.size(); i++)
		cout << vec[i].name << "   " << vec[i].value << "\n";
}

void print_score_for_name(vector<Name_value>& vec, const string& last_string) {
	string name;
	int score_for_name;
	
	cout << "\n Enter: \n";
	cout << " name to get name's number or \n";
	cout << " \'" << last_string << "\' to go to next stage \n";
	cout << "  or Ctrl+D to quit: ";
	while(cin >> name && name != last_string) {
		score_for_name = get_value_for_name(name, vec);
		if (score_for_name == no_name)
			cout << "No name for such name\n";
		else
			cout << "For name \"" << name << "\" score is " << score_for_name << "\n";
		
		cout << "\n Enter: \n";
		cout << " name to get name's number or \n";
		cout << " \'" << last_string << "\' to go to next stage \n";
		cout << "  or Ctrl+D to quit: ";
	}
	
}

void print_name_for_score(vector<Name_value>& vec) {
	int score;
	vector<string> names_for_score;
	
	cout << "\n Enter number to get number's name or Ctrl+D to quit: ";
	while(cin >> score) {
		names_for_score = get_names_for_value(score, vec);
		if (names_for_score.size() == 0)
			cout << "No score for such number\n";
		else {
			cout << "For score " << score << " names are: ";
			for (unsigned int i = 0; i < names_for_score.size(); i++)
				cout << names_for_score[i] << "\t";
			cout << "\n";
		}
		cout << "\n Enter number to get number's name or Ctrl+D to quit: ";
	}
	
}

int main() {
	vector<Name_value> vec;
	string name;
	int score;
	constexpr int exit_code = -3;
	const string last_string = "NoName";
	
	cout << "\n To finish entering of data enter \'NoName\' and 0: \n";
	cout << "Enter name and name's number: \n";
	while(cin >> name) {
		if (true == exist_name(name, vec)) {
			cout << "Error! Entered repeatable name:  " << name << "\n";
			return exit_code;
		}
		if (! (cin >> score) || score < 0) {
			cout << "Entered value must be integer >= 0 \n";
			return exit_code;
		}
		else if (name == last_string && score == 0)
			break;
		else {
			Name_value x = Name_value(name, score);
			vec.push_back(x);
		}
		
		cout << "\n To finish entering of data enter \'NoName\' and 0: \n";
		cout << "Enter name and name's number: \n";
	}
	
	if(vec.size() > 0) {
		print(vec);
		print_score_for_name(vec, last_string);
		print_name_for_score(vec);
	}
}
