#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int square(int x) {
		return x*x;
}

int multiply(int a, int b) {
	int repetitions = 0;
	int sum = 0;
	
	if (b < 0) 
		repetitions = -b;
	else
		repetitions = b;
	
	for (int counter = 0; counter < repetitions; counter++) 
		sum += a;
	
	if (b < 0) 
		return -sum;
	else
		return sum;
}
/*
void main4() {
	bool is_numbers_readed = false;
	double a;
	constexpr double diff = 1.0 / 100;
	
	cout << "Enter number: " << endl;
	while (cin >> a && cin >> b) {
		is_numbers_readed = true;
		if (a == b)
			cout << a << " == " << b << endl;
		else {
			if (a > b) {
				cout << a << " > " << b << endl;
				if (a - b < diff)
					cout << a << " almost equal to " << b << endl;
			}
			else if (a < b) {
				cout << a << " < " << b << endl;
				if (b - a < diff)
					cout << a << " almost equal to " << b << endl;
			}
		}
		cout << "Enter number: " << endl;
	}
	
	cout <<  "max == " << max << endl;
	cout <<  "min == " << min << endl;
}
*/

const string m   = "m";
const string cm   = "cm";
const string inch = "inch";
const string foot = "foot";

double calculate_to_meter(double number, string unit) {
	if (number <= 0) {
		cerr << number << " <= 0 unacceptable length\n";
		return number;
	}
		
	constexpr int cm_per_metr = 100;
	constexpr double cm_per_inch = 2.54;
	constexpr int inch_per_foot = 12;

	if (m == unit)
		return number;
	else if (cm == unit)
		return number / cm_per_metr;
	else if (inch == unit)
		return cm_per_inch * number / cm_per_metr;
	else if (foot == unit)
		return inch_per_foot * cm_per_inch * number / cm_per_metr;
	else {
		cerr << unit << " unrecognized unit \n";
		return -1;
	}
}
	
int main() {
	double number;
	double length;
	string unit;
	double min, max;
	int counter;
	double sum;
	vector<double> values;
	
	cout << "Enter number and unit (m/cm/inch/foot): ";
	for (counter = 0, sum = 0; cin >> number && cin >> unit;) {
		
		length = calculate_to_meter(number, unit);
		
		if (length > 0) { 
			values.push_back(length);
			if (sum == 0) {
				min = max = length;
				cout <<  "max == " << max << " " << m << endl;
				cout <<  "min == " << min << " " << m << endl;
			}
			else if (length > max) {
				max = length;
				cout <<  "max == " << max << " " << m << endl;
			}
			else if (length < min) {
				min = length;
				cout <<  "min == " << min << " " << m << endl;
			}
			sum += length;
			counter++;
		}
		
		cout << "Enter number and unit (m/cm/inch/foot): ";
	}
	
	if (sum > 0) {
		cout <<  "\n number of lengths == " << counter << endl;
		cout <<  "sum of lengths == " << sum <<  endl;
		cout <<  "\n max == " << max << " " << m << endl;
		cout <<  "min == " << min << " " << m << endl;
		cout <<  "All values in meters: " << endl;
		sort(values.begin(), values.end());
		for (counter = 0; counter < values.size(); counter++)
			cout << values[counter] << "\n";
	}
	else
		cout << endl <<  "No number entered" << endl;
}

void words() {
	/*
	cout << multiply(3, -3) << '\n';
	cout << multiply(3, 3) << '\n';
	cout << multiply(4, -3) << '\n';
	cout << multiply(4, 5) << '\n';
	cout << multiply(-14, -5) << '\n';
	cout << multiply(0, 0) << '\n';
	cout << multiply(0, -0) << '\n';
	cout << multiply(10, 0) << '\n';
	cout << multiply(0, -10) << '\n';
	cout << multiply(33, 1) << '\n';
	cout << multiply(2, 2) << '\n';
	cout << multiply(10, 10) << '\n';
	cout << multiply(33, 3) << '\n';
	*/
	vector<string> words;
	string temp;
	
	while(cin >> temp) {
		if ("Broccoli" == temp || "Return" == temp) 
			words.push_back("Biip");
		else
			words.push_back(temp);
	}
	
	cout << "Number of words: " << words.size() << endl;
	sort(words.begin(), words.end());
	cout << "After sort Number of words: " << words.size() << endl;
	cout << "Words: " <<  endl;
	
	for (string s : words)
		cout << s << endl;
}
