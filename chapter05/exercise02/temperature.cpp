#include <iostream>

using namespace std;

double ctok(double c)
{
	constexpr double constant = 273.15;
	double k = c + constant;

	return k;
}

int main() {
	double c = 0;
	
	cout << "Enter temperature in Celsius to get tempterature in Kelvin: ";
	cin >> c;
	if (cin) {
		double k = ctok(c);
		cout << k << endl;
	}
	else
		cerr << "Entered value is not type double\n";
}


