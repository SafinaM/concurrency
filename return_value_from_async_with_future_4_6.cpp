#include <future>
#include <iostream>
#include <chrono>
#include "common_listing_4.h"

template<typename R>
bool is_ready(std::future<R> const& f)
{ return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

//  modeling of real long task
std::string find_the_answer_to_ltuae() {
	auto delay = get_delay();
	std::this_thread::sleep_for(delay);
	std::cout << "id of async thread is " << std::this_thread::get_id() << ", was delayed on " << delay.count()  << std::endl;
	return "time for answer finding is " + std::to_string(delay.count());
}

int main() {
	std::future<std::string> answer = std::async(find_the_answer_to_ltuae);
	uint32_t n = 0;
	// count the number of iterations befor we get the answer on Life, The Univers and Everything
	while (!is_ready(answer)) {
		std::cout << n << ") "<< std::this_thread::get_id() << std::endl;
		++n;
	}
	std::cout << answer.get() << std::endl;
	return 0;
}