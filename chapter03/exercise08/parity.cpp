#include <iostream>
using namespace std;

int main() 
{
	int n = 0;
	
	cout << "Enter integer number: ";
	cin >> n;
	
	string parity = "even";
	int rest = n % 2;
	if (rest == 1 || -1 == rest) {
		parity = "odd";
	}
	
	cout << "Number " << n << " is " << parity << '\n';
}
