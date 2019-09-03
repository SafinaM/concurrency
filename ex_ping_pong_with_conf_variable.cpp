//
// Created by mar on 01.09.19.
//
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <thread>


class PingPongPlayer {
public:
	PingPongPlayer(const std::chrono::milliseconds interval, const uint32_t number) :
	_timeInterval(interval),
	_numberOfRepetitions(number){}
	
	void pingPong() {
		std::thread { [&](){ ping(); } }.detach();
		std::thread { [&](){ pong(); } }.detach();
	}
	
	~PingPongPlayer() {
	
	}

private:
	std::condition_variable _conditionVariable;
	std::mutex _mutex;
	std::chrono::milliseconds _timeInterval;
	bool _finished = false;
	bool _notified = false;
	uint32_t _numberOfRepetitions;
	
	void ping() {
		while(!_finished) {
			for (int i = 0; i < _numberOfRepetitions; ++i) {
				std::this_thread::sleep_for(std::chrono::milliseconds(_timeInterval));
				std::cout << i << ") Ping!" << " " << std::this_thread::get_id() << std::endl;
				std::unique_lock<std::mutex>(_mutex);
				_notified = true;
				_conditionVariable.notify_one();
				std::this_thread::yield();
			}
			_finished = true;
			_conditionVariable.notify_one();
		}
	};
	
	void pong(){
		while(!_finished) {
			std::unique_lock<std::mutex> lock(_mutex);
			while (!_notified) {
				_conditionVariable.wait(lock);
			}
			std::cout << "Pong!" << " " << std::this_thread::get_id() << std::endl;
			_notified = false;
			std::this_thread::sleep_for(std::chrono::milliseconds(_timeInterval));
			std::this_thread::yield();
		}
	};
};

int main() {
	{
		std::chrono::milliseconds ms(50);
		const uint32_t number = 100;
		PingPongPlayer player(ms, number);
		player.pingPong();
		std::this_thread::sleep_for(std::chrono::milliseconds(ms * number * 2));
	}

	
	
//	bool notified = false;
//	bool done = false;
//	std::mutex m;
//	std::condition_variable cond;
//	std::chrono::milliseconds period(200);
//	auto ping = [&](const uint32_t num) {
//		while(!done) {
//			for (int i = 0; i < num; ++i) {
//				std::this_thread::sleep_for(std::chrono::milliseconds(period));
//				std::cout << "Ping!" << " " << std::this_thread::get_id() << std::endl;
//				std::unique_lock<std::mutex>(m);
//				notified = true;
//				cond.notify_one();
//				std::this_thread::yield();
//			}
//			done = true;
//			cond.notify_one();
//		}
//	};
//
//	auto pong = [&]() {
//		while(!done) {
//			std::unique_lock<std::mutex> lock(m);
//			while (!notified) {
//				cond.wait(lock);
//			}
//			std::cout << "Pong!" << " " << std::this_thread::get_id() << std::endl;
//			notified = false;
//			std::this_thread::sleep_for(std::chrono::milliseconds(period));
//		}
//	};
//	const uint32_t number = 5;
//	std::thread ping_t, pong_t;
//	ping_t = std::thread(ping, number);
//	pong_t = std::thread(pong);
//	ping_t.join();
//	pong_t.join();
	
	return 0;
}