#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Sales_data {
private:
  string bookNo;
  unsigned unitsSold = 0;
  double revenue = 0.0;
public:
  Sales_data(string bn, unsigned us, double r) : bookNo(bn), unitsSold(us), revenue(r) {}
  Sales_data() : Sales_data("999-83-999999-9-9", 0, 0) {}
  Sales_data(string s) : Sales_data(s, 0, 0) {}
  Sales_data(istream &is) {
    read(is, *this);
  }
  Sales_data(unsigned howMany, double howMuch) : Sales_data("999-83-999999-9-9", howMany, howMuch) {}
  string isbn() { return bookNo; }
  Sales_data &combine(const Sales_data &sd) {
    unitsSold += sd.unitsSold;
    revenue += sd.revenue;
    return *this;
  }
  inline double avg_price() const {
  	return unitsSold ? revenue/unitsSold : 0.0;
  }
  friend istream &read(istream &, Sales_data &);
  friend ostream &print(ostream &, const Sales_data &);
};

istream &read(istream &is, Sales_data &sd) {
  is >> sd.bookNo >> sd.unitsSold >> sd.revenue;
  return is;
}

ostream &print(ostream &os, const Sales_data &sd) {
  os << sd.bookNo << " " << sd.unitsSold << " " << sd.revenue << endl;
  return os;
}

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
