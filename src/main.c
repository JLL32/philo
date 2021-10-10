#include "../include/philo.h"
#include <stddef.h>
#include <stdio.h>
#include <sys/_types/_timeval.h>
#include <unistd.h>
#include <sys/time.h>

#define HAS_FORK "has taken a fork"
#define EATING "is eating"
#define SLEEPING "is sleeping"
#define THINKING "is thinking"
#define DEAD "died"

void put_state(int id, char *state)
{
	printf("%d %s\n", id, state);
}

void block_thread(size_t ms)
{
	const size_t micro_s = ms * 1000;
	usleep(micro_s);
}

size_t get_time()
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void eating(t_philo *philo)
{
	if (philo->eating_times == 0)
	{
		philo->next = dead;
		return;
	}
	put_state(philo->id, HAS_FORK);
	put_state(philo->id, HAS_FORK);
	put_state(philo->id, EATING);
	block_thread(philo->time_to_eat);
	philo->eating_times--;
	philo->next = sleeping;
}

void sleeping(t_philo *philo)
{
	put_state(philo->id, SLEEPING);
	block_thread(philo->time_to_sleep);
	philo->next = thinking;
}

void thinking(t_philo *philo)
{
	put_state(philo->id, THINKING);
	block_thread(philo->remaining_life_time);
	if (philo->eating_times)
		philo->next = eating;
	else
		philo->next = dead;
}

void dead(t_philo *philo)
{
	put_state(philo->id, DEAD);
	philo->next = NULL;
}

void init(t_philo *philo)
{
	philo->next = eating;
	philo->starting_time = get_time();
	while(philo->next)
	{
		philo->next(philo);
	}
}

int main(void) {
	t_philo philo = {
		.id = 1,
		.time_to_eat = 1000,
		.time_to_sleep = 1000,
		.life_time = 1000,
		.eating_times = 3,
		.remaining_life_time = 1000,
		.starting_time = 0,
	};
	init(&philo);
}
