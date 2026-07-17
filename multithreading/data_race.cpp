#include <iostream>
#include <thread>

int global_var = 1;

void Increment(int& var) {
  for (int i = 0; i < 100000; i++) {
    var++;
  }
}

int main() {
  std::thread thread_1(Increment, std::ref(global_var));
  std::thread thread_2(Increment, std::ref(global_var));
  std::thread thread_3(Increment, std::ref(global_var));

  thread_1.join();
  thread_2.join();
  thread_3.join();

  std::cout << global_var << '\n';

  return 0;
}