#include <iostream>
#include <vector>

using namespace std;

vector<char> numbers = {'1', '2', '3', '4'};
const int size = numbers.size();

constexpr char end_of_game = 'e';
constexpr char continue_game = 'c';
constexpr char reserved = 'r';

void check_chars_preconditions(vector<char>& user_chars) {
	if (user_chars.size() != size) 
		throw runtime_error("Precondition: number of user's chars != number of all digits to guess");
}

void print(string start, string end, vector<char>& vec) {
	cerr << start << endl;
	for (unsigned int i = 0; i < vec.size(); i++) {
		cerr << vec[i] << " ";
	}
	cerr << end << endl;
}

unsigned int calculate_bulls(vector<char>& user_chars) {
	check_chars_preconditions(user_chars);
	
	unsigned int result = 0;
	for (unsigned int i = 0; i < size; i++) {
		if (user_chars[i] == numbers[i]) {
			user_chars[i] = reserved;
			result++;
		}
	}
	return result;
}

unsigned int get_number_of_char(char c, vector<char>& vec) {
	unsigned int counter = 0;
	for (unsigned int i = 0; i < vec.size(); i++) {
		if (c == vec[i])
			counter++;
	}
	return counter;
}

unsigned int calculate_cows(vector<char>& user_chars) {
	check_chars_preconditions(user_chars);
	
	print("BEGIN DEBUG: user_chars", "END", user_chars);
	vector<char> cow_chars;
	unsigned int result = 0;
	for (unsigned int i = 0; i < size; i++) {
		unsigned int single_char_cows_counter = get_number_of_char(user_chars[i], cow_chars);
		
		for (unsigned int j = 0; j < size; j++) {
			if (j != i && user_chars[i] != reserved && user_chars[j] != reserved) { // eliminate comparison with bulls positions
				if (user_chars[i] == numbers[j]) {
					if (single_char_cows_counter == 0) {
						cow_chars.push_back(user_chars[i]);
						result++;
					}
					else
						single_char_cows_counter--;
				}
			}
		}
	}
	print("END DEBUG: user_chars", "END", user_chars);
	return result;
}

bool is_digit(char c) {
	if (c >= '0' && c <= '9')
		return true;
	else
		return false;
}

bool get_chars(vector<char>& input_chars) {
	cout << "Enter " << size << " digits to guess digits sequence: ";
	char c;
	bool is_end_of_data = false;
	for (unsigned int i = 0; i < size && !is_end_of_data; i++) {
		cin >> c;
		
		if (!cin)
			is_end_of_data = true;
		else if (false == is_digit(c))
			throw runtime_error("Entered value must be digit from 0 to 9");
		else
			input_chars.push_back(c);
	}
	
	return is_end_of_data;
}

char game() {
	vector<char> user_chars; 
	bool is_end_of_data = get_chars(user_chars);
	
	if (is_end_of_data)
		return end_of_game;
	else {
		unsigned int bulls = calculate_bulls(user_chars);
		unsigned int cows  = calculate_cows(user_chars);
		
		cout << "Result: " << bulls << " bulls " << cows << " cows " << endl;
		if (size == bulls)
			return end_of_game;
		else
			return continue_game;
	}
}

int main() { 
	char game_status = continue_game;
	do {
		try {
			game_status = game();
		}
		catch (runtime_error& e) {
			cout << e.what() << endl;
		}
		
	} while(game_status == continue_game);

}
