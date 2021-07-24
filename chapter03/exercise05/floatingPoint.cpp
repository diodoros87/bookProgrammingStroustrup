#include <iostream>
using namespace std;

int main() 
{
	double val1 = 0;
	double val2 = 0;
	
	cout << "Enter two floating-point numbers: ";
	cin >> val1 >> val2;
	
	string comparison = " == ";
	
	if (val1 > val2)
		comparison = " > ";
	if (val1 < val2)
		comparison = " < ";
	
	cout << val1 << comparison << val2 << '\n';
	
	cout << val1 << " + " << val2 << " is " << val1 + val2 << '\n';
	
	cout << val1 << " - " << val2 << " is " << val1 - val2 << '\n';
	if (val2 != val1)
		cout << val2 << " - " << val1 << " is " << val2 - val1 << '\n';
		
	cout << val1 << " * " << val2 << " is " << val1 * val2 << '\n';
	
	cout << val1 << " / " << val2 << " is " << val1 / val2 << '\n';
	if (val2 != val1 && val2 != -val1)
		cout << val2 << " / " << val1 << " is " << val2 / val1 << '\n';
	
	cout << '\n';
}
