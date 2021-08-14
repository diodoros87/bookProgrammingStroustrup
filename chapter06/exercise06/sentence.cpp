#include <iostream>
#include <vector>

using namespace std;

bool exist(const string& s, const vector<string>& vec) {
	for (unsigned int i = 0; i < vec.size(); i++)
		if (vec[i] == s)
			return true;
		
	return false;
}

bool is_lower_letter(char c) {
	if (c >= 'a' && c <= 'z')
		return true;
	else
		return false;
}

bool is_upper_letter(char c) {
	if (c >= 'A' && c <= 'Z')
		return true;
	else
		return false;
}

char get_upper_case(char c) {
	if (false == is_lower_letter(c)) {
		if (!cin)
			exit(0);
		cerr << "character = " << c << endl; 
		throw runtime_error("character is not lower letter");
	}
	constexpr int DIFF = 'a' - 'A';
	int result_ascii = int(c) - DIFF;
	char result = char(result_ascii);
	return result;
}

bool equals(const string& a, const string& b, unsigned int index) {
	const unsigned int A_SIZE = a.size();
	if (index >= A_SIZE) {
		if (!cin)
			exit(0);
		cerr << "Index = " << index << " string.size = " << A_SIZE << endl; 
		throw runtime_error("index >= string.size())");
	}
	
	const unsigned int B_SIZE = b.size();
	if (B_SIZE != A_SIZE) 
		return false;
		
	char bc;
	for (unsigned int i = 0; i < A_SIZE; i++) {
		bc = b[i];
		if (index == i && true == is_lower_letter(bc)) 
			bc = get_upper_case(bc);
		if (a[i] != bc)
			return false;
	}

	return true;
}

bool exist(const string& s, const vector<string>& vec, bool case_sensitive, unsigned int index) {
	if (true == case_sensitive)
		return exist(s, vec);
	
	for (unsigned int i = 0; i < vec.size(); i++)
		if (true == equals(s, vec[i], index))
			return true;
			
	return false;
}

bool preposition(string& s, bool first_word) {
	const vector<string> prepositions = {"the"};
	cin >> s;
	bool result;
	if (true == first_word)
		result = exist(s, prepositions, false, 0);
	else
		result = exist(s, prepositions);
	return result;
}

bool noun(string& s, bool first_word) {
	const vector<string> nouns        = {"birds", "C++", "fish"};
	bool result = preposition(s, first_word);
	if (true == result) {
		cin >> s;
		result = exist(s, nouns);
	}
	else if (true == first_word)
		result = exist(s, nouns, false, 0);
	else
		result = exist(s, nouns);
	return result;
}

bool verb(string& s) {
	const vector<string> verbs        = {"fly", "swim", "rules"};
	cin >> s;
	bool result = exist(s, verbs);
	return result;
}

bool conjuction(string& s) {
	const vector<string> conjuctions        = {"and", "or", "but"};
	cin >> s;
	bool result = exist(s, conjuctions);
	return result;
}

void putback(const string& s) {
	for (int i = s.size() - 1; i >= 0;  i--)
		cin.putback(s[i]);
}

bool is_sentence_termination(string& s) {
	constexpr char DOT = '.';
	cin >> s;
	if (s.size() == 1 && s[0] == DOT)
		return true;
	else
		putback(s);
		
	return false;	
}

bool sentence(bool first_word) {
	string word;
	bool result = noun(word, first_word);
	if (result)
		result = verb(word);
	if (result && is_sentence_termination(word))
		return true;
	
	first_word = false;	
	if (result)
		result = conjuction(word);
	if (result)
		result = sentence(first_word);
	return result;
}

int main() {
	bool is_sentence = false;
	bool first_word = true;
	cout << "\n Enter Ctrl+D to quit \n ";
	cout << "Sentence must be started with upper letter \n";
	cout << "Enter words to check all words are sentence: \n";
	do {
		is_sentence = sentence(first_word);
		if (!cin) 
			break;
			
		if (is_sentence)
			cout << "OK \n";
		else
			cout << "Wrong \n";
	} while(true);
	
}
