#include <thread>
#include <stdio.h>
#include <iostream>
bool finished = false;
void print(uint32_t n, const std::string& id) {
	for (uint32_t i = 0; i < n; ++i) {
		std::cout << i << " " << std::this_thread::get_id() << " " << id << std::endl;
	}
	finished = true;
}

int main() {
	const uint32_t numberOfRepeatings = 10000;
	print(10000, "parent");
	if (finished) {
		std::thread task1(print, numberOfRepeatings, "child");
		task1.join();
	}
	return 0;
}