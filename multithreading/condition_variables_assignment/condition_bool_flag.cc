#include <condition_variable>
#include <iostream>
#include <string>
#include <thread>

using namespace std;
using namespace std::chrono;

// Global variables
mutex mut;
bool writing_done{false};
string sdata{"Empty"};

// Waiting thread
void reader() {
  unique_lock<std::mutex> guard(mut);  // Acquire lock to check the bool flag
  while (!writing_done) {
    guard.unlock();  // Unlock to not to block writer thread
    std::this_thread::sleep_for(10ms);
    guard.lock();  // Lock to check the flag
  }
  cout << "Data is " << sdata << endl;  // Wake up and use the new value
  guard.unlock();                       // Need to unlock the mutex
}

// Modyifing thread
void writer() {
  cout << "Writing data..." << endl;
  lock_guard<std::mutex> lg(mut);  // Acquire lock
  while (!writing_done) {
    std::this_thread::sleep_for(2s);  // Pretend to be busy...
    sdata = "Populated";              // Modify the data
    writing_done = true;              // Update the bool flag
  }
}

int main() {
  cout << "Data is " << sdata << endl;
  thread read{reader};
  thread write{writer};
  /*
  // If the writer thread finishes before the reader thread starts, the notification is lost
  thread write{writer};
  std::this_thread::sleep_for(500ms);
  thread read{reader};
  */
  write.join();
  read.join();
}