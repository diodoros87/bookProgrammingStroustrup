#include <iostream>
#include <vector>

using namespace std;

vector<short> get_primes(short number_of_primes) {
	vector<short> primes;
	if (number_of_primes < 1)
		return primes;
		
	primes.push_back(2);
	bool is_prime;
	short i;
	for (short number = 3; primes.size() < number_of_primes; number += 2) {
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

int main() {		
	short number;
	
	cout << "Enter number of smallest of prime numbers to search: ";
	cin >> number;
	
	vector<short> primes = get_primes(number);
	
	cout << "\n\n" << number << " of smallest of prime numbers :" << endl;
	for (short prime : primes) 
		cout << prime << " "; 
	
	cout << endl;
}
