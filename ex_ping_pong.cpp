//
// Created by mar on 01.09.19.
//

#include <iostream>
#include <thread>
#include <atomic>
#include <string>
#include <future>
using namespace std;

// Ping pong function for thread
void ping_pong(string name, std::chrono::milliseconds time) {
	for (int i = 0; i < 3; ++i) {
		std::cout << name << " " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(time));
	}
}

template<typename R>
bool is_ready(std::future<R> const& f)
{ return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

class CPingPong final {
public:
	CPingPong(const std::chrono::milliseconds time = std::chrono::milliseconds::zero()):
		m_ms(time) {
	}
	
	~CPingPong() {
		future.get();
	}
	
	void StartPingPong() {
		m_isAlive.store(true);
		thread { [&](){ Ping(); } }.detach();
		thread { [&](){ Pong(); } }.detach();
		future = std::async(std::launch::async, [&]() {
			this_thread::sleep_for(chrono::milliseconds(m_ms));
			m_isAlive.store(false);
		});
	}
private:
	void Ping() {
		while (m_isAlive.load()) {
			if (!m_bFlag.load()) {
				cout << "Ping " << this_thread::get_id() << " " <<  endl;
				m_bFlag.store(true);
				this_thread::sleep_for(chrono::milliseconds(100));
				this_thread::yield();
			}
		}
	}
	
	void Pong() {
		while (m_isAlive.load()) {
			if (m_bFlag.load()) {
				cout << "Pong " << this_thread::get_id() << endl;
				m_bFlag.store(false);
				this_thread::sleep_for(chrono::milliseconds(100));
				this_thread::yield();
			}
		}
	}

private:
	atomic_bool m_isAlive;
	atomic_bool m_bFlag;
	std::chrono::milliseconds m_ms;
	std::future<void> future;
};

int main() {

//	{
//		std::chrono::milliseconds period(200);
//		CPingPong o(period);
//		o.StartPingPong();
////		this_thread::sleep_for(chrono::milliseconds(1000));
//	}
	
	std::thread ping_t, pong_t;
	
	std::cout << "Ready… Set… Go!\n";
	
	//Launch ping and pong threads
	std::chrono::milliseconds duration(10);
	ping_t = std::thread(ping_pong, "Ping!", duration);
	pong_t = std::thread(ping_pong, "Pong!", duration);
	
	ping_t.join();
	pong_t.join();
	
	return 0;
}