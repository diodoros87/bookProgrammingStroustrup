#include <iostream>
using namespace std;

int main() 
{
	int val1 = 0;
	int val2 = 0;
	int val3 = 0;
	
	cout << "Enter three integers: ";
	
	cin >> val1 >> val2 >> val3;
	
	char separator = ',';
	
	if (val1 <= val2) {
		if (val1 <= val3) {
			cout << val1 << separator;
			
			if (val2 <= val3)
				cout << val2 << separator << val3;
			if (val2 > val3)
				cout << val3 << separator << val2;
		}
	}
	
	if (val2 < val1) {
		if (val2 <= val3) {
			cout << val2 << separator;
			
			if (val1 <= val3)
				cout << val1 << separator << val3;
			if (val1 > val3)
				cout << val3 << separator << val1;
		}	
	}
	
	if (val3 < val1) {
		if (val3 < val2) {
			cout << val3 << separator;
			
			if (val1 <= val2)
				cout << val1 << separator << val2;
			if (val1 > val2)
				cout << val2 << separator << val1;
		}
	}
	
	cout << '\n';
}
