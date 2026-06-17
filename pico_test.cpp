#include <iostream>
#include <map>
#include <set>
#include <vector>

void calculate_sums(const std::vector<int>& input, std::map<int, int>& sums) {
  // assume size is up to 3;
  if (input.size() > 1) {
    int sum1 = input.at(0) + input.at(1);
    if (sums.contains(sum1)) {
      sums.at(sum1)++;
    } else {
      sums.emplace(sum1, 1);
    }
    if (input.size() > 2) {  //  so 3
      int sum2 = input.at(1) + input.at(2);
      if (sum2 != sum1) {
        if (sums.contains(sum2)) {
          sums.at(sum2)++;
        } else {
          sums.emplace(sum2, 1);
        }
      }
    }
  }
}

std::vector<int>::const_iterator get_end(const std::vector<int>& A, int max_size) {
  auto end = A.begin();
  int size = 0;
  while (end != A.end()) {
    end++;
    size++;
    if (size == max_size) {
      break;
    }
  }
  return end;
}

std::vector<int> get_temp(const std::vector<int>& A) {
  auto begin = A.begin();
  int max_size = 3;
  auto end = get_end(A, max_size);
  return std::vector<int>(begin, end);
}

int solution(std::vector<int>& A) {  // here is the method meant to be implemented in the task.
  if (A.size() == 2 || A.size() == 3) {
    return 1;
  }

  std::vector<int> temp = get_temp(A);
  std::map<int, int> number_of_sums{};
  while (temp.size() > 0) {
    calculate_sums(temp, number_of_sums);
    int max_size_to_erase = 2;
    auto end = get_end(A, max_size_to_erase);
    A.erase(A.begin(), end);
    temp = get_temp(A);
  }

  std::set<int> occurences;
  for (auto& [sum, count] : number_of_sums) {
    occurences.emplace(count);
  }
  return *occurences.rbegin();
}

int main(int argc, char** argv) {
  std::vector<int> test_data_1{10, 1, 3, 1, 2, 2, 1, 0, 4};
  // std::vector<int> test_data_1{10,1, 3, 3};
  std::cout << solution(test_data_1) << "\n";

  std::vector<int> test_data_2{9, 9, 9, 9, 9};
  std::cout << solution(test_data_2) << "\n";

  std::vector<int> test_data_3{9, 9, 8, 8, 8, 8, 9, 9, 9, 8, 8, 9};
  std::cout << solution(test_data_3) << "\n";
  return 0;
}