#include <iostream>
#include <vector>

using namespace std;

constexpr int incorrect_result = -999;

vector<string> digits_in_words = { "zero", "one", "two",   "three", "four",
												  "five", "six", "seven", "eight", "nine" };
vector<string> digits = 			{ "0", 	 "1",   "2",      "3",    "4",
											  "5",    "6",   "7",      "8",    "9" };

int get_index(string x) {
	if (digits.size() != digits_in_words.size()) {
		cerr << "Unequable size of vectors of digits" << "\n";
		exit(incorrect_result);
	}
	for (int index = 0; index < digits.size(); index++) 
		if (digits_in_words[index] == x || digits[index] == x) 
			return index;
			
	return incorrect_result;
}

double calculate(char operation, int first, int second) {
	switch(operation) {
		case '+':
			return first + second;
		case '-':
			return first - second;
		case '*':
			return first * second;
		case '/':
			if (second != 0) 
				return double(first) / second;
			else
				cerr << "In integer division can not divide by 0" << "\n";
				return incorrect_result;
		case '%':
			if (second != 0) 
				return first % second;
			else
				cerr << "In integer modulo division can not divide by 0" << "\n";
				return incorrect_result;
		default:
			cerr << "operation is unrecognized" << "\n";
			return incorrect_result;		
	}
}

int main() 
{
	char operation;
	string op1;
	string op2;
	int operand1;
	int operand2;
	double result;
	
	cout << "Enter two digits and operation: ";
	while ( cin >> op1 && cin >> op2 && cin >> operation ) {
		operand1 = get_index(op1);
		operand2 = get_index(op2);
		if (incorrect_result == operand1 || incorrect_result == operand2)
			cerr << "Incorrect value of operand. Operands must be digits or digits in words.\n";
		else {
			result = calculate(operation, operand1, operand2);
			if (result != incorrect_result)
				cout << result << "\n";
		}
		cout << "Enter two digits and operation: ";
	}
}
