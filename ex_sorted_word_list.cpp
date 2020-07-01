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
#include <vector>
#include <atomic>
#include <cassert>
#include <sstream>

std::vector<std::string> words = {
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf",
	"sdfsdfasdf"
};
std::vector<std::string> test = words;
std::vector<std::string> additionalWords;
std::mutex mutex;
std::atomic<bool> wordsAreLocked = {false};
bool finished = false;

void addToList() {
	std::string str;
	while(std::getline(std::cin, str)) {
		{
			test.push_back(str);
			if (!wordsAreLocked.load()) {
				std::lock_guard<std::mutex> lockGuard(mutex);
				if (!additionalWords.empty()) {
					for (const auto& elem: additionalWords) {
						words.push_back(elem);
					}
					additionalWords.clear();
				}
				words.push_back(str);
			} else {
				additionalWords.push_back(str);
			}
			
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

template <typename T>
bool cmp(const T& a, const T& b) {
	return a > b;
}

// rude bubble sorting
template <typename T>
void sort(std::vector<T>& v, bool (*cmp)(const T&, const T&)) {
	for (int i = 0; i < v.size(); ++i) {
		for (int j = i + 1; j < v.size(); ++j) {
			if (cmp(v[i],  v[j])) {
				std::swap(v[i], v[j]);
			}
		}
	}
}

void sortList() {
	while (!finished) {
		{
			wordsAreLocked.store(true);
			std::lock_guard<std::mutex> lockGuard(mutex);
			std::cerr << "sorting begin" << std::endl;
			for (int i = 0; i < 1000000; ++i) {
				sort(words, cmp);
			}
			std::cerr << "sorting end" << std::endl;
			
		}
		wordsAreLocked.store(false);
		for (const auto& elem: words)
			std::cout << elem << std::endl;
		std::cout << "sorting!!!" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		
	}
}

int main() {
	uint8_t arr[] = {130, 182, 88, 209, 130, 193};
	
	std::string s(arr, arr + sizeof(arr));
	
//	std::cout << s << std::endl;
	
	std::ostringstream convert;
	for (int i = 0; i < sizeof(arr); ++i) {
		convert << std::dec << int(arr[i]);
	}
	std::cout << convert.str() << std::endl;
	std::string s1 = convert.str();
	const uint8_t* p = reinterpret_cast<const uint8_t*>(s1.c_str());
	std::thread thread1 = std::thread(addToList);
	std::thread thread2 = std::thread(sortList);
	
	thread1.join();
	thread2.join();
	assert(test.size() == words.size());
	sort(test, cmp);
	sort(words, cmp);
	std::size_t size = test.size();
	for (int i = 0; i < size; ++i) {
		assert(test[i] == words[i]);
	}
	return 0;
}