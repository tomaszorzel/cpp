#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std::literals;

class Semaphore {
 public:
  void Release() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "Adding one item\n";
    counter_++;
    Count();
    cond_var_.notify_all();
  }
  void Acquire() {
    std::unique_lock<std::mutex> lock(mutex_);
    std::cout << "Removing one item\n";
    while (counter_ == 0) {
      cond_var_.wait(lock);
    }
    counter_--;
    Count();
  }

 private:
  void Count() const { std::cout << "Number of items: " << counter_ << '\n'; }

  std::mutex mutex_;
  int counter_{0};
  std::condition_variable cond_var_;
};

int main() {
  Semaphore semaphore;
  auto release = [&semaphore]() {
    semaphore.Release();
    std::this_thread::sleep_for(50ms);
  };
  auto acquire = [&semaphore]() {
    semaphore.Acquire();
    std::this_thread::sleep_for(50ms);
  };

  std::vector<std::thread> threads;
  for (int i = 0; i < 2; i++) {
    threads.push_back(std::thread(release));
  }
  for (int i = 0; i < 4; i++) {
    threads.push_back(std::thread(acquire));
  }
  for (int i = 0; i < 3; i++) {
    threads.push_back(std::thread(release));
  }
  for (auto& t : threads) {
    t.join();
  }
}