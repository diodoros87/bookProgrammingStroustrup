#include <iostream>
#include <vector>

using namespace std;
	
int main() {
	vector<string> digits_in_words = { "zero", "one", "two",   "three", "four",
												  "five", "six", "seven", "eight", "nine" };								  
	const unsigned int size = digits_in_words.size();
												  
	cout << "To go to next stage enter 100. Enter digit (0-9) to get digit in words: \n";
	unsigned int index;
	while(cin >> index && index != 100) {
		if (index >= 0 && index < size)
			cout << digits_in_words[index] << endl;
		else
			cout << "Entered value is not digit \n";
			
		cout << "To go to next stage enter 100. Enter digit (0-9) to get digit in words: \n";
	}
	
	cout << "Enter digit in words (zero, one, ..., nine) to get digit as number: \n";
	string word;
	while(cin >> word) {
		unsigned int i = 0;
		bool is_digit = false;
		for (; false == is_digit && i < size; i++) 
			if (digits_in_words[i] == word) {
				cout << i << endl;
				is_digit = true;
			}
			
		if (false == is_digit)
			cout << "Entered value is not digit in words\n";
			
		cout << "Enter digit in words (zero, one, ..., nine) to get digit as number: \n";
	}
}
