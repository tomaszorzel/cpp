#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <ratio>
#include <thread>

template <typename Type>
class ConcurrentQueue {
  std::mutex mutex_;
  std::queue<Type> queue_{};
  std::condition_variable cv;
  int max_{50};

 public:
  ConcurrentQueue<Type>() = default;
  explicit ConcurrentQueue<Type>(int max) : max_(max) {}
  ConcurrentQueue<Type>(ConcurrentQueue<Type>&) = delete;
  ConcurrentQueue<Type>& operator=(ConcurrentQueue<Type>&) = delete;
  ConcurrentQueue<Type>(ConcurrentQueue<Type>&&) = delete;
  ConcurrentQueue<Type>& operator=(ConcurrentQueue<Type>&&) = delete;

  void Push(Type elem) {
    using namespace std::literals;
    std::unique_lock<std::mutex> lock(mutex_);
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

  bool Empty() { return queue_.empty(); }
};

using Func = std::function<void()>;

class ThreadPool {
  ConcurrentQueue<Func> work_queue_;
  std::vector<std::thread> threads_;
  int thread_count_;
  bool done{false};
  void Worker() {
    while (not done) {
      Func task;
      work_queue_.Pop(task);
      task();
      done = work_queue_.Empty();
    }
  }

 public:
  ThreadPool() : thread_count_(std::thread::hardware_concurrency() - 2) {
    std::cout << "Creating a thread pool with " << thread_count_ << " threads\n";
    for (int i = 0; i < thread_count_; i++) {
      threads_.push_back(std::thread{&ThreadPool::Worker, this});
    }
  }
  ~ThreadPool() {
    for (auto& thread : threads_) {
      thread.join();
    }
  }
  void Submit(Func func) { work_queue_.Push(func); }
};

void task() {
  using namespace std::literals;
  std::cout << "Thread id: " << std::this_thread::get_id() << " starting a task\n";
  std::this_thread::sleep_for(100ms);
  std::cout << "Thread id: " << std::this_thread::get_id() << " finishing a task\n";
}

int main() {
  ThreadPool pool;
  for (int i = 0; i < 20; i++) {
    pool.Submit(task);
  }
  pool.Submit([]() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "All task completed\n";
  });
}