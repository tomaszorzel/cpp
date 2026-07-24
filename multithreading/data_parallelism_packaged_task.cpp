#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

static std::mt19937 mt;
std::uniform_real_distribution<double> dist(0, 100);

double Sum(double* begin, double* end) { return std::accumulate(begin, end, 0.0); }

double AddParallel(std::vector<double>& vec) {
  std::packaged_task<double(double*, double*)> ptask1(Sum);
  std::packaged_task<double(double*, double*)> ptask2(Sum);
  std::packaged_task<double(double*, double*)> ptask3(Sum);
  std::packaged_task<double(double*, double*)> ptask4(Sum);

  // Get future objects
  auto fut1 = ptask1.get_future();
  auto fut2 = ptask2.get_future();
  auto fut3 = ptask3.get_future();
  auto fut4 = ptask4.get_future();

  // Start 4 threads
  auto* vec0 = &vec[0];
  auto vsize = vec.size();
  std::thread thr1(std::move(ptask1), vec0, vec0 + vsize / 4);
  std::thread thr2(std::move(ptask2), vec0 + vsize / 4, vec0 + 2 * vsize / 4);
  std::thread thr3(std::move(ptask3), vec0 + 2 * vsize / 4, vec0 + 3 * vsize / 4);
  std::thread thr4(std::move(ptask4), vec0 + 3 * vsize / 4, vec0 + vsize);

  // Join all threads
  thr1.join(); thr2.join(); thr3.join(), thr4.join();

  // return results from all threads.
  return fut1.get() + fut2.get() + fut3.get() + fut4.get();
}

int main() {
  std::vector<double> vec(16);
  std::iota(vec.begin(), vec.end(), 1.0);

  std::vector<double> vrand(10000);
  std::ranges::generate(vrand, []() { return dist(mt); });

  std::cout << "Sum of first 16 integers: " << AddParallel(vec) << '\n';
  std::cout << "Sum of first 10000 integers: " << AddParallel(vrand) << '\n';
}