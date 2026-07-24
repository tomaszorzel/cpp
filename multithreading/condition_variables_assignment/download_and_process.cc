#include <condition_variable>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

using namespace std::literals;

std::string shared_data;
bool update_progress{false};
bool completed{false};

std::mutex data_mutex;
std::mutex completed_mutex;

std::condition_variable cv_data;
std::condition_variable cv_completion;

void Download() {
  for (int i = 0; i < 5; i++) {
    std::cout << "Downloader thread waiting for data..\n";
    std::this_thread::sleep_for(2s);
    // lock data mutex
    std::unique_lock<std::mutex> lock_data(data_mutex);
    shared_data += "Block" + std::to_string(i);
    std::cout << "Downloaded shared data:" << shared_data << '\n';
    update_progress = true;
    lock_data.unlock();
    cv_data.notify_all();
  }
  // notify when downloading completed
  std::cout << "Downloading done.\n";
  std::lock_guard<std::mutex> compl_lock(completed_mutex);
  completed = true;
  cv_completion.notify_all();
}

void ProgressBar() {
  std::size_t len = 0;
  while (true) {
    std::cout << "ProgressBar waiting for data..\n";
    std::unique_lock<std::mutex> lock_data(data_mutex);
    cv_data.wait(lock_data, []() { return update_progress; });
    len = shared_data.size();
    update_progress = false;
    lock_data.unlock();
    std::cout << "Downloaded " << len << " bytes";
    
    std::unique_lock<std::mutex> lock_completed(completed_mutex);
    if (cv_completion.wait_for(lock_completed, 10ms, []() { return completed; })) {
      std::cout << "Reporting progress done\n";
      break;
    }
  }
}

void Process() {
  std::this_thread::sleep_for(200ms);
  std::cout << "Data processor waiting for data..\n";
  std::unique_lock<std::mutex> completed_lock(completed_mutex);
  cv_completion.wait(completed_lock, []() { return completed; });
  completed_lock.unlock();

  std::unique_lock<std::mutex> data_lock(data_mutex);
  std::cout << "Processing data..\n";
  data_lock.unlock();
}

int main() {
  std::cout << "Starting program, data:" << shared_data << '\n';
  std::thread downloader(Download);
  std::thread progress(ProgressBar);
  std::thread process(Process);

  downloader.join();
  progress.join();
  process.join();
  return 0;
}