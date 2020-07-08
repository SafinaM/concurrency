#include <iostream>
#include <thread>
#include <vector>

class joining_thread {
	
	std::thread t;

public:
	
	joining_thread() noexcept = default;
	
	template<typename Callable,typename ... Args>
	explicit joining_thread(Callable&& func, Args&& ... args) :
		t(std::forward<Callable>(func), std::forward<Args>(args)...)
	{}

	joining_thread(std::thread t_) noexcept :
		t(std::move(t)) {}
	
	joining_thread(joining_thread&& other) noexcept:
		t(std::move(other.t)) {}
	
	joining_thread& operator=(joining_thread&& other) noexcept {
		if(joinable())
			join();
		t = std::move(other.t);
		return *this;
	}
	
	joining_thread& operator=(std::thread other) noexcept {
		if(joinable())
			join();
		t = std::move(other);
		return *this;
	}
	
	void join() noexcept {
		t.join();
	}
	
	void detach() noexcept {
		t.detach();
	}
	
	bool joinable() {
		return t.joinable();
	}
	
	void swap(joining_thread& other) noexcept {
		t.swap(other.t);
	}
	
	std::thread::id get_id() const noexcept{
		return t.get_id();
	}
	
	~joining_thread() {
		if (joinable())
			join();
	}
	
};

void do_some_job(uint32_t i) {
	std::cout << i << ") "<< std::this_thread::get_id() << std::endl;
}

int main() {
	
	int max_number_ot_threads = std::thread::hardware_concurrency();
	
	joining_thread j_thread1(do_some_job, 10);
	joining_thread j_thread2 = std::move(j_thread1);
	
	return 0;
}

//	constexpr uint32_t number = 30;
//	std::vector<std::thread> threads;
//
//	for (uint32_t i = 0; i < number; ++i) {
//		threads.emplace_back(do_some_job, i);
//	}
//
//	for (uint32_t i = 0; i < number; ++i)
//		threads[i].join();
//