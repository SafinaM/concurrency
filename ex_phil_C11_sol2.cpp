#include <array>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include <map>
#include <iomanip>

std::mutex g_lockPrint;
constexpr int numberOfPhilosophers = 5;

struct fork
{
	std::mutex mutex;
};

struct table
{
	std::atomic<bool>                    ready{ false };
	std::array<fork, numberOfPhilosophers> forks;
};

std::map<std::string, int> counter;

struct philosopher
{
private:
	std::string const name;
	table const &     dinnerTable;
	fork&             leftFork;
	fork&             rightFork;
	std::thread       lifeThread;
	std::mt19937      rng{ std::random_device{}() };
public:
	
	
	philosopher(const std::string& n, table const & t, fork & l, fork & r) :
		name(n),
		dinnerTable(t),
		leftFork(l),
		rightFork(r),
		lifeThread(&philosopher::dine, this)
	{
	}
	
	~philosopher()
	{
		lifeThread.join();
	}
	
	void dine()
	{
		while (!dinnerTable.ready);
		
		do
		{
			think();
			eat();
		} while (dinnerTable.ready);
	}
	
	void print(const std::string& text)
	{
		std::lock_guard<std::mutex> cout_lock(g_lockPrint);
		std::cout
			<< std::left << std::setw(10) << std::setfill(' ')
			<< name << text << std::endl;
		++counter[name];
	}
	
	void eat()
	{
		std::lock(leftFork.mutex, rightFork.mutex);
		
		std::lock_guard<std::mutex> left_lock(leftFork.mutex,   std::adopt_lock);
		std::lock_guard<std::mutex> right_lock(rightFork.mutex, std::adopt_lock);
		
		print(" started eating.");
		
		static thread_local std::uniform_int_distribution<> dist(1, 6);
		std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng) * 10));
		print(" finished eating.");
	}
	
	void think()
	{
		static thread_local std::uniform_int_distribution<> wait(1, 6);
		std::this_thread::sleep_for(std::chrono::milliseconds(wait(rng) * 5));
		
		print(" is thinking ");
	}
};

void dine()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Dinner started!" << std::endl;
	
	{
		table table;
		std::array<philosopher, numberOfPhilosophers> philosophers
			{
				{
					{ "Aristotle", table, table.forks[0], table.forks[1] },
					{ "Platon",    table, table.forks[1], table.forks[2] },
					{ "Descartes", table, table.forks[2], table.forks[3] },
					{ "Kant",      table, table.forks[3], table.forks[4] },
					{ "Nietzsche", table, table.forks[4], table.forks[0] },
				}
			};
		
		table.ready = true;
		std::this_thread::sleep_for(std::chrono::seconds(20));
		table.ready = false;
	}
	
	std::cout << "Dinner done!" << std::endl;
}

int main()
{
	dine();
	for (const auto& elem:counter)
		std::cout <<  std::setw(10) << elem.first << " eats " << std::setw(10) << elem.second << std::endl;
	
	return 0;
}