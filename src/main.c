#include "../include/philo.h"
#include <stddef.h>
#include <stdio.h>
#include <sys/_types/_size_t.h>
#include <sys/_types/_timeval.h>
#include <unistd.h>
#include <sys/time.h>

#define HAS_FORK "has taken a fork"
#define EATING "is eating"
#define SLEEPING "is sleeping"
#define THINKING "is thinking"
#define DEAD "died"

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

size_t time_elapsed(size_t starting_time)
{
	return (get_time() - starting_time);
}

void put_state(const t_philo *philo, char *state)
{
	printf("%lu\t\t %d %s\n", time_elapsed(philo->first_starting_time), philo->id, state);
}

/*
** NOTE: Call it only when you are 100% sure that life_time >= time_elapsed
*/
size_t remaining_time(const t_philo *philo)
{
	return (philo->life_time - time_elapsed(philo->starting_time));
}

void eating(t_philo *philo)
{
	if (philo->eating_times == 0)
	{
		philo->next = dead;
		return ;
	}
	// Pick forks here
	if (time_elapsed(philo->starting_time) >= philo->life_time)
	{
		philo->next = dead;
		return;
	}
	put_state(philo, HAS_FORK);
	put_state(philo, HAS_FORK);
	put_state(philo, EATING);
	if (remaining_time(philo) > philo->time_to_eat)
	{
		block_thread(philo->time_to_eat);
	}
	else
		block_thread(remaining_time(philo));
	/* philo->starting_time = get_time(); */
	philo->eating_times--;
	philo->next = sleeping;
}

void sleeping(t_philo *philo)
{
	// NOTE: remaining_life_time can't be negative because it gets updated in in eating()
	if (remaining_time(philo) > philo->time_to_sleep)
	{
		put_state(philo, SLEEPING);
		block_thread(philo->time_to_sleep);
		philo->next = thinking;
	}
	else
	{
		put_state(philo, SLEEPING);
		block_thread(remaining_time(philo));
		philo->next = dead;
	}
}

void thinking(t_philo *philo)
{
	put_state(philo, THINKING);
	block_thread(remaining_time(philo));
	if (philo->eating_times)
	{
		philo->next = eating;
		philo->starting_time = get_time();
	}
	else
		philo->next = dead;
}

void dead(t_philo *philo)
{
	put_state(philo, DEAD);
	philo->next = NULL;
}

void init(t_philo *philo)
{
	philo->first_starting_time = get_time();
	philo->starting_time = philo->first_starting_time;
	// NOTE: what to do if eating time is 0, should the philo start thinking until time is out?
	while(philo->next)
	{
		philo->next(philo);
	}
}

int main(void) {
	t_philo philo = {
		.id = 1,
		.time_to_eat = 300,
		.time_to_sleep = 300,
		.life_time = 3300,
		.eating_times = 3,
		.starting_time = 0,
		.next = eating,
	};
	init(&philo);
}
