#include <iostream>
#include <vector>

using namespace std;

constexpr int fatal_error = -1;

void print(vector<int>& scores, vector<string>& names) {
	const unsigned int size = scores.size();
	if (size != names.size())
		exit(fatal_error);
		
	for (unsigned int i = 0; i < size; i++)
		cout << names[i] << "   " << scores[i] << "\n";
}

bool exist(string s, vector<string>& vec) {
	const unsigned int size = vec.size();

	for (unsigned int i = 0; i < size; i++)
		if (vec[i] == s)
			return true;
			
	return false;
}

vector<string> get_values_repeatable(vector<string>& vec) {
	const unsigned int size = vec.size();
	vector<string> result;
	for (unsigned int i = 0; i < size; i++)
		for (unsigned int j = i + 1; j < size; j++)
			if (vec[i] == vec[j] && false == exist(vec[i], result))
				result.push_back(vec[i]);
				
	return result;
}

void check_names(vector<string>& names) {
	vector<string> repeatable_names = get_values_repeatable(names);
	const unsigned int size = repeatable_names.size();
	if (size > 0) {
		cout << "Error! Entered repeatable names:  " << "\n";
		for (unsigned int i = 0; i < size; i++)
			cout << repeatable_names[i] << "\n";
	}
}

int main() {
	vector<string> names;
	vector<int> scores;
	string name;
	int score;
	
	cout << "Enter name and name's number: \n";
	while(cin >> name) {
		if (! (cin >> score) || score < 0)
			cout << "Entered value must be integer >= 0 \n";
		else if (name == "NoName" && score == 0)
			break;
		else {
			names.push_back(name);
			scores.push_back(score);
		}
		cout << "Enter name and name's number: \n";
	}
	
	print(scores, names);
	check_names(names);
}
