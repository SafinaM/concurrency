// Main thread reads data from stdin. Data is strings which user enters.
// New string should be placed in the begin of list.
// If input string is empty the program should print current content of list.
// Child thread is waking up every 5 seconds and sorts list.
// All operations with list should be synchronized with help of mutex.

#include <iostream>
#include <thread>
#include <list>
#include <mutex>
#include <condition_variable>

std::list<std::string> words;
std::mutex mutex;
bool finished = false;

void addToList() {
	std::string str;
	while(std::getline(std::cin, str)) {
		{
			std::lock_guard<std::mutex> lockGuard(mutex);
			words.push_back(str);
		}
		if (str == "") {
			for (const auto& elem: words)
				std::cout << elem << std::endl;
		} else if (str == "q") {
			break;
		}
	}
	finished = true;
}

void sortList() {
	while (!finished) {
		{
			std::lock_guard<std::mutex> lockGuard(mutex);
			words.sort([](const std::string& str1, const std::string& str2) {
				return str1 < str2;
			});
		}
		for (const auto& elem: words)
			std::cout << elem << std::endl;
		std::cout << "sorting!!!" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));

	}
}

int main() {
	std::thread thread1 = std::thread(addToList);
	std::thread thread2 = std::thread(sortList);

	thread1.join();
	thread2.join();
	return 0;
}