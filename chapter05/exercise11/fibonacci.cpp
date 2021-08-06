#include <iostream>
#include <vector>
#include <climits>
#include <limits>

using namespace std;

void print_max_int() {
	constexpr int max_int = numeric_limits<int>::max();
	cout << "Max value of type int - numeric_limits<int>::max() - is " << max_int << endl;
	cout << "Max value of type int - INT_MAX - is " << INT_MAX << endl;
}

int get_value(string prompt) {
	cout << prompt;
	
	int value;
	cin >> value;
	if (!cin) 
		throw runtime_error("Final condition: entered value must be type int");
	
	return value;
}

int get_number_of_integers() {
	int value = get_value("Enter number of these integers to display: ");
	
	if (value < 0)
		throw runtime_error("Final condition: number of integers to display < 0");
		
	return value;
}

void print_program_info() {
	cout << "To quit enter non-number value \n";
	cout << "Program displays some of first integers in Fibonacci sequence\n";
	cout << "Number of these integers is specified by User \n";
}

void check_preconditions(vector<int>& integers, int number) {
	if (number <= 0) 
		throw runtime_error("Precondition: number of first integers to display sum must be > 0");
	
	const unsigned int size = integers.size();
	if (size == 0)
		throw runtime_error("Precondition: number of all integers to display must be > 0");
		
	if (number > size) 
		throw runtime_error(
				"Precondition: Number of integers to display must be <= number of all integers \n");
}

void calculate_fibonacci(vector<int>& sequence) {
	sequence.clear();
	sequence.push_back(1);
	sequence.push_back(1);
	
	unsigned int last_index = sequence.size() - 1;
	int element = 2; 
	do {
		sequence.push_back(element);
		last_index++;
		element = sequence[last_index] + sequence[last_index - 1];
	} 	while (element > sequence[last_index]);

	cout << "Max integer of type int in Fibonacci sequence is " 
		<< sequence[last_index] << endl;
	cout << "Sum of 2 max integers of type int in Fibonacci sequence is " <<
		sequence[last_index] << " + " << sequence[last_index - 1]
		<< " = " << element << endl;
	long max_int_as_long = long(sequence[last_index]) + long(sequence[last_index - 1]);
	cout << "As type long sum of 2 max integers of type int in Fibonacci sequence is " <<
		sequence[last_index] << " + " << sequence[last_index - 1]
		<< " = " << max_int_as_long << endl;
	print_max_int();
}

void print_first_elements(vector<int>& integers, int number) {
	check_preconditions(integers, number);
		
	cout << "\n" << number << " of first integers in Fibonacci sequence: " 
		<< integers[0];
	
	for (unsigned int i = 1; i < number - 1; i++)
		cout << ", " << integers[i];
		
	if (number > 1)
		cout << " and " << integers[number - 1];
		
	cout << endl;
}

void print(vector<int>& integers) {
	const unsigned int size = integers.size();
	
	cout << "\n\nAll integers as type int in Fibonacci sequence: \n";
	for (unsigned int i = 0; i < size; i++)
		cout << "[" << i << "] = " << integers[i] << endl;
	cout << "Number of all integers as type int in Fibonacci sequence is " << size << "\n";
		
	cout << endl << endl << endl;
}

int main() { 
	vector<int> fibonacci;
	calculate_fibonacci(fibonacci);
	print(fibonacci);
	print_program_info();
	
	int integers_to_display = get_number_of_integers();
	if (integers_to_display <= 0) {
		cout << "Number of integers to display must be > 0 \n";
		return 0;
	}
	
	print_first_elements(fibonacci, integers_to_display);
}
