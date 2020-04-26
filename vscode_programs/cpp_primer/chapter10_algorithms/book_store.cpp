#include "book_store.h"

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]) {
	ifstream records(argv[1]);
	if (records) {
		ofstream out(argv[2], ofstream::app);
		if (!out) {
			cout << "Error: Cannot open file to write" << endl;
			return 1;
		}
	    Sales_data total(records);
	    Sales_data trans;
	    while (read(records, trans)) {
    	    if (total.isbn() == trans.isbn()) {
        	    total.combine(trans);
        	} else {
            	print(out, total);
            	total = trans;
        	}
    	}	
    	print(out, total);
	} else {
		cout << "Error: Cannot open file to read" << endl;
	}
}
