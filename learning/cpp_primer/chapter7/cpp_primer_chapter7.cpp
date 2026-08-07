//#include <iostream>
//using namespace std;
//
//class Sales_data {
//public:
//  string bookNo;
//  unsigned unitsSold = 0;
//  double revenue = 0.0;
//  Sales_data &combine(Sales_data &sd) {
//      unitsSold += sd.unitsSold;
//      revenue += sd.revenue;
//      return *this;
//  }
//  string getBookNo() {
//    return bookNo;
//  }
////  Sales_data &operator=(Sales_data &sd) {
////    bookNo = sd.bookNo;
////    unitsSold = sd.unitsSold;
////    revenue = sd.revenue;
////    return *this;
////  }
////  friend void print(Sales_data &sd);
////  friend bool read(Sales_data &sd);
//};
//
//Sales_data add(Sales_data &sd1, Sales_data &sd2) {
//  Sales_data sum = sd1;
//  sum.combine(sd2);
//  return sum;
//}
//
//ostream &print(ostream &os, const Sales_data &sd) {
//  os << "ISBN\t\t\tUnits sold\t\tRevenue\n";
//  os << sd.bookNo << "\t\t\t" << sd.unitsSold << "\t\t\t" << sd.revenue;
//  return os;
//}
//
//istream &read(istream &is, Sales_data &sd) {
//  is >> sd.bookNo >> sd.unitsSold >> sd.revenue;
//  return is;
//}
//
////void print(Sales_data &sd) {
////  cout << "ISBN\t\t\tUnits sold\t\tRevenue\n";
////  cout << sd.bookNo << "\t\t\t" << sd.unitsSold << "\t\t\t" << sd.revenue << endl;
////}
//
////bool read(Sales_data &sd) {
////  if (cin >> sd.bookNo >> sd.unitsSold >> sd.revenue)
////    return true;
////  else
////    return false;
////}
//
//int main()
//{
//  Sales_data data1, data2;
//  if (read(cin, data1)) {
//    while (read(cin, data2)) {
//      if(data1.getBookNo() == data2.getBookNo()) {
//        data1 = add(data1, data2);
//      } else {
//        print(cout, data1);
//        cout << endl;
//        data1 = data2;
//      }
//    }
//    print(cout, data1);
//    cout << endl;
//  } else {
//    cerr << "No data?!" << endl;
//  }
//}



#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Y;
class X {
  Y *y;
};

class Y {
  X x;
};

class Person;

istream &read(istream&, Person&);

class Person {
private:
  string fullName;
  string address;
public:
  Person() = default;
  Person(string name, string addr) : fullName(name), address(addr) {}
  Person(istream & is) { read(is, *this); }

  string getName() const {
    return fullName;
  }
  string getAddress() const {
    return address;
  }
  friend ostream& print(ostream &, const Person &);
  friend istream &read(istream &, Person &);
};

ostream &print(ostream &os, const Person &person) {
  os << person.getName() << ", " << person.getAddress() << endl;
  return os;
}

istream &read(istream &is, Person &person) {
  is >> person.fullName >> person.address;
  return is;
}


class Sales_data {
public:
//  Sales_data() {};
  Sales_data(unsigned howMany, double howMuch) : unitsSold(howMany), revenue(howMuch) {}
  Sales_data(istream &is=cin) {
    read(is, *this);
  }
  string isbn() { return bookNo; }
  Sales_data &combine(Sales_data & sd) {
    unitsSold += sd.unitsSold;
    revenue += sd.revenue;
    return *this;
  }
  inline double avg_price() const {
  	return unitsSold ? revenue/unitsSold : 0.0;
  }
  friend istream &read(istream &, Sales_data &);
  friend ostream &print(ostream &, const Sales_data &);

private:
  string bookNo;
  unsigned unitsSold = 0;
  double revenue = 0.0;
};

