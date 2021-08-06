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

bool is_lossy_conversion(double input_value) {
	int int_value = input_value;
	//double double_value = int_value;
	
	if (int_value == input_value)
		return false;
	else
		return true;
}

constexpr double min_Celsius = -273.15;

double ctof(double c)
{
	if (c < min_Celsius) 
		error("Precondition:  minimal temperature is ", min_Celsius);
	
	double f = 32 + 9 / 5.0 * c;
	
	bool lossy_conversion = is_lossy_conversion(f);
	if (true == lossy_conversion)
		error("\n Final condition: \
Lossy conversion has detected \nafter cast to int double value of ", f);

	return f;
}

const double min_Fahrenheit = 32 + 9 / 5.0 * min_Celsius;

double ftoc(double f)
{
	if (f < min_Fahrenheit)
		error("Precondition:  minimal temperature is ", min_Fahrenheit);
		
	double c = 5.0 / 9 * (f - 32);
	
	bool lossy_conversion = is_lossy_conversion(c);
	if (true == lossy_conversion)
		error("\n Final condition: \
Lossy conversion has detected \nafter cast to int double value of ", c);

	return c;
}

double calculate_temperature(double temperature, char scale) {
	double result;
	switch(scale) {
		case 'c':
			result = ctof(temperature);
			break;
		case 'f':
			result = ftoc(temperature);
			break;
		default:
			error("unrecognized scale of temperature", scale);
	}
	
	return result;
}

double convert_temperature(char scale) {
	switch(scale) {
		case 'c':
			cout << "Enter temperature in Celsius to get temperature in Fahrenheit: ";
			break;
		case 'f':
			cout << "Enter temperature in Fahrenheit to get temperature in Celsius: ";
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
	result = convert_temperature('f');
	cout << result << endl;
}


