#include <iostream>

// generic lambda
auto multiply = [](auto a, auto b) {
  return a*b;
};

int main () {
  int x = 2;
  int y =4;
  std::cout << multiply(x, y) << "\n";
  double a = 1.5;
  double b = 2.5;
  std::cout << multiply(a, b) << "\n";
  return 0;
}