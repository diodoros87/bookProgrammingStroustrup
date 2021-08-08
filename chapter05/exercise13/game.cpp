#include <iostream>
#include <vector>
//#include <cstdlib>

using namespace std;

const unsigned int size = 4;
vector<char> numbers(size);

const unsigned int min_value = 0;
const unsigned int max_value = 9;

unsigned int get_random_digit(int min, int max) {
	if (min < 0)
		throw runtime_error("Precondition: min must be >= 0");
	if (max > 9)
		throw runtime_error("Precondition: max must be <= 9");
	if (max <= min)
		throw runtime_error("Precondition: max must be > min");
		
	int random_value = rand ();
	unsigned int digit = min + random_value % (max - min + 1);
	
	return digit;
}

char digit_to_char(unsigned int n) {
	if (n > 9)
		throw runtime_error("Precondition: n must be >= 0 and n must be < 9");
		
	char c = n + '0';
	return c;
}

void draw_values(unsigned int seed, int min, int max) {
	srand(seed);
	unsigned int random_value;
	char c;
	for (unsigned int i = 0; i < size; i++) {
		random_value = get_random_digit(min, max);
		c = digit_to_char(random_value);
		numbers[i] = c;
	}
	
	cout << endl;
	for (unsigned int i = 0; i < size; i++)
		cout << numbers[i] << endl;
}

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

constexpr char reserved = 'r';

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
						break;
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

class End_Of_Data {};

void get_chars(vector<char>& input_chars) {
	input_chars.clear();
	cout << "Enter " << size << " digits to guess digits sequence: ";
	char c;
	for (unsigned int i = 0; i < size; i++) {
		cin >> c;
		
		if (!cin)
			throw End_Of_Data();
		else if (false == is_digit(c))
			throw runtime_error("Entered value must be digit from 0 to 9");
		else
			input_chars.push_back(c);
	}
}

constexpr char end_of_game = 'e';
constexpr char continue_game = 'c';

char game(vector<char>& user_chars) {
	unsigned int bulls = calculate_bulls(user_chars);
	unsigned int cows  = calculate_cows(user_chars);
	
	cout << "Result: " << bulls << " bulls and " << cows << " cows " << endl;
	if (size == bulls)
		return end_of_game;
	else
		return continue_game;
}

int calculate_seed() {
	char character;
	int result = 0;
	cout << "Enter some non-whitespaces chars to continue. To end of data press x. \n";
	while(cin >> character && character != 'x')
		result += int(character);
		
	if (!cin)
		throw End_Of_Data();
		
	return result;
}

void game(unsigned int seed) { 
	draw_values(seed, min_value, max_value);
	
	char game_status = continue_game;
	vector<char> user_chars; 

	do {
		try {
			get_chars(user_chars);
			game_status = game(user_chars);
		}
		catch (runtime_error& e) {
			cout << e.what() << endl;
		}
		
	} while(game_status == continue_game);
}

void print_menu(char start_game, char exit) {
	cout << "Select option:\n";
	cout << "1. Start game - press " << start_game << "\n";
	cout << "2. Exit       - press " << exit << "\n";
}

void menu() {
	constexpr char start_game = 'A';
	constexpr char exit = 'B';
	bool is_continue = true;
	char option;
	unsigned int seed;
	
	do {
		print_menu(start_game, exit);
		cin >> option;
		
		if (!cin)
			throw End_Of_Data();
		else {
			switch(option) {
				case start_game:
					seed = calculate_seed();
					game(seed);
					break;
				case exit:
					is_continue = false;
					break;
				default:
					cout << "Unrecognized option " << option << endl;
			}
		}
			
	} while(is_continue);

}

int main() {
	try {
		menu();
	} 
	catch (End_Of_Data& e) { }
}


