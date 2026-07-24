#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class SomeType {
 public:
  void Member() { std::cout << "Member called by thread_id:" << std::this_thread::get_id() << '\n'; }
};

std::atomic<SomeType*> ptr = nullptr;
std::mutex mtx;

void Task() {
  if (!ptr) {
    std::lock_guard<std::mutex> lck(mtx);
    if (!ptr) {
      ptr = new SomeType();
      std::cout << "Instance created\n";
    }
  }
  SomeType* copy = ptr;
  copy->Member();
}

int main() {
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; i++) {
    threads.push_back(std::thread(Task));
  }
  for (auto& t : threads) {
    t.join();
  }
  return 0;
}