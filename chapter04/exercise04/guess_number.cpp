#include <iostream>

using namespace std;

constexpr char quit = 'q';
constexpr char yes = 'y';
constexpr char no = 'n';

void print_options() {
	cout << "Press :\n";
	cout << quit << " to quit " << endl;
	cout << yes << " to yes " << endl;
	cout << no << " to no " << endl;
}
	
int main() {
	constexpr int begin = 1;
	constexpr int end = 100;
	
	char option = 0;
	
	cout << "Think about number from " << begin << " to " << end << endl;
	cout << "Press " << quit << " to quit or any key to continue " << end << endl;
	if (!(cin >> option) || quit == option) {
		return 1;
	}
	
	int first = begin;
	int last = end;
	int number;
	
	for(unsigned int i = 1; last != first; i++) {
		number = (first + last) / 2;
		cout << "Question number " << i << endl; 
		cout << "Are You thinking about number <= than " << number << " ?\n" ;
		print_options();
		if (!(cin >> option) )
			return 1;

		if (yes == option)
			last  = number;
		else if (no == option)
			first = number + 1;
		else if (quit == option)
			return 1;
		else {
			cerr << "Unrecognized option\n";
			i--;
		}
	}
	
	if (last == first)
		cout << "Number is " << first << endl;
}
