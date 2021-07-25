#include <iostream>
using namespace std;

int main() 
{
	string value;
	
	cout << "Enter number literally (for exit type: \"exit\"): ";
	
	while ( cin >> value &&  "exit" != value) {
		if ("zero" == value) {
			cout << "0";
		}
		else if ("one" == value) {
			cout << "1";
		}
		else if ("two" == value) {
			cout << "2";
		}
		else if ("three" == value) {
			cout << "3";
		}
		else if ("four" == value) {
			cout << "4";
		}
		else {
			cout << "String is unrecognized";
		}
		
		cout << "\n";
		cout << "Enter number literally: ";
	}
	
	cout << '\n';
}
