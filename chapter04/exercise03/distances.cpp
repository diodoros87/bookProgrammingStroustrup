#include <iostream>
#include <vector>

using namespace std;

bool is_empty_vector(vector<double> values) {	
	unsigned int size = values.size();
	if (size == 0) {
		cerr << "No values \n";
		return true; 
	}
	
	return false;
}

double calculate_sum(vector<double> values) {	
	double sum = 0;
	for (double v : values)
		sum += v;
		
	return sum;
}

double calculate_average(vector<double> values) {
	if (true == is_empty_vector(values))
		return 0; 
	
	double average = calculate_sum(values) / values.size();
	return average;
}

double find_max(vector<double> values) {	
	if (true == is_empty_vector(values))
		return 0;
		
	double max = values[0];
	
	for (unsigned int i = 1; i < values.size(); i++)
		if (values[i] > max)
			max = values[i];
		
	return max;
}

double find_min(vector<double> values) {	
	if (true == is_empty_vector(values))
		return 0;
		
	double min = values[0];
	
	for (unsigned int i = 1; i < values.size(); i++)
		if (values[i] < min)
			min = values[i];
		
	return min;
}

	
int main() {
	vector<double> distances;
	double x;
	
	cout << "Enter distance: \n";
	while(cin >> x) {
		if (x >= 0)
			distances.push_back(x);
		cout << "Enter distance: \n";
	}
		
	if (distances.size() == 0) {
		cout << "No distance entered\n";
	}
	else {
		double sum = calculate_sum(distances);
		double min = find_min(distances);
		double max = find_max(distances);
		double average = calculate_average(distances);
			
		cout << "sum of distances is " << sum << endl;
		cout << "min of distances is " << min << endl;
		cout << "max of distances is " << max << endl;
		cout << "average of distances is " << average << endl;
	}
}
