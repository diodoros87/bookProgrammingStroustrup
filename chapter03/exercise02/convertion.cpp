#include <iostream>
using namespace std;

int main() 
{
	double miles = 0;
	
	cout << "Enter number of miles: ";
	while (cin >> miles) {
		cout << miles << " miles is " << miles * 1.609 << " kilometers \n";
		cout << "Enter number of miles: ";
	}
	
	cout << '\n';
}
