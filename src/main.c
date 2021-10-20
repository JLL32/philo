#include "forks.h"
#include "parse_arg.h"
#include "philo.h"
#include "string_utils.h"
#include "time.h"
#include "types.h"
#include "utils.h"
#include <malloc/_malloc.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <sys/_pthread/_pthread_mutex_t.h>
#include <sys/_types/_size_t.h>

t_data	get_data(int argc, char **argv, int *err)
{
	size_t			n_philo;
	size_t			life_time;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	t_eating_times	eating_times;

	if (argc < 5 || argc > 6)
	{
		*err = 1;
		return ((t_data){});
	}
	n_philo = parse_arg_size_t(argv[1], err);
	life_time = parse_arg_size_t(argv[2], err);
	time_to_eat = parse_arg_size_t(argv[3], err);
	time_to_sleep = parse_arg_size_t(argv[4], err);
	if (argc == 5)
	{
		eating_times = (t_eating_times){1, true};
	}
	else
		eating_times = (t_eating_times){parse_arg_size_t(argv[5], err), false};
	if (*err)
		return ((t_data){});
	return (
		(t_data){n_philo, life_time, time_to_eat, time_to_sleep, eating_times});
}

t_philo	*create_philos(t_data data)
{
	t_philo	*philos;
	size_t	i;

	philos = malloc(data.n_philo * sizeof(*philos));
	i = 0;
	while (i < data.n_philo)
	{
		ft_memcpy(&philos[i],
				  &(t_philo){.id = i + 1,
							 .thread_id = 0,
							 .time_to_eat = data.time_to_eat,
							 .time_to_sleep = data.time_to_sleep,
							 .life_time = data.life_time,
							 .eating_times = data.eating_times,
							 .starting_time = get_time(),
							 .next = eating,
							 .l_fork = NULL,
							 .shared = NULL},
				  sizeof(t_philo));
		i++;
	}
	return (philos);
}

void	await(t_philo *philo_list, size_t size)
{
	size_t	i;

	i = 0;
	
	pthread_mutex_lock(&philo_list[i].shared->protect);
	while (philo_list[i].shared->total_meals)
	{
		pthread_mutex_unlock(&philo_list[i].shared->protect);
		if (remaining_time(&philo_list[i]) == 0)
		{
			put_state(&philo_list[i], DEAD);
			return ;
		}
		if (i == size - 1)
			i = 0;
		else
			i++;
		pthread_mutex_lock(&philo_list[i].shared->protect);
	}
}

t_shared *create_shared(t_data data,
	t_philo *philo_list,
	pthread_mutex_t *forks_list)
{
	t_shared		*shared;
	pthread_mutex_t	display;
	pthread_mutex_t protect;

	shared = malloc(sizeof(*shared));
	pthread_mutex_init(&display, NULL);
	pthread_mutex_init(&protect, NULL);
	*shared = (t_shared){.number_of_philos = data.n_philo,
						 .forks_list = forks_list,
						 .philo_list = philo_list,
						 .display_mutex = display,
						 .starting_time = get_time(),
						 .stop = false,
						 .protect = protect,
						 .total_meals = data.n_philo * data.eating_times.n};
	return (shared);
}

void	start_simulation(t_data data,
	t_philo *philo_list,
	pthread_mutex_t *forks_list)
{
	size_t			i;
	t_shared		*shared;

	shared = create_shared(data, philo_list, forks_list);
	i = 0;
	while (i < data.n_philo)
	{
		philo_list[i].l_fork = &forks_list[i];
		philo_list[i].r_fork = &forks_list[(i + 1) % data.n_philo];
		philo_list[i].shared = shared;
		pthread_mutex_init(&philo_list[i].protect_state, NULL);
		pthread_create(&(philo_list[i].thread_id), NULL, init, &philo_list[i]);
		i++;
	}
	await(philo_list, data.n_philo);
	free_all(&philo_list[0]);
}

int	main(int argc, char **argv)
{
	static int		err;
	t_philo			*philo_list;
	pthread_mutex_t	*forks_list;
	const t_data	data = get_data(argc, argv, &err);

	if (err)
		panic("Invalid arguments 😱");
	philo_list = create_philos(data);
	forks_list = create_forks(data.n_philo, &err);
	if (err)
	{
		destroy_forks(forks_list, data.n_philo);
		free(forks_list);
		free(philo_list);
		panic("Something went wrong with the forks 😱");
	}
	start_simulation(data, philo_list, forks_list);
}
