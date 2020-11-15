#include <iostream>
#include <thread>
#include <chrono>
#include <numeric>
#include <vector>
#include <cassert>

template <typename Iterator, typename Result>
struct accumulate_block {
	void operator()(Iterator fist, Iterator last, Result& result) {
		result = std::accumulate(fist, last, result);
	}
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T& init) {
	
	const uint32_t length = std::distance(first, last);
	
	if (!length)
		return init;
	
	const uint32_t number_hardware_threads = std::thread::hardware_concurrency();
	uint32_t number_of_threads = number_hardware_threads == 0 ? 2: number_hardware_threads;
	
	if (length <= number_of_threads)
		number_of_threads = 1;
	
	const uint32_t block_size = length / number_of_threads;
	
	std::vector<std::thread> threads(number_of_threads);
	std::vector<T> results(number_of_threads);
	uint32_t offset = 0;
	Iterator current_first = first;
	Iterator current_last = current_first;
	int i = 0;
	while (current_last != last) {
		if (offset + 2 * block_size < length) {
			std::advance(current_last, block_size);
		} else {
			current_last = last;
		}
		threads[i] = std::thread(accumulate_block<Iterator, T>(), current_first, current_last, std::ref(results[i]));
		current_first = current_last;
		offset += block_size;
		++i;
	}
	
	for (auto& thread: threads) {
		thread.join();
	}
	
	return std::accumulate(results.begin(), results.end(), 0);
}

int main() {
	
	std::vector<int> testVector0 = {1, 2, 3, 4, 20, 30, 12, 40, 50, 1, 10, 2, 8, 9, 12, 20, 3, 1};
	std::vector<int> testVector1 = {1, 2, 3, 4, 20, 30, 12, 40, 50, 1, 10, 2, 8, 9, 12, 45, 23, 30, 20};
	std::vector<int> testVector2 = {1, 2, 4, 5};
	std::vector<int> testVector3 = {1, 10};
	int result = 0;
	int sum0 = parallel_accumulate(testVector0.begin(), testVector0.end(), result);
	int sum1 = parallel_accumulate(testVector1.begin(), testVector1.end(), result);
	int sum2 = parallel_accumulate(testVector2.begin(), testVector2.end(), result);
	int sum3 = parallel_accumulate(testVector3.begin(), testVector3.end(), result);
	uint32_t result_for_test0 = std::accumulate(testVector0.begin(), testVector0.end(), 0);
	uint32_t result_for_test1 = std::accumulate(testVector1.begin(), testVector1.end(), 0);
	uint32_t result_for_test2 = std::accumulate(testVector2.begin(), testVector2.end(), 0);
	uint32_t result_for_test3 = std::accumulate(testVector3.begin(), testVector3.end(), 0);
	
	std::cout << "Reference: " << result_for_test0 << ", parallel version of algorithm: " << sum0 << std::endl;
	std::cout << "Reference: " << result_for_test1 << ", parallel version of algorithm: " << sum1 << std::endl;
	std::cout << "Reference: " << result_for_test2 << ", parallel version of algorithm: " << sum2 << std::endl;
	std::cout << "Reference: " << result_for_test3 << ", parallel version of algorithm: " << sum3 << std::endl;
	assert(result_for_test0 == sum0);
	assert(result_for_test1 == sum1);
	assert(result_for_test2 == sum2);
	assert(result_for_test3 == sum3);
	
	return 0;
}