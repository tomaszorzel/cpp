#include <execution>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
  std::vector<double> expected{0.1, 0.2, 0.3, 0.4, 0.5};
  std::vector<double> actual{0.09, 0.22, 0.27, 0.41, 0.52};

  auto max_diff = std::transform_reduce(
      std::execution::par, expected.begin(), expected.end(), actual.begin(), 0.0,
      [](double exp, double act) { return std::max(exp, act); },
      [](double exp, double act) { return std::abs(act - exp); });
  std::cout << "Max difference is: " << max_diff << '\n';

  return 0;
}