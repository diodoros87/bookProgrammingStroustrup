#include <iostream>
#include <limits>

using namespace std;
	
int main() {
	double field_grains = 1;
	int      int_total_grains    = field_grains;
	long int long_total_grains   = field_grains;
	double   double_total_grains = field_grains;
	double   chessboard_total_grains = field_grains;
	short int field_int = 1;		
	short int field_long = 1;	
	short counter;			
	bool int_overflow = false;	 
	bool long_overflow = false;	 
	
	constexpr double double_max = numeric_limits<double>::max();
	
	for(counter = 1; double_total_grains <= double_max; counter++) {	
		cout << "field           " << counter << endl;
		cout << "grains on field     " << field_grains << endl;
		cout << "int_total_grains    " << int_total_grains << endl;
		cout << "long_total_grains   " << long_total_grains << endl;
		cout << "double_total_grains " << double_total_grains << endl << endl;
		
		if (64 == counter) {
			chessboard_total_grains = double_total_grains;
			cout << "\n\nOn chessboard are " << double_total_grains << " grains of rice.\n\n\n";
		}
		
		field_grains *= 2;
		if (false == int_overflow) {
			int next_int = int_total_grains + field_grains;
			if (int_total_grains >= next_int) {
				int_overflow = true;
				field_int = counter;
				cout << "\n\n\nFor field number " << field_int << " type int has maximum precise value.\n";
				cout << "For field number " << field_int + 1 << " type int has arithmetic overflow.\n\n\n";
			}
			int_total_grains = next_int;
		}
		if (false == long_overflow) {
			long next_long = long_total_grains + field_grains;
			if (long_total_grains >= next_long) {
				long_overflow = true;
				field_long = counter;
				cout << "\n\n\nFor field number " << field_long << " type long has maximum precise value.\n";
				cout << "For field number " << field_long + 1 << " type long has arithmetic overflow.\n\n\n";
			}
			long_total_grains = next_long;
		}
		
		double_total_grains += field_grains;
	}
	
	cout << "\n\nSUMMARY" << endl;
	cout << "On chessboard are " << chessboard_total_grains << " grains of rice.\n";
	cout << "\n\n\nFor field number " << field_int << " type int has maximum precise value.\n";
	cout << "For field number " << field_int + 1 << " type int has arithmetic overflow.\n\n\n";
	cout << "\n\n\nFor field number " << field_long << " type long has maximum precise value.\n";
	cout << "For field number " << field_long + 1 << " type long has arithmetic overflow.\n\n\n";
	cout << "For field number " << counter - 1 << " type double has maximum approximate value.\n\n\n";
	cout << "For field number " << counter << " type double has arithmetic overflow.\n\n\n";
	cout << "grains on field     " << field_grains << endl;
	cout << "int_total_grains    " << int_total_grains << endl;
	cout << "long_total_grains   " << long_total_grains << endl;
	cout << "double_total_grains " << double_total_grains << endl;
}
