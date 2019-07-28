#include <exception>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <stack>

struct empty_stack: std::exception {
	const char* what() const throw();
};

template <typename T>
class threadsafe_list {
private:
	std::stack<T> data;
	mutable std::mutex m;
public:
	threadsafe_list() {}
	
	threadsafe_list(const threadsafe_list& other) {
		std::lock_guard<std::mutex> lock(other.m);
		// copy performed in constructor body
		data = other.data;
	}
	threadsafe_list& operator=(const threadsafe_list&) = delete;
	
	void push(T new_value) {
		std::lock_guard<std::mutex> lock(m);
		data.push(new_value);
	}
	
	std::shared_ptr<T> pop() {
		std::lock_guard<std::mutex> lock(m);
		// check for empty before trying to pop value
		if (data.empty()) throw empty_stack();
		// allocate return value before modifying stack
		std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
		data.pop();
		return res;
	}
	
	void pop(T& value) {
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) throw empty_stack();
		value = data.top();
		data.pop();
	}
	
	bool empty() const {
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};

int main() {
	return 0;
}