#include <exception>
#include <memory>
#include <mutex>
#include <thread>
#include <stack>
#include <list>
#include <string>
#include <iostream>

class empty_list: public std::exception
{
public:
	/** Constructor (C strings).
	 *  @param message C-style string error message.
	 *                 The string contents are copied upon construction.
	 *                 Hence, responsibility for deleting the char* lies
	 *                 with the caller.
	 */
	explicit empty_list(const char* message):
		msg_(message)
	{
	}
	
	/** Constructor (C++ STL strings).
	 *  @param message The error message.
	 */
	explicit empty_list(const std::string& message):
		msg_(message)
	{}
	
	/** Destructor.
	 * Virtual to allow for subclassing.
	 */
	virtual ~empty_list() throw (){}
	
	/** Returns a pointer to the (constant) error description.
	 *  @return A pointer to a const char*. The underlying memory
	 *          is in posession of the Exception object. Callers must
	 *          not attempt to free the memory.
	 */
	virtual const char* what() const throw (){
		return msg_.c_str();
	}

protected:
	/** Error message.
	 */
	std::string msg_;
};

struct Tmp {
	int data = 0;
	uint32_t now;
	Tmp(const int& data = 0): data(data), now(std::chrono::system_clock::now().time_since_epoch().count()) {}
	Tmp&operator= (const Tmp& other) {
		if(&other == this)
			return *this;
		data = other.data;
		now = other.now;
		return *this;
	}
	
	Tmp(const Tmp& other): data(other.data), now(other.now) {}
	
	friend bool operator<(const Tmp& left, const Tmp& right);
};

bool operator<(const Tmp& left, const Tmp& right) {
	return left.data < right.data;
}

template <typename T>
class threadsafe_list {
private:
	std::list<T> data;
	mutable std::mutex m;
public:
	threadsafe_list() {}
	
	threadsafe_list(const threadsafe_list& other) {
		std::lock_guard<std::mutex> lock(other.m);
		// copy performed in constructor body
		data = other.data;
	}
	threadsafe_list& operator=(const threadsafe_list&) = delete;
	
	void push_back(const T& new_value) {
		std::lock_guard<std::mutex> lock(m);
		data.push_back(new_value);
	}
	
	void push_front(const T& new_value) {
		std::lock_guard<std::mutex> lock(m);
		data.push_front(new_value);
	}
	
	void pop_back(T& value) {
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) {
			std::cerr << "empty list!" << std::endl;
			return;
		}
		value = data.back();
		data.pop_back();
	}
	
	void pop_front(T& value) {
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) {
			std::cerr << "empty list!" << std::endl;
			return;
		}
		value = data.front();
		data.pop_front();
	}
	
	bool empty() const {
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
	
	bool clear() const {
		std::lock_guard<std::mutex> lock(m);
		return data.clear();
	}
	
	bool size() const {
		std::lock_guard<std::mutex> lock(m);
		return data.size();
	}
	
	template <typename Iter>
	bool insert(Iter it, const T& value) const {
		std::lock_guard<std::mutex> lock(m);
		return data.insert(it, value);
	}
	
	template <typename Iter>
	bool erase(Iter it) const {
		std::lock_guard<std::mutex> lock(m);
		return data.erase(it);
	}
	
	void sort() {
		std::lock_guard<std::mutex> lock(m);
		data.sort();
	}
};
template <class T>
void fillVector(threadsafe_list<T>& list, T value) {
	list.push_front(value);
}

template <class T>
void pop_front(threadsafe_list<T>& list, T& value) {
	list.pop_front(value);
}

void pop_back(threadsafe_list<Tmp>& list, Tmp& value) {
	list.pop_back(value);
	std::cout << value.data << "  " << value.now << std::endl;
}

template <class T>
void sortList(threadsafe_list<T>& list) {
	list.sort();
}

int main() {
	threadsafe_list<Tmp> list;
	const long long numberOfElements = 10000;
	for (uint32_t i = 0; i < numberOfElements; ++i) {
		Tmp tmp1 = Tmp(numberOfElements - i);
		std::thread t1(fillVector<Tmp>, std::ref(list), tmp1);
		Tmp tmp2 = Tmp(numberOfElements - i);
		std::thread t2(fillVector<Tmp>, std::ref(list), tmp2);
		Tmp value;
		std::thread t4(pop_back,  std::ref(list), std::ref(value));
		
		std::thread t6(sortList<Tmp>, std::ref(list));
		
		t1.join();
		t2.join();
		t4.join();
		t6.join();
	}
	return 0;
}