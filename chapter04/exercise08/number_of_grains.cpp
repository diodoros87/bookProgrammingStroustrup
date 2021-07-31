#include <iostream>

using namespace std;
	
int main() {
	long int field_grains = 1;
	long int total_grains = field_grains;
	short int field_to_1000 = 1;		
	short int field_to_million = 1;
	short int field_to_1000000000 = 1;					  
	
	for(; total_grains <= 1000000000; field_to_1000000000++) {
		if (total_grains <= 1000)
			field_to_1000++;
		if (total_grains <= 1000000)
			field_to_million++;
			
		cout << "field           " << field_to_1000000000 << endl;
		cout << "grains on field " << field_grains << endl;
		cout << "total grains    " << total_grains << endl << endl << endl;
		
		field_grains *= 2;	
		total_grains += field_grains;
	}
	
	cout << "\n\nSUMMARY" << endl;
	cout << "field to 1000       " << field_to_1000 << endl;
	cout << "field to million    " << field_to_million << endl;
	cout << "field to 1000000000 " << field_to_1000000000 << endl;
	cout << "grains on field     " << field_grains << endl;
	cout << "total grains        " << total_grains << endl;
}
