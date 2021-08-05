#include <iostream>

using namespace std;

void error(string s, double x) {
	cerr << s << x << endl;
	throw runtime_error("");
}

void error(string s, char c) {
	cerr << s << c << endl;
	throw runtime_error("");
}

double ktoc(double k)
{
	constexpr double min_Kelvin = 0;

	if (k < min_Kelvin)
		error("Precondition:  minimal temperature is ", min_Kelvin);
	
	constexpr double min_Celsius = -273.15;
	double c = k + min_Celsius;

	return c;
}

double ctok(double c)
{
	constexpr double min_Celsius = -273.15;

	if (c < min_Celsius) 
		error("Precondition:  minimal temperature is ", min_Celsius);
	
	double k = c - min_Celsius;

	return k;
}

double calculate_temperature(double temperature, char scale) {
	double result;
	switch(scale) {
		case 'c':
			result = ctok(temperature);
			break;
		case 'k':
			result = ktoc(temperature);
			break;
		default:
			error("unrecognized scale of temperature", scale);
	}
	
	return result;
}

double convert_temperature(char scale) {
	switch(scale) {
		case 'c':
			cout << "Enter temperature in Celsius to get temperature in Kelvin: ";
			break;
		case 'k':
			cout << "Enter temperature in Kelvin to get temperature in Celsius: ";
			break;
		default:
			error("unrecognized scale of temperature ", scale);
	}
	
	double t = 0;
	cin >> t;
	if (!cin) 
		throw runtime_error("Entered value is not type double");
	else {
		double result = calculate_temperature(t, scale);
		return result;
	}
}

int main() {
	double result = 0;
	result = convert_temperature('c');
	cout << result << endl;
	result = convert_temperature('k');
	cout << result << endl;
	result = convert_temperature('a');
	cout << result << endl;
}


