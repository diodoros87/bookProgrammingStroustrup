#include <iostream>
#include <vector>

using namespace std;

double get_value(string prompt) {
	cout << prompt;
	
	double value;
	cin >> value;
	if (!cin) 
		throw runtime_error("Final condition: entered value must be type double");
	
	return value;
}

class Non_Number_of_Integers {};

int get_number_of_numbers() {
	int value;
	
	cout << "Enter number of these numbers to calculate their sum: ";
	cin >> value;
	if (!cin) 
		throw Non_Number_of_Integers();
	
	if (value < 0)
		throw runtime_error("number of numbers to sum < 0");
		
	return value;
}

void get_numbers(vector<double>& numbers) {
	numbers.clear();
	
	bool is_program_continue = true;
	double n;
	while(is_program_continue) {
		try {
			n = get_value("To end of data - enter non-number value. Enter number: ");
			numbers.push_back(n);
		}
		catch (runtime_error& e) {
			is_program_continue = false;
		}
	}
}

void print_program_info() {
	cout << "To quit enter non-number value \n";
	cout << "Program calculate sum of first numbers \n";
	cout << "Number of these numbers is specified by User \n";
}

void check_preconditions(vector<double>& numbers, int n) {
	if (n <= 0) 
		throw runtime_error("Precondition: number of first numbers to calculate sum must be > 0");
	
	const unsigned int size = numbers.size();
	if (size == 0)
		throw runtime_error("Precondition: number of all numbers to calculate sum must be > 0");
		
	if (n > size) 
		throw runtime_error(
				"Precondition: Number of numbers to calculate sum must be <= number of all numbers \n");
}

double sum_first_elements(vector<double>& numbers, int n) {
	check_preconditions(numbers, n);
		
	double sum = 0;
	for (unsigned int i = 0; i < n; i++)
		sum += numbers[i];
		
	return sum;
}

vector<double> get_first_elements_differences(vector<double>& numbers, int n) {
	check_preconditions(numbers, n);
	
	vector<double> result(n - 1);
	for (unsigned int i = 0; i < result.size(); i++)
		result[i] = numbers[i + 1] - numbers[i];
		
	return result;
}

void print_first_elements(vector<double>& numbers, int n) {
	check_preconditions(numbers, n);
	
	cout << numbers[0];
	
	for (unsigned int i = 1; i < n - 1; i++)
		cout << ", " << numbers[i];
		
	if (n > 1)
		cout << " and " << numbers[n - 1];
}

void print_first_elements(vector<double>& numbers, int n, double sum) {
	check_preconditions(numbers, n);
		
	cout << "Sum " << n << " of numbers (";
	
	print_first_elements(numbers, n);
		
	cout << ") is " << sum << endl;
}

void print_first_elements_differences(vector<double>& numbers, int n) {
	check_preconditions(numbers, n);
		
	vector<double> differences = get_first_elements_differences(numbers, n);
	const unsigned int size = differences.size();
	if (0 == size)
		return;
	
	cout << "Differences between first of adjacent " << n << " numbers (";
	print_first_elements(differences, size);
	cout << ")\n";
}

void process_numbers(int numbers_to_sum) {
	vector<double> numbers;
	get_numbers(numbers);
	double sum = sum_first_elements(numbers, numbers_to_sum);
	
	print_first_elements(numbers, numbers_to_sum, sum);
	print_first_elements_differences(numbers, numbers_to_sum);
}

int main() { 
	print_program_info();
	
	int numbers_to_sum;
	try {
		numbers_to_sum = get_number_of_numbers();
	}
	catch (Non_Number_of_Integers& e) {
		return 0;
	}
	
	if (numbers_to_sum <= 0) {
		cout << "Number of numbers to calculate sum must be > 0 \n";
		return 1;
	}
	
	process_numbers(numbers_to_sum);
}
