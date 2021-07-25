#include <iostream>
using namespace std;

int main() 
{
	int max = 0;
	
	int cent1;
	cout << "How many 1 cent do you have? ";
	cin >> cent1;
	max = cent1;
	
	int cent2;
	cout << "How many 2 cents do you have? ";
	cin >> cent2;
	if (cent2 > max)
		max = cent2;
	
	int cent5;
	cout << "How many 5 cents do you have? ";
	cin >> cent5;
	if (cent5 > max)
		max = cent5;
	
	int cent10;
	cout << "How many 10 cents do you have? ";
	cin >> cent10;
	if (cent10 > max)
		max = cent10;
	
	int cent20;
	cout << "How many 20 cents do you have? ";
	cin >> cent20;
	if (cent20 > max)
		max = cent20;
	
	int cent50;
	cout << "How many 50 cents do you have? ";
	cin >> cent50;
	if (cent50 > max)
		max = cent50;
	
	int dollar;
	cout << "How many 1 dollars do you have? ";
	cin >> dollar;
	if (dollar > max)
		max = dollar;
		
	int chars_for_max = 1;
	int tmp = max;
	while (tmp >= 10) {
		tmp /= 10;
		chars_for_max++;
	}
	
	int chars = 1;
	tmp = cent1;
	while (tmp >= 10) {
		tmp /= 10;
		chars++;
	}
	int spaces = chars_for_max - chars;
	cout << "You have ";
	while(spaces-- > 0) 
		cout << " ";
	cout << cent1 << " coin";
	if (cent1 > 1)
		cout << "s";
	cout <<  " of 1 cent \n";
	
	
	chars = 1;
	tmp = cent2;
	while (tmp >= 10) {
		tmp /= 10;
		chars++;
	}
	spaces = chars_for_max - chars;
	cout << "You have ";
	while(spaces-- > 0) 
		cout <<  " ";
	cout <<  cent2 << " coin";
	if (cent2 > 1)
		cout <<  "s";
	cout <<  " of 2 cents \n";
	
	chars = 1;
	tmp = cent5;
	while (tmp >= 10) {
		tmp /= 10;
		chars++;
	}
	spaces = chars_for_max - chars;
	cout <<  "You have ";
	while(spaces-- > 0) 
		cout <<  " ";
	cout <<   cent5 << " coin";
	if (cent5 > 1)
		cout <<  "s";
	cout <<  " of 5 cents \n";
	
	chars = 1;
	tmp = cent10;
	while (tmp >= 10) {
		tmp /= 10;
		chars++;
	}
	spaces = chars_for_max - chars;
	cout <<  "You have ";
	while(spaces-- > 0) 
		cout <<  " ";
	cout <<   cent10 << " coin";
	if (cent10 > 1)
		cout <<  "s";
	cout <<  " of 10 cents \n";
	
	chars = 1;
	tmp = cent20;
	while (tmp >= 10) {
		tmp /= 10;
		chars++;
	}
	spaces = chars_for_max - chars;
	cout <<  "You have ";
	while(spaces-- > 0) 
		cout <<  " ";
	cout <<   cent20 << " coin";
	if (cent20 > 1)
		cout <<  "s";
	cout <<  " of 20 cents \n";
	
	chars = 1;
	tmp = cent50;
	while (tmp >= 10) {
		tmp /= 10;
		chars++;
	}
	spaces = chars_for_max - chars;
	cout <<  "You have ";
	while(spaces-- > 0) 
		cout <<  " ";
	cout <<   cent50 << " coin";
	if (cent50 > 1)
		cout <<  "s";
	cout <<  " of 50 cents \n";
	
	chars = 1;
	tmp = dollar;
	while (tmp >= 10) {
		tmp /= 10;
		chars++;
	}
	spaces = chars_for_max - chars;
	cout <<  "You have ";
	while(spaces-- > 0) 
		cout <<  " ";
	cout <<  dollar << " coin";
	if (dollar > 1)
		cout <<  "s";
	cout <<  " of dollar \n";

	int total_money = cent1 + 2 * cent2 + 5 * cent5 + 10 * cent10 + 20 * cent20 + 
							50 * cent50 + 100 * dollar;
	int total_dollars = total_money / 100;
	int rest_of_cents = total_money % 100;
	
	
	cout << " Total money is " << total_money << " cents " << '\n';
	cout << " Total money is " << total_dollars << ",";
	if (rest_of_cents < 10)
		cout << "0";
	cout << rest_of_cents << " dollars \n";
}
