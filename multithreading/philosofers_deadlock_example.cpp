#include <unistd.h>

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std::literals;

constexpr int nforks = 5;
constexpr int nphilosophers = nforks;
const std::array<const char*, nphilosophers> names = {"A", "B", "C", "D", "E"};
std::array<int, nphilosophers> mouthfuls{0};
constexpr auto think_time = 2s;
constexpr auto eat_time = 1s;
std::array<std::mutex, nforks> fork_mutex;
std::mutex print_mutex;

void Print(int n, const std::string& str, int fork_no) {
  std::lock_guard<std::mutex> print_lock(print_mutex);
  std::cout << "Philosopher " << names.at(n) << str << fork_no << '\n';
}

void Print(int n, const std::string& str) {
  std::lock_guard<std::mutex> print_lock(print_mutex);
  std::cout << "Philosopher " << names.at(n) << str << '\n';
}

void Dine(int phil_no) {
  int left_fork = phil_no;
  int right_fork = (phil_no + 1) % nforks;

  Print(phil_no, "\'s left fork is number ", left_fork);
  Print(phil_no, "\'s right fork is number ", right_fork);
  Print(phil_no, " is thinking...");

  std::this_thread::sleep_for(think_time);

  Print(phil_no, " reaches for fork number ", left_fork);
  Print(phil_no, " reaches for fork number ", right_fork);

  std::lock(fork_mutex.at(left_fork), fork_mutex.at(right_fork));
  // Potentially std::scoped_lock could be used as well but it seems that g++ doesn't support it's usag with more than
  // one mutex:
  // std::scoped_lock<std::mutex> scoped_lck{fork_mutex.at(left_fork), fork_mutex.at(right_fork)};

  // For excercise let's acquire the ownership of the locked mutexes with unique_lock
  std::unique_lock<std::mutex> lock_1{fork_mutex.at(left_fork), std::adopt_lock};
  std::unique_lock<std::mutex> lock_2{fork_mutex.at(right_fork), std::adopt_lock};

  Print(phil_no, " picks up fork ", left_fork);
  Print(phil_no, " picks up fork ", right_fork);

  Print(phil_no, " is eating...");

  std::this_thread::sleep_for(eat_time);

  mouthfuls.at(phil_no)++;
  Print(phil_no, " puts down fork ", left_fork);
  Print(phil_no, " puts down fork ", right_fork);
  Print(phil_no, " is thinking...");

  // fork_mutex.at(left_fork).unlock();
  // fork_mutex.at(right_fork).unlock();
  
  // Unlock the unique_lock.
  lock_1.unlock();
  lock_2.unlock();
  std::this_thread::sleep_for(think_time);
}

int main() {
  std::vector<std::thread> philos;
  philos.resize(nphilosophers);
  for (int i = 0; i < nphilosophers; ++i) {
    philos.at(i) = (std::thread{Dine, i});
  }

  for (auto& philos : philos) {
    philos.join();
  }

  for (int i = 0; i < nphilosophers; ++i) {
    std::cout << "Philosopher " << names.at(i) << " had eaten " << mouthfuls.at(i) << " times\n";
  }

  return 0;
}