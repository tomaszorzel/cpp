#include <iostream>
#include <vector>

void f() {
  std::cout << "call f()" << std::endl;
}

void f(int i) {
  std::cout << "call f(int)" << std::endl;
}

void f(int i1, int i2) {
  std::cout << "call f(int, int)" << std::endl;
}

void f(double d1, double d2=3.14) {
  std::cout << "call f(double, double)" << std::endl;
}

int calc(int&, int&);
int calc(const int&, const int&);
int calc(char *cp1, char *cp2) {
  std::cout << "plain" << std::endl;
  return 0;
}
int calc(const char *cp1, const char *cp2) {
  std::cout << "const" << std::endl;
  return 0;
}

//int calc(char * const cp1, char * const cp2) {
//  std::cout << "const2" << std::endl;
//  return 0;
//}

int fun(int a, int b);
typedef int (*pfun)(int, int);

int add(int a, int b) {
  return a+b;
}

int subtract(int a, int b) {
  return a-b;
}

int multiply(int a, int b) {
  return a*b;
}

int devide(int a, int b) {
  if (b!=0)
    return a/b;
  else {
    std::cout << "error, b cannot be 0!" << std::endl;
    return 1;
  }
}

int main() {
//  f(2.56, 42.0);
//  f(42);
//  f(42, 0);
//  f(2.56, 3.14);
//  char *hello = "hello world";
//  char * const world = hello;
//  calc(world, world);

  std::vector<pfun> vpf;
  vpf.push_back(add);
  vpf.push_back(subtract);
  vpf.push_back(multiply);
  vpf.push_back(devide);
  std::cout << vpf.size() << std::endl;

  for (std::vector<pfun>::iterator it=vpf.begin(); it!=vpf.end(); ++it) {
    std::cout << (*it)(10, 5) << std::endl;
  }

  return 0;
}
