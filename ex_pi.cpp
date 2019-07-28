#include <iostream>
#include <stdio.h>
#include <vector>
#include <thread>
#include <future>
#include <math.h>
#include <signal.h>
#include <iomanip>

static volatile int keepRunning = 1;

void intHandler(int sig) {
	keepRunning = 0;
}
const int stepOfFlagVerifying = 100;

double calculatePi(long long begin, long long end) {
	double pi = 0;
	for (long long i = begin; i < end ; i++) {
		pi += 1.0 / (i * 4.0 + 1.0);
		pi -= 1.0 / (i * 4.0 + 3.0);
		if (i % stepOfFlagVerifying == 0 && !keepRunning)
			break;
	}
	std::cout << pi * 4 << "  " << std::this_thread::get_id() << std::endl;
	return pi;
}

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "incorrect number of argc. pass number of steps and number of threads" << std::endl;
		exit(1);
	}
	
	signal(SIGINT, intHandler);
	long long numSteps = atoi(argv[1]);
	const int numThreads = atoi(argv[2]);
	std::vector<std::future<double >> futures;
	futures.reserve(numThreads);
	for (int i = 0; i < numThreads; ++i) {
		futures.emplace_back(std::move(std::async(std::launch::async, calculatePi, i * numSteps, (i + 1) * numSteps)));
	}

	double pi = 0.0;
	for (int i = 0; i < numThreads; i++) {
		pi += futures[i].get();
	}
	pi *= 4.0;
	printf("pi done - %.15g \n", pi);
	
	return 0;
}