istream &read(istream &is, Sales_data &sd) {
  is >> sd.bookNo >> sd.unitsSold >> sd.revenue;
  return is;
}

ostream &print(ostream &os, const Sales_data &sd) {
  os << sd.bookNo << " " << sd.unitsSold << " " << sd.revenue << endl;
  return os;
}

/// class Screen & WindowMgr
class WindowMgr;

class Screen {
  friend class WindowMgr;

public:
  using pos = string::size_type;
	Screen() = default;
	Screen(pos h, pos w) : height(h), width(w), contents(h*w, ' ') {}
	Screen(pos h, pos w, char c) : height(h), width(w), contents(h*w, c) {}

	inline char get();
	inline char get(pos, pos) const;
	inline Screen &move(pos, pos);
	Screen &set(char);
	Screen &set(pos, pos, char);
	inline Screen &display(ostream&);
	inline const Screen &display(ostream&) const;
  inline pos size() const;
private:
	pos cursor = 0;
	pos height = 0, width = 0;
	string contents;
  inline void doDisplay(ostream &os) const;
};

inline void Screen::doDisplay(ostream &os) const {
  os << contents;
}

inline Screen& Screen::move(pos r, pos c) {
	pos row = r * width;
	cursor = row + c;
	return *this;
}

inline Screen& Screen::display(ostream &os) {
  doDisplay(os);
  return *this;
}

inline const Screen & Screen::display(ostream &os) const {
  doDisplay(os);
  return *this;
}

inline char Screen::get(pos r, pos c) const {
  pos row = r * width;
	return contents[row+c];
}

inline char Screen::get() {
	return contents[cursor];
}

inline Screen &Screen::set(char c) {
  contents[cursor] = c;
  return *this;
}

inline Screen &Screen::set(pos r, pos c, char ch) {
  contents[r*width + c] = ch;
  return *this;
}

Screen::pos Screen::size() const {
  return height * width;
}

class WindowMgr {
public:
  using ScreenIndex = vector<Screen>::size_type;
  void clear(ScreenIndex);
  ScreenIndex addScreen(const Screen &);
private:
  vector<Screen> screens{Screen(24, 80, ' ')};
};

void WindowMgr::clear(ScreenIndex si) {
  Screen &s = screens[si];
  s.contents = string(s.height * s.width, ' ');
}

WindowMgr::ScreenIndex WindowMgr::addScreen(const Screen &s) {
  screens.push_back(s);
  return screens.size() - 1;
}

typedef string Type;
Type initVal();
class Exercise {
public:
  typedef double Type;
  Type setVal(Type);
  Type initVal();
private:
  int val;
};
Exercise::Type Exercise::setVal(Exercise::Type parm) {
  val = parm + initVal();
  return val;
}
Exercise::Type Exercise::initVal() {
  return 10.0;
}
Type initVal() {
  return "globalInit";
}

/// class Date
class Date {
private:
  enum month { JANUARY, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER };

  unsigned int day=1;
  month _month=(Date::month)1;
  short year=1999;
public:
  Date() = default;
  Date(unsigned int d, unsigned int m, short y) : day(d), _month((Date::month)m), year(y) {}
  void printDate(ostream &os=cout) {
    os << day << "-" << _month << "-" << year << endl;
  }
};

int main () {
//  Exercise e;
//  cout << e.initVal() << endl;

//  Screen myScreen(5, 5, 'X');
//  myScreen.move(4, 0).set('#').display(cout);
//  cout << "\n";
//  myScreen.display(cout);
//  cout << "\n";

//  Sales_data total(cin);
//  Sales_data trans;
//  while (read(cin, trans)) {
//    if (total.isbn() == trans.isbn()) {
//      total.combine(trans);
//    } else {
//      print(cout, total);
//      total = trans;
//    }
//  }
//  print(cout, total);

  Date d1;
  Date d2(2,2,2000);
  d1.printDate();
  d2.printDate();
}
