#include <iostream>
#include <vector>
#include <limits>

using namespace std;

bool is_digit(char c) {
	if (c >= '0' && c <= '9')
		return true;
	else
		return false;
}

int digit_to_int(char c) {
	if (false == is_digit(c))
		throw runtime_error("Precondition: char must be digit \'0\' - \'9\'");
		
	int number = int(c) - int('0');
	return number;
}

bool is_digits_string(const string& s) {
	const unsigned int LENGTH = s.size();
	for (unsigned int i = 0; i < LENGTH; i++) 
		if (false == is_digit(s[i]))
			return false;
			
	if (0 == LENGTH)
		return false;
	else		
		return true;
}

constexpr unsigned long U_LONG_MAX = numeric_limits<unsigned long>::max();

unsigned long power(unsigned int base, unsigned short exponent) {
	unsigned long result = 1;
	for (unsigned short counter = 1; counter <= exponent; counter++) {
		if (result > U_LONG_MAX / base) {
			cerr << "Calculate " << base << " to power of " << exponent << " unsigned long overflow :\n";
			cerr << "While calculating " << base << " to power of " << counter <<
				" result = " << result << " U_LONG_MAX = " << U_LONG_MAX << " \n";
			cerr << "result > U_LONG_MAX / base that is " << result << " > " << U_LONG_MAX / base << "\n";
			cerr << "result * base = " << result * base << "\n";
			throw overflow_error("overflow error");
		}
		
		result *= base;
	}
		
	return result;
} 

unsigned long string_to_int(const string str) {
	if (false == is_digits_string(str))
		throw runtime_error("Precondition: string must have only digits 0-9 ");
	const unsigned int LENGTH = str.size();
	if (0 == LENGTH)
		throw runtime_error("Precondition: string can not be empty");
		
	unsigned long result = 0;
	unsigned long power_of_10  = 0;
	constexpr unsigned int base = 10;
	unsigned short digit = 0; 
	for (unsigned int exponent = 0; exponent < LENGTH; exponent++) {
		power_of_10 = power(base, exponent);
		digit = digit_to_int(str[LENGTH - 1 - exponent]);
		if (power_of_10 > U_LONG_MAX / digit) {
			cerr << "power_of_10 > U_LONG_MAX / digit that is " << power_of_10 << " > " << U_LONG_MAX / digit << "\n";
			cerr << "power_of_10 * digit = " << power_of_10 * digit << "\n";
			throw overflow_error("overflow error");
		}
		if (result > U_LONG_MAX - power_of_10 * digit) {
			cerr << "result > U_LONG_MAX  - power_of_10 * digit that is " << result << " > " 
				<< U_LONG_MAX - power_of_10 * digit << "\n";
			cerr << "result + power_of_10 * digit = " << result + power_of_10 * digit << "\n";
			throw overflow_error("overflow error");
		}
		
		result += power_of_10 * digit;
	}

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

string get_digits_string() {	
	const string prompt = "Enter number to decomposition: ";
	const string error = "Value must have only digits 0-9 ";
	string s = get_string(prompt);
	if (false == is_digits_string(s))
		throw runtime_error(error);
	
	return s;
}

unsigned long get_order_of_magnitude(unsigned long number) {
	constexpr unsigned int base = 10;
	unsigned long result = 1;
	unsigned short exponent = 0;
	while (number > result) {
		try {
			result = power(base, ++exponent);
		}
		catch (overflow_error& e) {
			if (number <= U_LONG_MAX) {
				result = power(base, --exponent);
				return result;
			}
		}
	}
		
	if (number < result && 0 != number)
		result = power(base, --exponent);
		
	return result;
}

void decomposition(unsigned long number) {
	cout << "Decomposition of number " << number << "\n";
	unsigned long order_of_magnitude = get_order_of_magnitude(number);
	constexpr unsigned int base = 10;
	while(order_of_magnitude > 0) {
		cout << "Number is " << number / order_of_magnitude;
		cout << " for order of magnitude = " << order_of_magnitude << endl;
		number %= order_of_magnitude;
		order_of_magnitude /= base;
	}
}

void decomposition() {
	string digits;
	do {
		cout << "\nTo quit press Ctrl+D\n";
		try {
			digits = get_digits_string();
			unsigned long number = string_to_int(digits);
			decomposition(number);
		}
		catch (runtime_error& e) {
			cout << e.what() << endl;
		}
	} while(true);
}

int main() {
	try {
		decomposition();
	} 
	catch (End_Of_Data& e) { }
}


