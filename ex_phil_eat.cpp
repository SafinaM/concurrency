/*
 * File:   din_phil.c
 * Author: nd159473 (Nickolay Dalmatov, Sun Microsystems)
 * adapted from http://developers.sun.com/sunstudio/downloads/ssx/tha/tha_using_deadlock.html
 *
 * Created on January 1, 1970, 9:53 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <cstdint>
#include <algorithm>

#define PHILO 5
#define DELAY 30000
#define FOOD 50

pthread_mutex_t forks[PHILO];
pthread_t phils[PHILO];
int countOfEatings[PHILO];
int sortedCountofEatings[PHILO];
void *philosopher (void *id);
int food_on_table ();
void get_fork (int, int, const char *);
void down_forks (int, int);
pthread_mutex_t foodlock;
int f = FOOD;
int sleep_seconds = 0;

int main(int argc, char **argv)
{
	for (int i = 0; i < PHILO; ++i) {
		countOfEatings[i] = 0;
		sortedCountofEatings[0] = 0;
	}
	int i;
	if (argc == 2)
		sleep_seconds = atoi(argv[1]);
	
	pthread_mutex_init(&foodlock, NULL);
	for (i = 0; i < PHILO; i++)
		pthread_mutex_init(&forks[i], NULL);
	for (i = 0; i < PHILO; i++) {
		pthread_create(&phils[i], NULL, philosopher, &i);
	}
	for (i = 0; i < PHILO; i++)
		pthread_join (phils[i], NULL);
	
	for (int i = 0; i < PHILO; ++i) {
		printf("Philosofer %d, eats %d times\n", i, countOfEatings[i]);
	}
	return 0;
}

void*
philosopher(void *num)
{
	int number = *static_cast<int*>(num);
	printf("num %d\n", number);
	int id;
	int left_fork, right_fork;
	id = (intptr_t)number;
	printf("Philosopher %d sitting down to dinner.\n", id);
	right_fork = id;
	left_fork = id + 1;
	
	/* Wrap around the forks. */
	if (left_fork == PHILO)
		left_fork = 0;
	while (f > 0) {
		if (f == 0)
			break;
		/* Thanks to philosophers #1 who would like to
		 * take a nap before picking up the forks, the other
		 * philosophers may be able to eat their dishes and
		 * not deadlock.
		 */
		int min = 0;
		int max = 0;
		for (int i = 0; i < PHILO; ++i) {
			if (countOfEatings[min] > countOfEatings[i])
				min = i;
			if (countOfEatings[max] < countOfEatings[i])
				max = i;
		}
		
		if (max == number)
			sleep(sleep_seconds);
		get_fork(id, left_fork, "left");
		get_fork(id, right_fork, "right");
	
		f = food_on_table();
		++countOfEatings[number];
		printf("Philosopher %d: get dish %d.\n", id, f);
		printf("Philosopher %d: eating.\n", id);
		usleep(DELAY * (FOOD - f + 1));
		down_forks(right_fork, left_fork);
		if (!f)
			break;
		printf("Philosopher %d is done eating.\n", id);
	}
	return (NULL);
}

int
food_on_table()
{
	static int food = FOOD;
	int myfood;
	printf("FOOD %d\n", food);
	pthread_mutex_lock(&foodlock);
	if (food > 0) {
		food--;
	}
	myfood = food;
	pthread_mutex_unlock(&foodlock);
	return myfood;
}

void
get_fork(int phil,
		 int fork,
		 const char *hand)
{
	pthread_mutex_lock(&forks[fork]);
	printf("Philosopher %d: got %s fork %d\n", phil, hand, fork);
}

void
down_forks (int f1, int f2)
{
	pthread_mutex_unlock (&forks[f1]);
	pthread_mutex_unlock (&forks[f2]);
}