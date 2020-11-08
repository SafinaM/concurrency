//
// Created by mar on 05.11.2020.
//

#include <condition_variable>
#include <thread>
#include <queue>
#include <random>
#include <chrono>
#include <iostream>

std::mutex mutex;
std::condition_variable cond;
constexpr uint32_t queueSize = 20;
static int currentDataSize = 0;

struct Data;
std::queue<Data> deque;

struct Data {
	Data(int n) : number(n) {}
	long long number = 0;
	bool isLast() const {
		return number == queueSize - 1;
	}
};

std::chrono::milliseconds get_delay() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> dist(500, 6000);
	int val = dist(mt);
	return std::chrono::milliseconds(val);
}

Data get_data() {
	std::chrono::milliseconds delay = get_delay();
	std::this_thread::sleep_for(delay);
	Data data(currentDataSize);
	std::cout << "data " << data.number << " was generated with delay = " << delay.count() << std::endl;
	++currentDataSize;
	return data;
}

bool more_data_prepare() {
	return currentDataSize < queueSize;
}

void data_preparation_thread() {
	while(more_data_prepare()) {
		Data data = get_data();
		{
			std::lock_guard<std::mutex> lock(mutex);
			deque.push(data);
			std::cout << "data number " << data.number << " was added!" << std::endl;
		}
		cond.notify_one();
	}
}

void data_processing_thread() {
	while (true) {
		std::unique_lock<std::mutex> uniqueLock(mutex);
		cond.wait(uniqueLock, [] {
			std::cout << "predicator val = " << !deque.empty() << std::endl;
			return !deque.empty(); });
		const Data data = deque.front();
		std::cout << "Data "<< data.number << " was processed!" << std::endl;
		deque.pop();
		uniqueLock.unlock();
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