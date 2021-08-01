#include <iostream>
#include <vector>

using namespace std;

vector<short> get_primes(short end) {
	vector<short> primes;
	if (end < 2)
		return primes;
		
	primes.push_back(2);
	bool is_prime;
	short i;
	for (short number = 3; number <= end; number += 2) {
		is_prime = true;
		i = 0;
		while (true == is_prime && i < primes.size() && primes[i] * primes[i] <= number) {
			if (number % primes[i] == 0)
				is_prime = false;
			i++;
		}
		
		if (is_prime == true)
			primes.push_back(number);
	}
	
	return primes;
}

vector<short> get_primes(short begin, short end) {
	vector<short> result;
	if (begin > end || end < 2)
		return result;
		
	if (begin <= 2) {
		begin = 3;
		result.push_back(2);
	}
	else if (begin % 2 == 0) 
		begin++;
		
	if (end % 2 == 0) 
		end--;
		
	vector<short> primes = get_primes(end);
	for (short number = begin; number <= end; number += 2) {
		for (short prime : primes) 
			if (number == prime)
				result.push_back(prime); 
	}
	
	return result;
}

int main() {		
	short begin = 1;
	short end   = 100;
	
	cout << "This program search prime number from begin number to last number\n";
	cout << "Begin number and last number are included to range\n";
	cout << "Enter begin of range: ";
	cin >> begin;
	cout << "Enter end of range: ";
	cin >> end;
	
	vector<short> primes = get_primes(begin, end);
	
	cout << "\n\nPrimes numbers from " << begin << " to " << end << endl;
	for (short prime : primes) 
		cout << prime << " "; 
	
	cout << endl;
}
