//
// Created by mar on 12.11.2020.
//

#include "common_listing_4.h"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

template <typename T>
class thread_safe_queue {
public:
	thread_safe_queue() = default;
	thread_safe_queue(const thread_safe_queue& other) {
		std::lock_guard<std::mutex> lk(other.mut);
		m_queue = other.data_queue;
	}

	void push(const T& value) {
		std::lock_guard<std::mutex>lockGuard(m_mutex);
		m_queue.push(value);
		m_cond.notify_one();
	}

	bool try_pop(T& value) {
		if (m_queue.empty())
			return false;
		value = m_queue.front();
		m_queue.pop();
	}

	std::shared_ptr<T> try_pop() {
		if (m_queue.empty())
			return false;
		std::shared_ptr<T> ptr = std::make_shared(m_queue.front());
		m_queue.pop();
		return ptr;
	}

	void wait_and_pop(T& value) {
		std::unique_lock<std::mutex> uniqueLock(m_mutex);
		m_cond.wait(uniqueLock, [this]() {
			return !m_queue.empty();
		});
		value = m_queue.front();
		m_queue.pop();
	}

	std::shared_ptr<T> wait_and_pop(const T& value) {
		std::unique_lock<std::mutex> uniqueLock(m_mutex);
		m_cond.wait(uniqueLock, [this]() {
			return !m_queue.empty();
		});
		std::shared_ptr<T> ptr = std::make_shared(m_queue.front());
		m_queue.pop();
		return ptr;
	}

	bool empty() {
		std::lock_guard<std::mutex> lockGuard(m_mutex);
		return m_queue.empty();
	}

private:
	mutable std::mutex m_mutex;
	std::condition_variable m_cond;
	std::queue<T> m_queue;
};

thread_safe_queue<Data> data_queue;

void data_preparation_thread() {
	while(more_data_prepare()) {
		Data data = get_data();
		data_queue.push(data);
		std::cout << "data number " << data.number << " was added!" << std::endl;
	}
}

void data_processing_thread() {
	while (true) {
		Data data;
		data_queue.wait_and_pop(data);
		std::cout << "Here data should be processed!" << std::endl;
		if (data.isLast()) {
			break;
		}
	}
}

int main() {

	std::thread thr1(data_preparation_thread);
	std::thread thr2(data_processing_thread);

	thr1.join();
	thr2.join();
	return 0;
}