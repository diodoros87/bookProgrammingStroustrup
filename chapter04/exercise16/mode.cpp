#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print_numbers(vector<int> numbers, string title) {
	cout << title << endl;
	for (int n : numbers)
		cout << n << " ";
	cout << "\n Number of numbers: " << numbers.size() << endl;
}

vector<int> get_mode(vector<int> numbers) {	
	vector<int> max_frequency_more_one;
	int max = 1;
	int frequency = 1;
	sort(numbers.begin(), numbers.end());
	for (int i = 1; i < numbers.size(); i++) {
		if (numbers[i] != numbers[i - 1]) 
			frequency = 1;
		else {
			frequency++;
			if (frequency == max) 
				max_frequency_more_one.push_back(numbers[i]);
			else if (frequency > max) {
				max_frequency_more_one.clear();
				max = frequency;
				max_frequency_more_one.push_back(numbers[i]);
			}
		}
	}
	
	if (max == 1)
		return numbers;
	
	return max_frequency_more_one;
}

void print_mode(vector<int> numbers) {
	vector<int> mode = get_mode(numbers);
	print_numbers(mode, "Mode is: ");
}

int main() {
	vector<int> set_0;   // empty
	vector<int> set_1 = {1, 2, 13, 4, 15, 6};   // all
	vector<int> set_2 = {1, 1, 2, 13, 4, 15, 6}; // 1
	vector<int> set_3 = {1, 1, 2, 13, 13, 4, 15, 6};  // 1 13
	vector<int> set_4 = {1, 1, 2, 13, 13, 4, 15, 15, 6, 6};  // 1 6 13  15
	vector<int> set_5 = {1, 1, 2, 13, 13, 4, 4, 4, 15, 6, 6};   // 4
	vector<int> set_6 = {1, 1, 1, 2, 13, 13, 4, 4, 4, 15, 6, 6};  // 1 4
	vector<int> set_7 = {1, 1, 1, 2, 13, 13, 4, 4, 4, 15, 6, 6, 18, 18, 18};  // 1 4  18
	vector<int> set_8 = {1, 1, 1, 2, 13, 13, 4, 4, 4, 15, 6, 6, 18, 18, 18, 18};  //  18
	/*
	vector<int> natural_numbers;
	int n;
	
	cout << "Enter integers > 0 : \n";
	while(cin >> n) 
		if (n > 0) 
			natural_numbers.push_back(n);
	*/		
	//print_numbers(natural_numbers, "Entered numbers: ");
	//sort(natural_numbers.begin(), natural_numbers.end());
	//print_numbers(natural_numbers, "After sort entered numbers: ");

	print_mode(set_0);
	print_mode(set_1);
	print_mode(set_2);
	print_mode(set_3);
	print_mode(set_4);
	print_mode(set_5);
	print_mode(set_6);
	print_mode(set_7);
	print_mode(set_8);

}
