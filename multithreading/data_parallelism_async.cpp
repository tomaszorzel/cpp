
#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

static std::mt19937 mt;
std::uniform_real_distribution<double> dist(0, 100);

double Sum(std::vector<double>::iterator beg, std::vector<double>::iterator end) {
  return std::accumulate(beg, end, 0.0);
}

double AddParallel(std::vector<double>& vec) {
  auto vsize = vec.size();
  std::future<double> fut1 = std::async(std::launch::async, Sum, vec.begin(), vec.begin() + vsize / 4);
  std::future<double> fut2 = std::async(std::launch::async, Sum, vec.begin() + vsize / 4, vec.begin() + 2 * vsize / 4);
  std::future<double> fut3 =
      std::async(std::launch::async, Sum, vec.begin() + 2 * vsize / 4, vec.begin() + 3 * vsize / 4);
  std::future<double> fut4 = std::async(std::launch::async, Sum, vec.begin() + 3 * vsize / 4, vec.end());
  return fut1.get() + fut2.get() + fut3.get() + fut4.get();
}

int main() {
  std::vector<double> vec(16);
  std::iota(vec.begin(), vec.end(), 1.0);

  std::vector<double> vrand(10000);
  std::ranges::generate(vrand, []() { return dist(mt); });

  std::cout << "Sum of first 16 integers: " << AddParallel(vec) << '\n';
  std::cout << "Sum of first 10000 random numbers: " << AddParallel(vrand) << '\n';
}