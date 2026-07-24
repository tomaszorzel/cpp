#include <future>
#include <iostream>
#include <thread>

using namespace std::literals;

void sum(int a, int b, std::promise<int>& sum_promise) {
  std::this_thread::sleep_for(2s);
  sum_promise.set_value(a + b);
}

void print_res(std::promise<int>& sum_promise) {
  std::cout << "Waiting for result\n";
  std::cout << "6 + 7 is " << sum_promise.get_future().get() << '\n';
}

int main() {
  std::promise<int> promise;
  int x = 6;
  int y = 7;
  std::thread sum_thread(sum, x, y, std::ref(promise));
  std::thread print_thread(print_res, std::ref(promise));
  sum_thread.join();
  print_thread.join();
  return 0;
}