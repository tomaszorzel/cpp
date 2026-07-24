// Thread pool with work stealing
#include <iostream>
#include <mutex>
#include <chrono>

#include "thread_pool.h"

using namespace std::literals;

// Protect shared state in random number engine
std::mutex rand_mut;

// Constructor
thread_pool::thread_pool() {
	thread_count = std::thread::hardware_concurrency() - 1;
	std::cout << "Creating a thread pool with " << thread_count << " threads\n";
	
	// Create a dynamic array of queues
	work_queues = std::make_unique<Queue []> (thread_count);
	
	// Start the threads
	for (int i = 0; i < thread_count; ++i) {
		threads.push_back(std::thread{&thread_pool::worker, this, i});
	}
}

// Destructor
thread_pool::~thread_pool() {
	// Wait for the threads to finish
	for (auto& thr: threads) {
		thr.join();
	}
}

// Returns a random number between 0 and thread_count
int thread_pool::get_random()
{
	std::lock_guard<std::mutex> lck_guard(rand_mut);
	std::uniform_int_distribution<int> dist(0, thread_count-1);
	return dist(mt);
}
	
// Entry point function for the threads
void thread_pool::worker(int idx) {
	while (true) {
		// Number of queues we have checked so far
		int visited = 0;
		Func task;

		// Take a task function off our queue
		int i = idx;
		
		while (!work_queues[i].try_pop(task)) {
			// Nothing on this queue
			// Pick another queue at random
			i = get_random();
			
			// Hot loop avoidance
			// If we have checked "enough" queues, pause for a while
			// then start again with our own queue
			if (++visited == thread_count) {
				std::this_thread::sleep_for(10ms);
				visited = 0;
				i = idx;
			}
		}

		// Invoke the task function
		task();
	}
}

// Choose a thread's queue and add a task to it
void thread_pool::submit(Func func) {
	int i;
	
	// Pick a queue at random
	do {
		i = get_random();
	}
	// Until we find one that is not full
	while (!work_queues[i].try_push(func));
}
