#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


double calculate_median(vector<double> values) {
	unsigned int size = values.size();
	if (size == 0) {
		cerr << "No values \n";
		return 0; 
	}
	else if (size % 2 == 1)
		return values[size / 2];
	else {
		return (values[size / 2 - 1] + values[size / 2]) / 2;
	}
}

	
int main() {
	vector<double> temps;
	double temp;
	
	while(cin >> temp) 
		temps.push_back(temp);
		
	if (temps.size() == 0) {
		cout << "No temperature entered\n";
	}
	else {
		double sum = 0;
		for (double i : temps)
			sum += i;
			
		cout << "Average of temperature is " << sum / temps.size() << endl;
		sort(temps.begin(), temps.end());
		
		cout << "Median of temperature is: " << calculate_median(temps) << endl;
	}
}
