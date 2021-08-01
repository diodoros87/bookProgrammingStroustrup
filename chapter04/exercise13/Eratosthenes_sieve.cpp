#include <iostream>
#include <vector>

using namespace std;

vector<short> get_primes(short last) {
	vector<short> primes;
	if (last < 2)
		return primes;
	
	vector<bool> markers;	
	markers.push_back(false);  // primes[0] == false
	markers.push_back(false);  // primes[1] == false
	
	for (short i = 2; i <= last; i++) 
		markers.push_back(true);
		
	for (short i = 2; i <= last; i++) 
		if (true == markers[i]) 
			for (short j = i + i; j <= last; j += i) 
				markers[j] = false;
				
	for (short i = 2; i <= last; i++) 
		if (true == markers[i]) 
			primes.push_back(i);			
	
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
	constexpr short begin = 1;
	constexpr short end   = 100;
	
	vector<short> primes = get_primes(begin, end);
	
	cout << "\n\nPrimes numbers from " << begin << " to " << end << endl;
	for (short prime : primes) 
		cout << prime << " "; 
	
	cout << endl;
}
