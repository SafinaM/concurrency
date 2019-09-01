//
// Created by mar on 01.09.19.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <thread>

int main() {
	bool notified = false;
	bool done = false;
	std::mutex m;
	std::condition_variable cond;
	std::chrono::milliseconds period(200);
	auto ping = [&](const uint32_t num) {
		while(!done) {
			for (int i = 0; i < num; ++i) {
				std::this_thread::sleep_for(std::chrono::milliseconds(period));
				std::cout << "Ping!" << " " << std::this_thread::get_id() << std::endl;
				std::unique_lock<std::mutex>(m);
				notified = true;
				cond.notify_one();
				std::this_thread::yield();
			}
			done = true;
			cond.notify_one();
		}
	};
	
	auto pong = [&]() {
		while(!done) {
			std::unique_lock<std::mutex> lock(m);
			while (!notified) {
				cond.wait(lock);
			}
			std::cout << "Pong!" << " " << std::this_thread::get_id() << std::endl;
			notified = false;
			std::this_thread::sleep_for(std::chrono::milliseconds(period));
		}
	};
	const uint32_t number = 5;
	std::thread ping_t, pong_t;
	ping_t = std::thread(ping, number);
	pong_t = std::thread(pong);
	ping_t.join();
	pong_t.join();
	
	return 0;
}