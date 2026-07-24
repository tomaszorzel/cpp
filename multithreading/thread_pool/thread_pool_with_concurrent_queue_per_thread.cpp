#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <ratio>
#include <thread>

using namespace std::literals;

template <typename Type>
class ConcurrentQueue {
  std::timed_mutex mutex_;
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

  // void Push(Type elem) {
  //   using namespace std::literals;
  //   std::unique_lock<std::mutex> lock(mutex_);
  //   cv.wait(lock, [this]() { return queue_.size() < max_; });
  //   queue_.push(elem);
  //   cv.notify_one();
  // }

  // void Pop(Type& elem) {
  //   std::unique_lock<std::mutex> lock(mutex_);
  //   cv.wait(lock, [this]() { return not queue_.empty(); });
  //   elem = queue_.front();
  //   queue_.pop();
  //   cv.notify_one();
  // }

  bool Empty() { return queue_.empty(); }

  bool TryPush(Type value) {
    std::unique_lock<std::timed_mutex> lock(mutex_, std::defer_lock);
    if (!lock.try_lock_for(1ms) || queue_.size() > max_) {
      return false;
    }
    queue_.push(value);
    return true;
  }

  bool TryPop(Type& value) {
    std::unique_lock<std::timed_mutex> lock(mutex_, std::defer_lock);
    if (!lock.try_lock_for(1ms) || queue_.empty()) {
      return false;
    }
    value = queue_.front();
    queue_.pop();
    return true;
  }
};

using Func = std::function<void()>;
using Queue = ConcurrentQueue<Func>;
std::mutex rand_mutex;

class ThreadPool {
  std::mt19937 mt;
  std::unique_ptr<Queue[]> work_queues_;
  std::vector<std::thread> threads_;
  std::mutex pos_mutex;
  // int pos{0};
  int thread_count_;
  // bool done{false};
  
  int GetRandom(){
    std:: lock_guard<std::mutex> lock(rand_mutex);
    std::uniform_int_distribution<int> dist(0, thread_count_-1);
    return dist(mt);
  }

  void Worker(int idx) {
    while(true){
      int visited = 0;
      Func task;
      int i = idx;
      while (!work_queues_[i].TryPop(task)) {
        i = GetRandom();
  
        if (++visited == thread_count_) {
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
          visited = 0;
          i = idx;
        }
      }
      task();
    }
    // while (not done) {
    //   Func task;
    //   work_queues_[idx].Pop(task);
    //   task();
      // int queue_count = thread_count_;
      // for (int i = 0; i < queue_count; i++) {
      //   done = true;
      //   done &= work_queues_[i].Empty();
      // }
    // }
  }

 public:
  ThreadPool() : thread_count_(std::thread::hardware_concurrency() - 2) {
    std::cout << "Creating a thread pool with " << thread_count_ << " threads\n";
    work_queues_ = std::make_unique<Queue[]>(thread_count_);
    for (int i = 0; i < thread_count_; i++) {
      threads_.push_back(std::thread{&ThreadPool::Worker, this, i});
    }
  }
  ~ThreadPool() {
    for (auto& thread : threads_) {
      thread.join();
    }
  }
  void Submit(Func func) {
    int i;
    do {
      i = GetRandom();
    } while (!work_queues_[i].TryPush(func));

    // std::lock_guard<std::mutex> lock(pos_mutex);
    // work_queues_[pos].Push(func);
    // int queue_count = thread_count_;
    // pos = (pos + 1) % queue_count;
  }
};

void task() {
  using namespace std::literals;
  std::cout << "Thread id: " << std::this_thread::get_id() << " starting a task\n";
  std::this_thread::sleep_for(100ms);
  std::cout << "Thread id: " << std::this_thread::get_id() << " finishing a task\n";
}

void task2() {
  std::cout << "Thread id: " << std::this_thread::get_id() << " starting a task\n";
  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cout << "Thread id: " << std::this_thread::get_id() << " starting a task\n";
}

int main() {
  ThreadPool pool;
  pool.Submit(task2);
  for (int i = 0; i < 200; i++) {
    pool.Submit(task);
  }
  pool.Submit([]() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "All task completed\n";
  });
}