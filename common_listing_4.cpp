#include <thread>
#include <random>
#include <chrono>
#include <iostream>
#include "common_listing_4.h"

Data::Data(int n) : number(n) {}

bool Data::isLast() const {
	return number == queueSize - 1;
}

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

