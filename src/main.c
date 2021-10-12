#include "../include/philo.h"
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_size_t.h>
#include <sys/_types/_timeval.h>
#include <unistd.h>
#include <sys/time.h>

#define HAS_FORK "has taken a fork"
#define EATING "is eating"
#define SLEEPING "is sleeping"
#define THINKING "is thinking"
#define DEAD "died"

#include "parse_arg.h"


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
	pthread_mutex_lock(philo->display_mutex);
	printf("%lu\t\t %d %s\n", time_elapsed(philo->first_starting_time), philo->id, state);
	if (philo->next == NULL)
		exit(0);
	pthread_mutex_unlock(philo->display_mutex);
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
	philo->next = sleeping;
	philo->eating_times--;
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
	/* block_thread(remaining_time(philo)); */
	if (philo->eating_times)
	{
		// unlock a fork here
		put_state(philo, THINKING);
		philo->next = eating;
		philo->starting_time = get_time();
	}
	else
		philo->next = dead;
}

void dead(t_philo *philo)
{
	philo->next = NULL;
	put_state(philo, DEAD);
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

t_data get_data(int argc, char** argv, int *err)
{
	size_t n_philo;
	size_t life_time;
	size_t time_to_eat;
	size_t time_to_sleep;
	size_t eating_times;

	if (argc < 5 || argc > 6)
	{
		*err = 1;
		return (t_data){};
	}
	n_philo = parse_arg_size_t(argv[1], err);
	life_time = parse_arg_size_t(argv[2], err);
	time_to_eat = parse_arg_size_t(argv[3], err);
	time_to_sleep = parse_arg_size_t(argv[4], err);
	if (argc == 5)
		eating_times = 1;
	else
		eating_times = parse_arg_size_t(argv[5], err);
	if (*err)
		return (t_data){};
	return ((t_data) {n_philo, life_time, time_to_eat, time_to_sleep, eating_times});
}

void panic(char *err_msg)
{
	write(0, err_msg, strlen(err_msg));
	exit(EXIT_FAILURE);
}

t_philo **create_philos(t_data data)
{
	t_philo **philos;
	size_t i;

	philos = malloc(data.n_philo * sizeof(t_philo *));
	i = 0;
	while(i < data.n_philo)
	{
		philos[i] = malloc(sizeof(t_philo));
		memcpy(philos[i], &(t_philo){
			.id = i + 1,
			.time_to_eat = data.time_to_eat,
			.time_to_sleep = data.time_to_sleep,
			.life_time = data.life_time,
			.eating_times = data.eating_times,
			.starting_time = 0,
			.first_starting_time = 0,
			.next = eating,
			.display_mutex = NULL,
		}, sizeof(t_philo)); // NOTE: replace memcpy later
		i++;
	}
	return philos;
}

int main(int argc, char **argv) {

	pthread_mutex_t display;
	static int err;
	const t_data data = get_data(argc, argv, &err);
	const size_t starting_time = get_time();
	if (err)
		panic("Invalid arguments 😱");
	t_philo **philos = create_philos(data);
	/* init_philos(create_philos(data)); */
	pthread_mutex_init(&display, NULL);
	pthread_t thread[data.n_philo];
	size_t i = 0;
	while(i < data.n_philo)
	{
		philos[i]->starting_time = starting_time;
		philos[i]->first_starting_time = starting_time;
		philos[i]->display_mutex = &display;
		pthread_create(&thread[i], NULL, init, philos[i]);
		i++;
	}

	i = 0;
	while (i < data.n_philo)
	{
		pthread_join(thread[i], NULL);
		i++;
	}
	// freee philos and the data inside
	// free pthread_t
}
