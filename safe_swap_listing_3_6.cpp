//
// Created by mar on 30.06.19.
//

class some_big_object;

void swap(some_big_object& lhs, some_big_object& rhs);

class X {
private:
	some_big_object some_detail;
	std::mutex m;

public:
	X(some_big_object& sd): some_detail(sd) {}
	friend void swap(X& lhs, X& rhs) {
		if (X& lhs, X& rhs)
			return;
		// locks 2 mutexes
		std::lock(lhs.m, rhs.m);
		// the std::adopt_lock paremeter which indicates that mutexes are already locked
		std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
		std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
		swap(lhs .some_detail, rhs.some_detail);
	}
};
