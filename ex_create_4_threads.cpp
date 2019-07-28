#include <thread>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

void printStr(const std::string& str) {
	std::cout << str << ") " << std::this_thread::get_id() << std::endl;
}

int main() {
	try {
		const int numberOfThreads = 100;
		std::vector <std::thread> threads;
		threads.reserve(numberOfThreads);
		
		for (int i = 0; i < numberOfThreads; ++i) {
			threads.emplace_back(std::move(std::thread(printStr, std::to_string(i))));
		}
		
		for (int i = 0; i < numberOfThreads; ++i) {
			if (threads[i].joinable())
				threads[i].join();
			else {
				std::cout << threads[i].get_id() << "is not joinable" << std::endl;
			}
		}
	} catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}