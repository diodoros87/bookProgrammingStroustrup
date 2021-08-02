#include <iostream>
#include <vector>

using namespace std;

constexpr int fatal_error = -1;
const string no_score = "";

void ensure_sizes(vector<int>& scores, vector<string>& names) {
	if (scores.size() != names.size())
		exit(fatal_error);
}

void print(vector<int>& scores, vector<string>& names) {
	ensure_sizes(scores, names);
	for (unsigned int i = 0; i < names.size(); i++)
		cout << names[i] << "   " << scores[i] << "\n";
}

bool exist(int n, vector<int>& vec) {
	for (unsigned int i = 0; i < vec.size(); i++)
		if (vec[i] == n)
			return true;
			
	return false;
}

bool exist(string s, vector<string>& vec) {
	for (unsigned int i = 0; i < vec.size(); i++)
		if (vec[i] == s)
			return true;
			
	return false;
}

string get_name_for_score(int score, vector<string>& names, vector<int>& scores) {
	ensure_sizes(scores, names);
	for (unsigned int i = 0; i < scores.size(); i++)
		if (scores[i] == score)
			return names[i];
			
	return no_score;
}

void print_name_for_score(vector<string>& names, vector<int>& scores) {
	int score;
	string name_for_score;
	
	cout << "\n Enter number to get number's name or Ctrl+D to quit: ";
	while(cin >> score) {
		name_for_score = get_name_for_score(score, names, scores);
		if (name_for_score == no_score)
			cout << "No score for such number\n";
		else
			cout << "For score \"" << score << "\" name is " << name_for_score << "\n";
		//print(scores, names);
		cout << "\n Enter number to get number's name or Ctrl+D to quit: ";
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
		print_name_for_score(names, scores);
	}
}
