//
// Created by mar on 02.07.20.
//

#include <thread>
#include <exception>
#include <iostream>

void do_something(int i) {
	std::cout << i << std::endl;
	
}

struct func {
	int& i;
	func (int& i) : i(i) {}
	
	void operator() () {
		for (int j = 0; j < 1000; ++j)
			do_something(i); // potential access to dangling reference
	}
};

void do_something_in_current_thread() {
	for (int i = 0; i < 100000; ++i) {
		if (i == 50)
//			throw std::out_of_range("out_of_range");
		std::cout << "something in current state: " << i << std::endl;
	}
}

class scoped_thread {
private:
	std::thread t;
public:
	explicit scoped_thread(std::thread t_) : t(std::move(t_)) {
		if (!t.joinable()) {
			throw std::logic_error("no thread!");
		}
	}
	
	scoped_thread(const scoped_thread&) = delete;
	scoped_thread& operator=(const scoped_thread&) = delete;
	
	~scoped_thread() {
		t.join();
	}
};

void f() {
	int some_local_state = 1;
	scoped_thread t{std::thread(func(some_local_state))};
	do_something_in_current_thread();
}

int main() {
	f();
	return 0;
}