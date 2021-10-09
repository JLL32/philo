#include "../include/philo.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#define HAS_FORK "has taken a fork"
#define EATING "is eating"
#define SLEEPING "is sleeping"
#define THINKING "is thinking"
#define DEAD "died"

void put_state(int id, char *state)
{
	printf("%d %s\n", id, state);
}

void eating(t_philo *philo)
{
	static size_t eating_times = 0;
	if (eating_times == philo->eating_times)
	{
		philo->next = dead;
	}
	else
	{
		put_state(philo->id, EATING);
		usleep(philo->time_to_eat);
		eating_times++;
		philo->next = sleeping;
	}
}

void sleeping(t_philo *philo)
{
	put_state(philo->id, SLEEPING);
	usleep(philo->time_to_sleep);
	philo->next = thinking;
}

void thinking(t_philo *philo)
{
	put_state(philo->id, THINKING);
	usleep(philo->time_to_die);
	philo->next = eating;
}

void dead(t_philo *philo)
{
	put_state(philo->id, DEAD);
	philo->next = NULL;
}

void init(t_philo *philo)
{
	put_state(philo->id, HAS_FORK);
	philo->next = eating;
	while(philo->next)
	{
		philo->next(philo);
	}
}

int main(void) {
	t_philo philo = {
		.id = 1,
		.time_to_eat = 1000000,
		.time_to_sleep = 1000000,
		.time_to_die = 2000000,
		.eating_times = 5,
	};
	init(&philo);
}
