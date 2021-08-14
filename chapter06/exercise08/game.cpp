#include <iostream>
#include <vector>

using namespace std;

constexpr unsigned int SIZE = 4;
vector<char> sequence_to_guess(SIZE);

char random_ascii_char(const char MIN, const char MAX) {
	int random_value = rand ();
	int ascii_result = MIN + random_value % (MAX - MIN + 1);
	char result = char(ascii_result);
	return result;	
}

char random_upper_letter() {	
	constexpr char MIN = 'A';
	constexpr char MAX = 'Z';
	char result = random_ascii_char(MIN, MAX);
	return result;
}

char random_lower_letter() {	
	constexpr char MIN = 'a';
	constexpr char MAX = 'z';
	char result = random_ascii_char(MIN, MAX);
	return result;
}

char get_random_letter() {	
	int random_value = rand () % 2;
	char result;
	if (0 == random_value)
		result = random_upper_letter();
	else
		result = random_lower_letter();
	
	return result;
}

void draw_values(unsigned int seed) {
	srand(seed);

	for (unsigned int i = 0; i < SIZE; i++) 
		sequence_to_guess[i] = get_random_letter();
	
	cout << endl;
	for (unsigned int i = 0; i < SIZE; i++)
		cout << sequence_to_guess[i] << endl;
}

void check_chars_preconditions(vector<char>& user_chars) {
	if (user_chars.size() != SIZE) 
		throw runtime_error("Precondition: number of user's chars != number of all chars to guess");
}

void print(string start, string end, vector<char>& vec) {
	cerr << start << endl;
	for (unsigned int i = 0; i < vec.size(); i++) {
		cerr << vec[i] << " ";
	}
	cerr << end << endl;
}

constexpr char reserved = '#';

unsigned int calculate_bulls(vector<char>& user_chars) {
	check_chars_preconditions(user_chars);
	
	unsigned int result = 0;
	for (unsigned int i = 0; i < SIZE; i++) {
		if (user_chars[i] == sequence_to_guess[i]) {
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
	for (unsigned int i = 0; i < SIZE; i++) {
		unsigned int single_char_cows_counter = get_number_of_char(user_chars[i], cow_chars);
		
		for (unsigned int j = 0; j < SIZE; j++) {
			if (j != i && user_chars[i] != reserved && user_chars[j] != reserved) { // eliminate comparison with bulls positions
				if (user_chars[i] == sequence_to_guess[j]) {
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

bool is_letter(char c) {
	bool result = is_lower_letter(c) || is_upper_letter(c);
	return result;
}

class End_Of_Data {};

string get_string(const unsigned int length) {	
	string s;
	
	cout << "Enter " << length << "-length string of letters to guess sequence: ";

	do {
		cin >> s;
		if (!cin)
			throw End_Of_Data();
		if (length == s.size()) 
			break;
		else
			cerr << "Entered value must have " << length << " letters length \n";
	} while (true);
	
	return s;
}

bool is_letters_string(const string& s) {
	const unsigned int LENGTH = s.size();
	for (unsigned int i = 0; i < LENGTH; i++) 
		if (false == is_letter(s[i]))
			return false;
			
	if (0 == LENGTH)
		return false;
	else		
		return true;
}

string get_letters_string(const unsigned int length) {	
	string s = get_string(length);
	if (false == is_letters_string(s))
		throw runtime_error("Entered value must be letter a-z or A-Z");
	
	return s;
}

void get_chars(const string& str, vector<char>& input_chars) {
	input_chars.clear();
	for (unsigned int i = 0; i < str.size(); i++)
		input_chars.push_back(str[i]);
}

void get_chars(vector<char>& input_chars) {
	input_chars.clear();
	string guess_string = get_letters_string(SIZE);
	get_chars(guess_string, input_chars);
}

constexpr char END_OF_GAME = '0';
constexpr char CONTINUE_GAME = '1';

char game(vector<char>& user_chars) {
	unsigned int bulls = calculate_bulls(user_chars);
	unsigned int cows  = calculate_cows(user_chars);
	
	cout << "Result: " << bulls << " bulls and " << cows << " cows " << endl;
	if (SIZE == bulls)
		return END_OF_GAME;
	else
		return CONTINUE_GAME;
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
	draw_values(seed);
	
	char game_status = CONTINUE_GAME;
	vector<char> user_chars; 

	do {
		try {
			get_chars(user_chars);
			game_status = game(user_chars);
		}
		catch (runtime_error& e) {
			cout << e.what() << endl;
		}
		
	} while(game_status == CONTINUE_GAME);
}

void print_menu(char start_game, char exit) {
	cout << "Select option:\n";
	cout << "1. Start game - press " << start_game << "\n";
	cout << "2. Exit       - press " << exit << "\n";
}

void menu() {
	constexpr char start_game = '1';
	constexpr char exit = '2';
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


