//
// Created by mar on 03.07.19.
// two threads using a hierarchical mutex

hierarchical_mutex high_level_mutex(10000);
hierarchical_mutext low_level_mutext(5000);
int do_low_level_stuff();

int low_level_func() {
	std::lock_guard<hierarchical_mutex> lk(low_level_mutext);
	return do_low_level_stuff();
}

void high_level_stuff(int some_param);

void high_level_func() {
	std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
	high_level_stuff(low_level_func());
}

void thread_a() {
	high_level_func();
}

hierarchical_mutex other_mutex(100);
void do_other_stuff();

void other_stuff() {
	high_level_func();
	do_other_stuff();
}

void thread_b() {
	std::lock_guard<hierarchical_mutex> lk(other_mutex);
	other_stuff();
}