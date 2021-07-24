#include <iostream>
using namespace std;

int main() 
{
	int val1 = 0;
	int val2 = 0;
	
	cout << "Enter two integers: ";
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
	
	if (val2 != 0)
		cout << val1 << " / " << val2 << " is " << val1 / val2 << '\n';
	if (val2 != val1 && val2 != -val1 && val1 != 0)
		cout << val2 << " / " << val1 << " is " << val2 / val1 << '\n';
	
	cout << '\n';
}
