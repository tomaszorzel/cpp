#include <random>
#include <iostream>

std::mt19937_64 mt;

int main() {
  std::uniform_int_distribution<int> ui(0, 10);
  for (int i = 0; i < 10; i++) {
    std::cout << ui(mt) << '\n';
  }
  std::uniform_real_distribution<float> uf(0, 100);
  for (int i = 0; i < 5; i++) {
    std::cout << uf(mt) << '\n';
  }

  return 0;
}