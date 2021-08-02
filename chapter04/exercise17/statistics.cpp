#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print(vector<string> v, string title) {
	cout << title << endl;
	for (string s : v)
		cout << s << " ";
	cout << "\n Number of elements: " << v.size() << endl;
}

vector<string> get_mode(vector<string> v) {	
	vector<string> max_frequency_more_one;
	int max = 1;
	int frequency = 1;
	sort(v.begin(), v.end());
	for (unsigned int i = 1; i < v.size(); i++) {
		if (v[i] != v[i - 1]) 
			frequency = 1;
		else {
			frequency++;
			if (frequency == max) 
				max_frequency_more_one.push_back(v[i]);
			else if (frequency > max) {
				max_frequency_more_one.clear();
				max = frequency;
				max_frequency_more_one.push_back(v[i]);
			}
		}
	}
	
	if (max == 1)
		return v;
	
	return max_frequency_more_one;
}

vector<string> get_max(vector<string> v) {	
	const unsigned int size = v.size();
	if (0 == size)
		return v;
		
	vector<string> max {v[0]};
	for (unsigned int i = 1; i < size; i++) {
		if (max[0] == v[i]) 
			max.push_back(v[i]);
		else if (max[0] < v[i]) {
			max.clear();
			max.push_back(v[i]);
		}
	}
	
	return max;
}

string get_extreme_after_sort(vector<string> v, char extreme) {	
	const unsigned int size = v.size();
	if (0 == size)
		return "";
		
	sort(v.begin(), v.end());	
	switch(extreme) {
		case 'x':   // max
			return v[size - 1];
		case 'n':   // min
			return v[0];
		default:
			return "";
	}
}

vector<string> get_min(vector<string> v) {	
	const unsigned int size = v.size();
	if (0 == size)
		return v;
		
	vector<string> min {v[0]};
	for (unsigned int i = 1; i < size; i++) {
		if (min[0] == v[i]) 
			min.push_back(v[i]);
		else if (min[0] > v[i]) {
			min.clear();
			min.push_back(v[i]);
		}
	}
	
	return min;
}

void print_statistics(vector<string> v, string title) {
	cout << endl << endl <<  title << endl;
	
	vector<string> mode = get_mode(v);
	print(mode, "Mode as vector is: ");
	
	vector<string> max = get_max(v);
	print(max, "\nMax as vector is: ");
	
	cout << " Max is: " <<  get_extreme_after_sort(v, 'x') << endl;
	
	vector<string> min = get_min(v);
	print(min, "\nMin as vector is: ");
	
	cout << " Min is: " <<  get_extreme_after_sort(v, 'n') << endl;
}

int main() {
	vector<string> set_0;   // empty
	vector<string> set_1 = {"1", "2", "13", "4", "15", "6"};   // all
	vector<string> set_2 = {"1", "1", "2", "13", "4", "15", "6"}; // 1
	vector<string> set_3 = {"1", "1", "2", "13", "13", "4", "15", "6"};  // 1 13
	vector<string> set_4 = {"1", "1", "2", "13", "13", "4", "15", "15", "6", "6"};  // 1 6 13  15
	vector<string> set_5 = {"1", "1", "2", "13", "13", "4", "4", "4", "15", "6", "6"};   // 4
	vector<string> set_6 = {"1", "1", "1", "2", "13", "13", "4", "4", "4", "15", "6", "6"};  // 1 4
	vector<string> set_7 = {"1", "1", "1", "2", "13", "13", "4", "4", "4", "15", "6", "6", "18", "18", "18"};  // 1 4  18
	vector<string> set_8 = {"1", "1", "1", "2", "13", "13", "4", "4", "4", "15", "6", "6", "18", "18", "18", "18"};  //  18
	
	vector<string> misc = {"Eric", "Diodor", "Diogenes", "Marcus", "mortalis", "meditare mortem", "Mars", "Zeus", "Demonax"}; 
	vector<string> misc_1 = {"Eric", "Diodor", "Diogenes", "Marcus", "mortalis", "meditare mortem", "Mars", "Zeus", "Demonax",
									"meditare mortem", "meditare mortem", "Deus", "Deo Optimo Maximo", "Deo Optimo Maximo", 
									"Apollo", "Apollo", "Apollo", "Poseidon", "mortalis", "mortalis", "mortalis" }; 

	print_statistics(set_0, "Set 0");
	print_statistics(set_1, "Set 1");
	print_statistics(set_2, "Set 2");
	print_statistics(set_3, "Set 3");
	print_statistics(set_4, "Set 4");
	print_statistics(set_5, "Set 5");
	print_statistics(set_6, "Set 6");
	print_statistics(set_7, "Set 7");
	print_statistics(set_8, "Set 8");
	print_statistics(misc, "Misc");
	print_statistics(misc_1, "Misc 1");
}
