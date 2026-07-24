#include <condition_variable>
#include <iostream>
#include <string>
#include <thread>


using namespace std;
using namespace std::chrono;

// Global variables
mutex mut;
condition_variable cv;
bool modified{false};
string sdata{"Empty"};

// Waiting thread
void reader() {
  unique_lock<std::mutex> guard(mut);   // Acquire lock
  cv.wait(guard, [](){return modified;});                       // Wait for condition variable to be notified
  cout << "Data is " << sdata << " thread_id:" << std::this_thread::get_id() << endl;  // Wake up and use the new value
}

// Modyifing thread
void writer() {
  cout << "Writing data..." << endl;
  {
    lock_guard<std::mutex> lg(mut);   // Acquire lock
    std::this_thread::sleep_for(2s);  // Pretend to be busy...
    sdata = "Populated";              // Modify the data
		modified = true;
  }
  cv.notify_all();  // Notify the condition variable
}

int main() {
  cout << "Data is " << sdata << endl;
  // Even if the writer thread finishes before the reader thread starts, the notification should not be lost.
  thread write{writer};
  std::this_thread::sleep_for(500ms);
  thread read1{reader};
	std::this_thread::sleep_for(10ms);
	thread read2{reader};
	std::this_thread::sleep_for(10ms);
	thread read3{reader};
  write.join();
  read1.join();
	read2.join();
	read3.join();
}