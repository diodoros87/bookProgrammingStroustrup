#include <iostream>
#include <vector>
#include <limits>

using namespace std;

unsigned long factorial(short number) {
	if (0 > number)
		throw runtime_error("Precondition: can not calculate factorial for number < 0");
	
	constexpr unsigned long U_LONG_MAX = numeric_limits<unsigned long>::max();
	unsigned long result = 1;
	for (short counter = 2; counter <= number; counter++) {
		if (result > U_LONG_MAX / counter) {
			cerr << "Calculating factorial of " << number << " = " << number << "! :\n";
			cerr << "While calculating factorial of " << counter << " = " << counter << "! :\n";
			cerr << 	" result = " << result << " U_LONG_MAX = " << U_LONG_MAX << " \n";
			cerr << "result > U_LONG_MAX / counter that is " << result << " > " << U_LONG_MAX / counter << "\n";
			cerr << "result * counter = " << result * counter << "\n";
			throw overflow_error("unsigned long overflow error");
		}
		result = result * counter;
	}
	
	if (0 >= result)
		throw runtime_error("Final condition: result of factorial can not be number <= 0");
		
	return result;
}

class End_Of_Data {};

string get_string(const string& prompt) {	
	string s;
	
	cout << prompt;
	cin >> s;
	if (!cin)
		throw End_Of_Data();
	
	return s;
}

short get_number(const string& prompt) {	
	short n;
	
	cout << prompt;
	cin >> n;
	if (!cin) {
		cerr << "Max value for short type int is " << numeric_limits<short>::max() << endl;
		cerr << "Min value for short type int is " << numeric_limits<short>::min() << endl;
		throw End_Of_Data();
	}
	
	return n;
}

void get_numbers(short& a, short& b) {
	const string PROMPT = "Enter value of number ";
	cout << "\nTo quit press Ctrl+D\n";
	a = get_number(PROMPT + "a: ");
	b = get_number(PROMPT + "b: ");
}

unsigned long calculate_permutation(short a, short b) {
	if (a < 0 || b < 0)
		throw runtime_error("Precondition: can not calculate permutation for number < 0");
	if (a < b)
		throw runtime_error("Precondition of permutation: a must be >= b");
	unsigned long dividend = factorial(a);
	unsigned long divisor = factorial(a - b);
	unsigned long result = dividend / divisor;
	return result;
}

void permutation(short a, short b) {
	unsigned long result = calculate_permutation(a, b);
	cout << "P(" << a << ", " << b << ") = " << result << endl;
}

unsigned long calculate_combination(short a, short b) {
	unsigned long dividend = calculate_permutation(a, b);
	unsigned long divisor = factorial(b);
	unsigned long result = dividend / divisor;
	return result;
}

void combination(short a, short b) {
	unsigned long result = calculate_combination(a, b);
	cout << "C(" << a << ", " << b << ") = " << result << endl;
}

void calculation(const string& option) {
	const string PERMUTATION = "P";
	const string COMBINATION = "C";
	short a, b;
	
	get_numbers(a, b);
	try {
		if (option == PERMUTATION) 
			permutation(a, b);
		else if (option == COMBINATION) 
			combination(a, b);
		else {
			cerr << "Unrecognized option " << option << endl;
			throw exception();
		}
	}
	catch (runtime_error& e) {
		cerr << e.what() << endl;
	}
}

void print_formulas() {
	cout << "Formulas for:\n";
	cout << "Permutation       P(a, b) = a! / (a-b)!  \n";
	cout << "Combination       C(a, b) = P(a, b) / b! \n";
}

void print_menu(const string PERMUTATION, const string COMBINATION, const string EXIT) {
	cout << "\n\n";
	print_formulas();
	cout << "Select option:\n";
	cout << "1. Calculate permutation - enter " << PERMUTATION << "\n";
	cout << "2. Calculate combination - enter " << COMBINATION << "\n";
	cout << "3. Exit                  - enter " << EXIT << "\n";
}

bool is_lower_case(char c) {
	if (c >= 'a' && c <= 'z')
		return true;
	else
		return false;
}

char get_upper_case(char c) {
	if (false == is_lower_case(c)) {
		cerr << "character = " << c << endl; 
		throw runtime_error("character is not lower case");
	}
	constexpr int DIFF = 'a' - 'A';
	int result_ascii = int(c) - DIFF;
	char result = char(result_ascii);
	return result;
}

string get_upper_case(const string& s) {
	const unsigned int LENGTH = s.size();
	if (0 == LENGTH)
		throw runtime_error("Precondition: string can not be empty");
	string result = s;
	if (false == is_lower_case(s[0]))
		return result;
		
	char c = get_upper_case(s[0]);
	result[0] = c;
	return result;
}

void menu() {
	const string PERMUTATION = "P";
	const string COMBINATION = "C";
	const string EXIT = "E";
	bool is_continue = true;
	string option;
	string option_uppercase;
	
	do {
		print_menu(PERMUTATION, COMBINATION, EXIT);
		option = get_string("");
		option_uppercase = get_upper_case(option);
		if (option == PERMUTATION || option_uppercase == PERMUTATION) 
			calculation(PERMUTATION);
		else if (option == COMBINATION || option_uppercase == COMBINATION) 
			calculation(COMBINATION);
		else if (option == EXIT || option_uppercase == EXIT)
			is_continue = false;
		else
			cout << "Unrecognized option " << option << endl;
			
	} while(is_continue);

}

int main() {
	try {
		menu();
	} 
	catch (End_Of_Data& e) { 
		cerr << endl;
	}
	catch (exception& e) {
		cerr << e.what() << endl;
	}
	catch (...) {
		cerr << "Unrecognized type of exception" << endl;
	}
}


