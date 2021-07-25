#include <iostream>
using namespace std;

int main() 
{
	string operation;
	double operand1 = 0;
	double operand2 = 0;
	bool numbers = true;
	
	cout << "Enter operation before two operands (for exit type: \"exit\"): ";
	
	while ( cin >> operation && "exit" != operation && true == numbers ) {
		if (! (cin >> operand1) || ! (cin >> operand2)) {
			cout << "Incorrect value of operand. Operands must be number.";
			numbers = false;
		}
		else {
			if ("+" == operation || "plus" == operation) {
				cout << operand1 + operand2;
			}
			else if ("-" == operation || "minus" == operation) {
				cout << operand1 - operand2;
			}
			else if ("*" == operation || "multiply" == operation) {
				cout << operand1 * operand2;
			}
			else if ("/" == operation || "divide" == operation) {
				cout << operand1 / operand2;
			}
			else if ("%" == operation || "modulo" == operation) {
				if ((int)operand2 == 0)
					cout << "In integer modulo division can not divide by 0";
				else 
					cout << (int)operand1 % (int)operand2;
			}
			else {
				cout << "operation is unrecognized";
			}
			
			cout << "\n";
			cout << "Enter operation before two operands (for exit type: \"exit\"): ";
		}
		
	}
	
	cout << '\n';
}
