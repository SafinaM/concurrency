//
// Created by mar on 27.06.20.
//

#include <thread>
#include <iostream>
#include <vector>
#include <exception>

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

void ooops() {
	int some_local_state = 0;
	func my_func(some_local_state);
	std::thread my_thread(my_func);
	my_thread.detach(); // do not wait for thread to finish
	// new thread might still be running
}

void do_something_in_current_thread() {
	for (int i = 0; i < 100000; ++i) {
		if (i == 50)
			throw std::out_of_range("out_of_range");
		std::cout << "something in current state: " << i << std::endl;
	}
}

void f1() {
	int some_local_state = 0;
	func my_func(some_local_state);
	std::thread t(my_func);
	try {
		do_something_in_current_thread();
	} catch (const std::exception& e) {
		t.join();
		throw;
	}
	t.join();
}

struct thread_guard {
	std::thread& thr;
	thread_guard(std::thread& thr_) : thr(thr_)  {}
	
	~thread_guard() {
		if (thr.joinable())
			thr.join();
	}
	
	thread_guard(const thread_guard&) = delete;
	thread_guard& operator = (const thread_guard&) = delete;
};

void f2() {
	int some_local_state = 0;
	func my_func(some_local_state);
	std::thread t(my_func);
	thread_guard thr_guard(t);
	try {
		do_something_in_current_thread();
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void function() {

}

void some_other_function() {

}

int main() {
	// example of terminating
	std::thread t1 = std::thread(function);
	std::thread t2 = std::thread(some_other_function);
	std::thread t3;
	t3 = std::move(t2);
	
	// This assignment will terminate the program!
//	t3 = std::move(t1);
	
//	f1();
	f2();
//	ooops();
	return 0;
}