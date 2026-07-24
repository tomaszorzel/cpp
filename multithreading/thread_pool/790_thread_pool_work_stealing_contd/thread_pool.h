// Thread pool with work stealing
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <queue>
#include <thread>
#include <functional>
#include <memory>
#include <random>

#include "concurrent_queue.h"

// Type aliases to simplify the code
// All the task functions will have this type
using Func = std::function<void()>;

// Alias for concurrent queue type
using Queue = concurrent_queue<Func>;

class thread_pool {
	// Random number engine
	std::mt19937 mt;
	
	// Each thread has its own queue of task functions
	std::unique_ptr<Queue []> work_queues;
	
	// Vector of thread objects which make up the pool
	std::vector<std::thread> threads;
	
	// Entry point function for the threads
	void worker(int idx);
	
	// Returns a random number between 0 and thread_count
	int get_random();
	
	// The number of threads in the pool
	int thread_count;
public:
	thread_pool();
	~thread_pool();
	
	// Add a task to the queue
	void submit(Func func);
};

#endif //THREAD_POOL_H