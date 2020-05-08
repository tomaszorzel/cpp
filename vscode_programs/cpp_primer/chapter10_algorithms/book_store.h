#ifndef BOOK_STORE_H
#define BOOK_STORE_H

#include <fstream>
#include <iostream>

class Sales_data {
private:
  std::string bookNo;
  unsigned unitsSold = 0;
  double revenue = 0.0;
public:
  Sales_data(std::string bn, unsigned us, double r) : bookNo(bn), unitsSold(us), revenue(r) {}
  Sales_data() : Sales_data("999-83-999999-9-9", 0, 0) {}
  Sales_data(std::string s) : Sales_data(s, 0, 0) {}
  Sales_data(std::istream &is) {
    read(is, *this);
  }
  Sales_data(unsigned howMany, double howMuch) : Sales_data("999-83-999999-9-9", howMany, howMuch) {}
  std::string isbn() const { return bookNo; }
  Sales_data &combine(const Sales_data &sd) {
    unitsSold += sd.unitsSold;
    revenue += sd.revenue;
    return *this;
  }
  inline double avg_price() const {
  	return unitsSold ? revenue/unitsSold : 0.0;
  }
  friend std::istream &read(std::istream &, Sales_data &);
  friend std::ostream &print(std::ostream &, const Sales_data &);
  friend std::ostream &operator<<(std::ostream &os, const Sales_data&);
};

std::istream &read(std::istream &is, Sales_data &sd) {
  is >> sd.bookNo >> sd.unitsSold >> sd.revenue;
  return is;
}

std::ostream &print(std::ostream &os, const Sales_data &sd) {
  os << sd.bookNo << " " << sd.unitsSold << " " << sd.revenue << std::endl;
  return os;
}

std::ostream &operator<<(std::ostream &os, const Sales_data & sd) {
  os << sd.bookNo << " " << sd.unitsSold << " " << sd.revenue << std::endl;
  return os;
}

#endif // BOOK_STORE_H