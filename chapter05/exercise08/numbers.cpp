#include <iostream>
#include <vector>

using namespace std;

int get_value(string prompt) {
	cout << prompt;
	
	int value;
	cin >> value;
	if (!cin) 
		throw runtime_error("Final condition: entered value must be type int");
	
	return value;
}

class Non_Number_of_Integers {};

int get_number_of_integers() {
	int value;
	
	try {
		value = get_value("Enter number of these integers to calculate their sum: ");
	}
	catch (runtime_error& e) {
		throw Non_Number_of_Integers();
	}
	
	if (value < 0)
		throw runtime_error("number of integers to sum < 0");
		
	return value;
	
}

void get_integers(vector<int>& integers) {
	integers.clear();
	
	bool is_program_continue = true;
	int integer;
	while(is_program_continue) {
		try {
			integer = get_value("To end of data - enter non-number value. Enter integer: ");
			integers.push_back(integer);
		}
		catch (runtime_error& e) {
			is_program_continue = false;
		}
	}
}

void print_program_info() {
	cout << "To quit enter non-integer value \n";
	cout << "Program calculate sum of first integers \n";
	cout << "Number of these integers is specified by User \n";
}

void check_preconditions(vector<int>& integers, int number) {
	if (number <= 0) 
		throw runtime_error("Precondition: number of first integers to calculate sum must be > 0");
	
	const unsigned int size = integers.size();
	if (size == 0)
		throw runtime_error("Precondition: number of all integers to calculate sum must be > 0");
		
	if (number > size) 
		throw runtime_error(
				"Precondition: Number of integers to calculate sum must be <= number of all integers \n");
}

int sum_first_elements(vector<int>& integers, int number) {
	check_preconditions(integers, number);
		
	int sum = 0;
	for (unsigned int i = 0; i < number; i++)
		sum += integers[i];
		
	return sum;
}

void print_first_elements(vector<int>& integers, int number, int sum) {
	check_preconditions(integers, number);
		
	cout << "Sum " << number << " of integers (" << integers[0];
	
	for (unsigned int i = 1; i < number - 1; i++)
		cout << ", " << integers[i];
		
	if (number > 1)
		cout << " and " << integers[number - 1];
		
	cout << ") is " << sum << endl;
}

int main() { 
	print_program_info();
	
	int integers_to_sum;
	try {
		integers_to_sum = get_number_of_integers();
	}
	catch (Non_Number_of_Integers& e) {
		return 0;
	}
	
	if (integers_to_sum <= 0) {
		cout << "Number of integers to calculate sum must be > 0 \n";
		return 1;
	}
	
	vector<int> integers;
	get_integers(integers);
	int sum = sum_first_elements(integers, integers_to_sum);
	
	print_first_elements(integers, integers_to_sum, sum);
}
