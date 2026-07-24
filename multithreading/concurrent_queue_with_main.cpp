#include <condition_variable>
#include <cstdint>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>

class QueueEmpty : public std::runtime_error {
 public:
  QueueEmpty(const std::string& what = "Error, queue is empty!") : std::runtime_error(what) {}
};

template <typename Type>
class ConcurrentQueue {
  std::mutex mutex_;
  std::queue<Type> queue_{};
  int max_;
  std::condition_variable cv;

 public:
  explicit ConcurrentQueue<Type>(int max = 50) : max_(max){};
  ConcurrentQueue<Type>(ConcurrentQueue<Type>&) = delete;
  ConcurrentQueue<Type>& operator=(ConcurrentQueue<Type>&) = delete;
  ConcurrentQueue<Type>(ConcurrentQueue<Type>&&) = delete;
  ConcurrentQueue<Type>& operator=(ConcurrentQueue<Type>&&) = delete;

  void Push(Type elem) {
    using namespace std::literals;
    std::unique_lock<std::mutex> lock(mutex_);
    // while (queue_.size() > max_) {
    //   lock.unlock();
    //   std::this_thread::sleep_for(50ms);
    //   lock.lock();
    // }
    cv.wait(lock, [this]() { return queue_.size() < max_; });
    queue_.push(elem);
    cv.notify_one();
  }

  void Pop(Type& elem) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv.wait(lock, [this]() { return not queue_.empty(); });
    elem = queue_.front();
    queue_.pop();
    cv.notify_one();
  }
};

ConcurrentQueue<std::string> queue;

void Writer() {
  using namespace std::literals;
  std::this_thread::sleep_for(2s);  // pretend to be busy
  std::cout << "Writer calling push...\n";
  for (int i = 0; i < 60; i++) {
    std::string sdata = "Item " + std::to_string(i);
    queue.Push(sdata);
  }
  std::cout << "Writer returned from push...\n";
}

void Reader() {
  std::string sdata;
  std::cout << "Reader calling pop...\n";
  for (int i = 0; i < 60; i++) {
    queue.Pop(sdata);
    std::cout << "Reader received data: " << sdata << '\n';
  }
}

int main() {
  auto writer_fut = std::async(std::launch::async, Writer);
  auto reader_fut = std::async(std::launch::async, Reader);
  writer_fut.wait();
  reader_fut.wait();
}