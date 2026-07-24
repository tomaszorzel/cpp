#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <vector>

int main() {
  std::vector<int> vec{1, 5, 6, 4, 3, 2, 10};
  std::sort(std::execution::par_unseq, vec.begin(), vec.end());
  for (const auto& i : vec) {
    std::cout << i << ' ';
  }
  std::cout << '\n';

  std::vector<int> vec2(20000000);
  int count = 0;
  auto start = std::chrono::high_resolution_clock::now();
  std::for_each(std::execution::par_unseq, vec2.begin(), vec2.end(), [&count](int& x) { x = ++count; });
  // std::for_each(vec2.begin(), vec2.end(), [&count](int& x) { x = ++count; }); // default = sequentially
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << duration.count() << '\n';
  // std::ranges::for_each(vec2, [](const int& x){std::cout << x << " ";});
  // std::cout << '\n';
  return 0;
}