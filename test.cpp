#include <cstdio>
#include <iostream>
#include <string>
#include <unordered_map>

class Counter {
 public:
  Counter(int argc, char** argv) {
    std::string line;
    if (argc > 1) {
      for (int i = 1; i < argc; i++) {
        line = argv[i];
        Count(line);
      }
    } else {
      while (std::getline(std::cin, line)) {
        Count(line);
      }
    }
  }

  void Count(const std::string& line);
  void PrintRes();

 private:
  std::unordered_map<char, int> counters_{};
};

void Counter::Count(const std::string& line) {
  for (const char& character : line) {
    if (counters_.count(character) != 0) {
      counters_[character]++;
    } else {
      counters_.insert(std::make_pair(character, 1));
    }
  }
  PrintRes();
  counters_.clear();
}

void Counter::PrintRes() {
  for (auto [character, counter] : counters_) {
    std::cout << character << ": " << counter << std::endl;
  }
}

int main(int argc, char** argv) {
  try {
    Counter counter(argc, argv);
  } catch (...) {
    std::cout << "Failed" << std::endl;
  }
  return 0;
}