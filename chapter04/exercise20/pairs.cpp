#include <iostream>
#include <vector>

using namespace std;

constexpr int fatal_error = -1;
constexpr int no_name = -2;

void ensure_sizes(vector<int>& scores, vector<string>& names) {
	if (scores.size() != names.size())
		exit(fatal_error);
}

void print(vector<int>& scores, vector<string>& names) {
	ensure_sizes(scores, names);
	for (unsigned int i = 0; i < names.size(); i++)
		cout << names[i] << "   " << scores[i] << "\n";
}

bool exist(string s, vector<string>& vec) {
	for (unsigned int i = 0; i < vec.size(); i++)
		if (vec[i] == s)
			return true;
			
	return false;
}

int get_score_for_name(string name, vector<string>& names, vector<int>& scores) {
	ensure_sizes(scores, names);
	for (unsigned int i = 0; i < names.size(); i++)
		if (names[i] == name)
			return scores[i];
			
	return no_name;
}

void print_score_for_name(vector<string>& names, vector<int>& scores) {
	string name;
	int score_for_name;
	
	cout << "\n Enter name to get name's number or Ctrl+D to quit: ";
	while(cin >> name) {
		score_for_name = get_score_for_name(name, names, scores);
		if (score_for_name == no_name)
			cout << "No name for such name\n";
		else
			cout << "For name \"" << name << "\" score is " << score_for_name << "\n";
		//print(scores, names);
		cout << "\n Enter name to get name's number or Ctrl+D to quit: ";
	}
	
}

int main() {
	vector<string> names;
	vector<int> scores;
	string name;
	int score;
	constexpr int exit_code = -3;
	
	cout << "\n To finish entering of data enter \'NoName\' and 0: \n";
	cout << "Enter name and name's number: \n";
	while(cin >> name) {
		if (true == exist(name, names)) {
			cout << "Error! Entered repeatable name:  " << name << "\n";
			return exit_code;
		}
		if (! (cin >> score) || score < 0) {
			cout << "Entered value must be integer >= 0 \n";
			return exit_code;
		}
		else if (name == "NoName" && score == 0)
			break;
		else {
			names.push_back(name);
			scores.push_back(score);
		}
		
		cout << "\n To finish entering of data enter \'NoName\' and 0: \n";
		cout << "Enter name and name's number: \n";
	}
	
	if(names.size() > 0) {
		print(scores, names);
		print_score_for_name(names, scores);
	}
}
