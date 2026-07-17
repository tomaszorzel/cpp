#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

bool IsDevisibleBy3(int test) {
  std::vector<int> digits;
  while (test > 0) {
    int digit = test % 10;
    digits.push_back(digit);
    test /= 10;
  }
  int sum_of_digits = std::accumulate(digits.begin(), digits.end(), 0);
  return sum_of_digits % 3 == 0;
}

bool IsDevisibleBy5(int test) {
  int last_digit = test % 10;
  return (last_digit == 5 || last_digit == 0);
}

void FizzBuzz() {
  int i = 1;
  while (i < 20) {
    if (IsDevisibleBy3(i)) {
      if (IsDevisibleBy5(i)) {
        std::cout << "fizzbuzz\n";
      } else {
        std::cout << "fizz\n";
      }
    } else if (IsDevisibleBy5(i)) {
      std::cout << "buzz\n";
    } else {
      std::cout << i << '\n';
    }
    i++;
  }
}

int main() {
  std::thread fizzbuzz_thread(FizzBuzz);
  fizzbuzz_thread.join();
  return 0;
}