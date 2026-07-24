#include <future>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using namespace std::literals;

class Bank {
 public:
  void Debit(const std::string& name, int amount) { std::cout << "Debiting " << amount << " from " << name << '\n'; }
  void Credit(const std::string& name, int amount) { std::cout << "Crediting " << amount << " to " << name << '\n'; }
  void Print(const std::string& name) { std::cout << "Displaying credit for " << name << '\n'; }
};

template <typename T>
class Monitor {
  T data_;
  std::mutex mutex;

 public:
  Monitor<T>(T data = T{}) : data_(data) {}
  template <typename Func>
  auto operator()(Func func) {
    std::lock_guard<std::mutex> lock(mutex);
    return func(data_);
  }
};

int main() {
  Monitor<Bank> monitor{};
  monitor([](Bank bank) {
    bank.Credit("Paul", 1000);
    bank.Debit("Peter", 1000);
    bank.Print("Paul");
    bank.Print("Peter");
  });
  std::cout << "Done 1\n";

  Monitor<std::string> str_mon("start: "s);
  for (int i = 0; i < 5; i++) {
    str_mon([i](std::string& str) { str += std::to_string(i) + ", " + std::to_string(i) + ", "; });
    str_mon([](const std::string& str) { std::cout << str << '\n'; });
  }
  std::cout << "Done 2\n";

  Monitor<std::string> str_mon_2("start: "s);
  std::vector<std::future<void>> futs;
  for (int i = 0; i < 5; i++) {
    futs.push_back(std::async([&str_mon_2, i]() {
      str_mon_2([i](std::string& str) {
        std::this_thread::sleep_for(10ms);
        str += std::to_string(i) + ", " + std::to_string(i) + ", ";
      });
      str_mon_2([](const std::string& str) { std::cout << str << '\n'; });
    }));
  }
  for (auto& fut : futs) fut.wait();
  std::cout << "Done 3\n";
}