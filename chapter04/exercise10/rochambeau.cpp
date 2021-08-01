#include <iostream>
#include <vector>

using namespace std;

vector<char>   computer_options_selection = { 'p',     'r',    's' };

constexpr int victory = -99;
constexpr int tie     = -333;
constexpr int defeat  = -77;

constexpr char paper    = 'p';
constexpr char rock     = 'r';
constexpr char scissors = 's';

constexpr char quit     = 'q';

constexpr int fatal_error = -1000;

void abort(char option) {
	cerr << "Fatal error - another option " << option << "\n";
	exit(fatal_error);
}

int get_result_versus_paper(char option) {
	switch(option) {
		case paper:
			return tie;
		case rock:
			return defeat;
		case scissors:
			return victory;
		default:
			abort(option);
	}
}

int get_result_versus_rock(char option) {
	switch(option) {
		case paper:
			return victory;
		case rock:
			return tie;
		case scissors:
			return defeat;
		default:
			abort(option);
	}
}
	
int get_result_versus_scissors(char option) {
	switch(option) {
		case paper:
			return defeat;
		case rock:
			return victory;
		case scissors:
			return tie;
		default:
			abort(option);
	}
}

int get_result_for_first_player(char first_option, char second_option) {
	int result = 0;
	switch(second_option) {
		case paper:
			result = get_result_versus_paper(first_option);
			break;
		case rock:
			result = get_result_versus_rock(first_option);
			break;
		case scissors:
			result = get_result_versus_scissors(first_option);
			break;
		default:
			abort(second_option);
	}
	
	return result;
}

string get_result_string(char first_option, char second_option) {
	int result = get_result_for_first_player(first_option, second_option);
	switch(result) {
		case victory:
			return "First player is winner ";
		case tie:
			return "Tie ";
		case defeat:
			return "Second player is winner ";
		default:
			cerr << "Fatal error - another result " << result << "\n";
			exit(fatal_error);
	}
}

char get_computer_option(unsigned int number) {
	char character;
	cout << "2. Enter key/keys to continue. To go to next step enter e or E: ";
	while(cin >> character && character != 'e' && character != 'E')
		number += (unsigned int)character;
		
	number += number % 16;
	unsigned int index = number % computer_options_selection.size();
		
	char computer_option = computer_options_selection[index];
	return computer_option;
}

bool is_game_option(char option) {
	switch(option) {
		case paper:
		case rock:
		case scissors:
			return true;
		default:
			return false;
	}
}

int main() {							  										  
	cout << "1. Enter key (" << paper << ", " << rock << ", " << scissors 
			<< ") to select Your option. To quit enter: " << quit << "\n";
	char first_option;
	char second_option;
	string game_result;
	for(unsigned int counter = 0; cin >> first_option && first_option != quit; counter++) {
		if (false == is_game_option(first_option)) 
			cout << "Option " << first_option << " is not a game option\n";
		else {
			second_option = get_computer_option(counter);
			cout << "First player option: " << first_option << endl;
			cout << "Second player option: " << second_option << endl;
			game_result = get_result_string(first_option, second_option);
			cout << "GAME RESULT: \n" << game_result << endl;
			cout << "\nEnter key (" << paper << ", " << rock << ", " << scissors 
				<< ") to select option. To quit enter: " << quit << "\n";
		}
	}
}
