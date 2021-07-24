#include <iostream>
using namespace std;

int main() 
{
	string val1 = "";
	string val2 = "";
	string val3 = "";
	
	cout << "Enter three words: ";
	
	cin >> val1 >> val2 >> val3;
	
	string separator = ", ";
	
	string result = "";
	
	if (val1 <= val2) {
		if (val1 <= val3) {
			result = val1 + separator;
			
			if (val2 <= val3)
				result += val2 + separator + val3;
			if (val2 > val3)
				result += val3 + separator + val2;
		}
	}
	
	if (val2 < val1) {
		if (val2 <= val3) {
			result = val2 + separator;
			
			if (val1 <= val3)
				result += val1 + separator + val3;
			if (val1 > val3)
				result += val3 + separator + val1;
		}
	}
	
	if (val3 < val1) {
		if (val3 < val2) {
			result = val3 + separator;
			
			if (val1 <= val2)
				result += val1 + separator + val2;
			if (val1 > val2)
				result += val2 +separator + val1;
		}
	}
	
	cout << result << '\n';
}
