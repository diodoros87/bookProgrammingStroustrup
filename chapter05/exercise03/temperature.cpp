#include <iostream>

using namespace std;

constexpr double min_Celsius_temperature = -273.15;

double ctok(double c)
{
	double k = c - min_Celsius_temperature;

	return k;
}

int main() {
	double c = 0;
	cout << "Enter temperature in Celsius to get tempterature in Kelvin: ";
	cin >> c;
	if (cin) {
		if (c < min_Celsius_temperature) {
			cerr << "Error:  minimal temperature is " << min_Celsius_temperature;
		}
		else {
			double k = ctok(c);
			cout << k << endl;
		}
	}
	else
		cerr << "Entered value is not type double\n";
}


