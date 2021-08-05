#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

void error(string s, string sx, string sy, unsigned int x, unsigned int y) {
	cerr << s << endl;
	cerr << sx << x << endl;
	cerr << sy << y << endl;
	throw runtime_error("");
}

void print(vector<double>& numbers, vector<string> names) {
	const unsigned int size = numbers.size();
	if (size != names.size())
		error("Unequal sizes", "numbers.size() = ", "names.size() = ", 
										numbers.size(), names.size());
		
	for (unsigned int i = 0; i < size; i++)
		cout << names[i] << " = " << numbers[i] << "\n";
}

void print(vector<double>& solutions) {
	const unsigned int size = solutions.size();
	vector<string> names;
	
	switch(size) {
		case 1:
			names.push_back("x");
			break;
		case 2:
			names.push_back("x1");
			names.push_back("x2");
			break;
		default:
			return;
	}
	print(solutions, names);
}

bool is_identity_equation(double a, double b, double c) {
	if (a == 0 && b == 0 && c == 0)
		return true;
		
	return false;
}

bool is_conflicting_equation(double a, double b, double c) {
	if (a == 0 && b == 0 && c != 0)
		return true;
		
	return false;
}

void linear_equation(double b, double c, vector<double>& solutions) {
	solutions.clear();
	
	if (b != 0) {
		double x = -c / b;
		solutions.push_back(x);
	}
	else {
		if (c == 0)
			cout << "Equation has infinity number of solutions\n";	
		else
			cout << "Equation has no solution\n";	
	}	
}

void quadratic_equation(double a, double b, double c, vector<double>& solutions) {	
	solutions.clear();
	
	if (true == is_identity_equation(a, b, c)) {
		cout << "Equation has infinity number of solutions\n";	
		return;
	}
	if (true == is_conflicting_equation(a, b, c)) {
		cout << "Equation has no solution\n";
		return;
	}
	if (a == 0) {
		linear_equation(b, c, solutions);
		return;
	}
	
	double discriminant = b * b - 4 * a * c;
	if (0 > discriminant) {
		cout << "Equation has no real solution\n";
		return;
	}
	if (0 == discriminant) {
		double x = -b / (2 * a);
		solutions.push_back(x);
		return;
	}
	
	double x1 = (-b - sqrt(discriminant)) / (2 * a);
	double x2 = (-b + sqrt(discriminant)) / (2 * a);
	
	solutions.resize(2);
	solutions[0] = x1;
	solutions[1] = x2;
}

double getValue(string prompt) {
	cout << prompt;
	
	double value;
	cin >> value;
	if (!cin) {
		cout << "Entered value is not type double \n";
		throw runtime_error("");
	}
	
	return value;
}

int main() { 
	double a, b, c;
	a = b = c = 0;
	vector<double> solutions;
	
	cout << "Quadratic equation has formula: a * x * x + b * x + c = 0 \n";
	cout << "Enter values of coefficients (a, b, c) for quadratic equation: \n";
	cout << "To quit enter non-number value \n";
	bool is_program_continue = true;
	while(is_program_continue) {
		try {
			a = getValue("Enter value of coefficient a: ");
			b = getValue("Enter value of coefficient b: ");
			c = getValue("Enter value of coefficient c: ");
			quadratic_equation(a, b, c, solutions);
			print(solutions);
			cout << "\n\nQuadratic equation has formula: a * x * x + b * x + c = 0 \n";
			cout << "Enter values of coefficients (a, b, c) for quadratic equation: \n";
			cout << "To quit enter non-number value \n";
		}
		catch (runtime_error& e) {
			is_program_continue = false;
		}
	}
}
