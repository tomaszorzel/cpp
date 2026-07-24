// Simple concurrent queue implementation
// pop() busy waits when the queue is empty
#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

#include <queue>
#include <mutex>

using namespace std::literals;

template <class T>
class concurrent_queue {
	std::timed_mutex mut;
	std::queue<T> que;
	int max{50};
public:
	concurrent_queue() = default;
	concurrent_queue(int max) : max(max) {};
	
	bool try_push(T value) {
		// Lock the mutex with a time-out
		std::unique_lock<std::timed_mutex> lck_guard(mut, std::defer_lock);
		
		// Cannot lock - return immediately
		if (!lck_guard.try_lock_for(1ms)  || que.size() > max) {
			return false;
		}
		
		// Locked - add the element to the queue
		que.push(value);
		
		return true;
	}

	bool try_pop(T& value) {
		// Lock the mutex with a time-out
		std::unique_lock<std::timed_mutex> lck_guard(mut, std::defer_lock);
		
		// Cannot lock - return immediately
		if (!lck_guard.try_lock_for(1ms) || que.empty()) {
			return false;
		}
		
		// Locked - remove front element from the queue
		value = que.front();
		que.pop();
		
		return true;
	}
};

#endif //CONCURRENT_QUEUE_H