#include "../include/philo.h"
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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

void *monitor(void *arg)
{
	int n = 3;
	int i = 0;
	t_philo **philos = arg;
	while(i < n)
	{
		if (philos[i]->next == NULL)
		{
			put_state(philos[i], DEAD);
			exit(0);
		}
		else if (philos[i]->next == eating)
		{
			put_state(philos[i], EATING);
		}
		else if (philos[i]->next == sleeping)
		{
			put_state(philos[i], SLEEPING);
		}
		else if (philos[i]->next == thinking)
		{
			put_state(philos[i], THINKING);
		}
		if (i == 2)
			i = 0;
		i++;
	}
	return arg;
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
	/* put_state(philo, EATING); */
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
		/* put_state(philo, SLEEPING); */
		block_thread(philo->time_to_sleep);
		philo->next = thinking;
	}
	else
	{
		/* put_state(philo, SLEEPING); */
		block_thread(remaining_time(philo));
		philo->next = dead;
	}
}

void thinking(t_philo *philo)
{
	/* put_state(philo, THINKING); */
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
	/* put_state(philo, DEAD); */
	philo->next = NULL;
}

void *init(void *p)
{
	t_philo *philo = p;
	philo->starting_time = get_time();
	// NOTE: what to do if eating time is 0, should the philo start thinking until time is out?
	while(philo->next)
	{
		philo->next(philo);
	}
	return philo;
}

int main(void) {

	const size_t starting_time = get_time();

	t_philo philo = {
		.id = 1,
		.time_to_eat = 300,
		.time_to_sleep = 300,
		.life_time = 3000,
		.eating_times = 3,
		.starting_time = starting_time,
		.first_starting_time = starting_time,
		.next = eating,
	};

	t_philo philo2 = {
		.id = 2,
		.time_to_eat = 300,
		.time_to_sleep = 300,
		.life_time = 3000,
		.eating_times = 3,
		.starting_time = starting_time,
		.first_starting_time = starting_time,
		.next = eating,
	};

	t_philo philo3 = {
		.id = 3,
		.time_to_eat = 300,
		.time_to_sleep = 300,
		.life_time = 3000,
		.eating_times = 3,
		.starting_time = starting_time,
		.first_starting_time = starting_time,
		.next = eating,
	};

	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_t thread_monitor;
	pthread_create(&thread_monitor, NULL, monitor, (t_philo*[]){&philo, &philo2, &philo3});
	pthread_create(&thread1, NULL, init, &philo);
	pthread_create(&thread2, NULL, init, &philo2);
	pthread_create(&thread3, NULL, init, &philo3);
	pthread_join(thread_monitor, NULL);
}
