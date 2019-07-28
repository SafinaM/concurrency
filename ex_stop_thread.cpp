#include <stdio.h>
#include <thread>
#include <iostream>

bool stopped = false;
bool cleaned = false;

void printSomething(const std::string& passedStr) {
	while (!stopped) {
		std::cout << passedStr << std::endl;
	}
	if (stopped) {
		std::cout << "pthread_cleanup_push" << std::endl;
		cleaned = true;
	}
}

int main() {
	std::thread thr(printSomething, "child thread");
	std::this_thread::sleep_for(std::chrono::seconds(2));
	stopped = true;
	thr.join();
	
	if (cleaned)
		std::cout << "pthread_cancel" << std::endl;
	
	return 0;
}