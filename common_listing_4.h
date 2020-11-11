#pragma once

#include <condition_variable>
#include <thread>
#include <queue>
#include <random>
#include <chrono>

constexpr uint32_t queueSize = 20;
static int currentDataSize = 0;
struct Data;

struct Data {
	Data(int n = 0);
	long long number = 0;
	bool isLast() const;
};

std::chrono::milliseconds get_delay();

Data get_data();
bool more_data_prepare();
