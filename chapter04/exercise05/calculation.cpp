#include <iostream>
using namespace std;

int main() {
	char operation;
	double operand1;
	double operand2;
	
	cout << "Enter two numbers and operation: ";
	while ( cin >> operand1 && cin >> operand2 && cin >> operation ) {
		switch(operation) {
			case '+' :
				cout << "Sum is " << operand1 + operand2;
				break;
			case '-' :
				cout << "Difference is " << operand1 - operand2;
				break;
			case '*' :
				cout << "Product is " << operand1 * operand2;
				break;
			case '/' :
				cout << "Quotient is " << operand1 / operand2;
				break;
			case '%' :
				if ((int)operand2 == 0)
					cout << "In integer modulo division can not divide by 0";
				else 
					cout << "Remainder of integer modulo division is " << (int)operand1 % (int)operand2;
				
				break;
			default:
				cout << "operation is unrecognized";
		}
			
			cout << "\n";
			cout << "Enter two numbers and operation: ";
	}
		
	
	
	cout << '\n';
}
