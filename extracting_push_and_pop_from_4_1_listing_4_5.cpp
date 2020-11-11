#include <condition_variable>
#include <memory>
#include <thread>
#include <queue>
#include <random>
#include <chrono>
#include <iostream>
#include "common_listing_4.h"

template<typename T>
class threadsafe_queue {
public:
	threadsafe_queue() = default;
	threadsafe_queue(const threadsafe_queue&) = delete;
	threadsafe_queue& operator=(const threadsafe_queue&) = delete;
	void push(T new_value) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queue.push(new_value);
		m_cond.notify_one();
	}
	bool try_pop(T& value) {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_queue.empty())
			return false;
		value = m_queue.front();
		return true;
	}
	std::shared_ptr<T> try_pop() {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_queue.empty())
			return std::shared_ptr<Data>();
		std::shared_ptr<Data> data_ptr = std::make_shared<Data>(m_queue.front());
		return data_ptr;
	}
	void wait_and_pop(T& value) {
		std::unique_lock<std::mutex> uniqueLock(m_mutex);
		m_cond.wait(uniqueLock, [this] {
			return !m_queue.empty(); });
		value = m_queue.front();
		m_queue.pop();
	}
	std::shared_ptr<T> wait_and_pop(){
		std::unique_lock<std::mutex> uniqueLock(m_mutex);
		m_cond.wait(uniqueLock, [this] {
			return !m_queue.empty(); });
		std::shared_ptr<Data> data_ptr = std::make_shared(m_queue.front());
		m_queue.pop();
		return data_ptr;
	}

	bool empty() const {
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.empty();
	}

	int size() const {
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.size();
	}

private:
	std::condition_variable m_cond;
	std::mutex m_mutex;
	std::queue<T> m_queue;
};

threadsafe_queue<Data> data_queue;

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