#include <iostream>

using namespace std;

double ctok(double c)
{
	constexpr double min_Celsius_temperature = -273.15;

	if (c < min_Celsius_temperature) {
		cerr << "Precondition:  minimal temperature is " << min_Celsius_temperature << endl;
		throw runtime_error("");
	}
	
	double k = c - min_Celsius_temperature;

	return k;
}

int main() {
	double c = 0;
	cout << "Enter temperature in Celsius to get temperature in Kelvin: ";
	cin >> c;
	if (cin) {
		double k = ctok(c);
		cout << k << endl;
	}
	else
		cerr << "Entered value is not type double\n";
}


