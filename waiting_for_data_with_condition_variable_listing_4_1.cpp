#include <condition_variable>
#include <thread>
#include <queue>
#include <random>
#include <chrono>
#include <iostream>
#include "common_listing_4.h"

std::mutex mutex;
std::condition_variable cond;
std::queue<Data> data_queue;

void data_preparation_thread() {
	while(more_data_prepare()) {
		Data data = get_data();
		{
			std::lock_guard<std::mutex> lock(mutex);
			data_queue.push(data);
			std::cout << "data number " << data.number << " was added!" << std::endl;
		}
		cond.notify_one();
	}
}

void data_processing_thread() {
	while (true) {
		std::unique_lock<std::mutex> uniqueLock(mutex);
		cond.wait(uniqueLock, [] {
			std::cout << "predicator val = " << !data_queue.empty() << std::endl;
			return !data_queue.empty(); });
		const Data data = data_queue.front();
		std::cout << "Data "<< data.number << " was processed!" << std::endl;
		data_queue.pop();
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