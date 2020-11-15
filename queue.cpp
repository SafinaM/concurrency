//
// Created by mar on 25.10.20.
//

#include <vector>
#include <mutex>
#include <initializer_list>

class StackException: std::exception {
public:
	StackException(const char* err): err(err) {}
	
	const char* what() const noexcept override {
		return err;
	}

private:
	const char* err;
};

template <typename T>
class Queue {

public:
	
	Queue() = default;
	Queue(std::initializer_list<T> list) : m_container{list}{ };
	Queue(const Queue& queue) = default;
	Queue(Queue&& queue) = default;
	Queue& operator=(const Queue& queue) = default;
	
	void push(T element) {
		m_container.emplace_back(std::move(element));
	}
	
	T pop(){
		if (empty()) {
			throw StackException("Stack is not empty");
		}
		T element = back();
		m_container.pop_back();
		return element;
	}
	
	const T& back() const {
		return m_container.back();
	}
	
	const T& front() const {
		return m_container.front();
	}
	
	std::size_t size() const {
		return m_container.size();
	}
	
	bool empty() const {
		return m_container.size() == 0;
	}
	
	void swap(const Queue& queue) {
		std::swap(m_container, queue.m_container);
	}

private:
	std::vector<T> m_container;
	std::mutex m_mutex;
};

int main() {
	Queue<int> q1;
	Queue<int> q2{1, 2, 3};
	
	
	return 0;
}