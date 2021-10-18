#include "forks.h"
#include "parse_arg.h"
#include "philo.h"
#include "string_utils.h"
#include "time.h"
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
			&(t_philo){
			.id = i + 1, .thread_id = 0,
			.time_to_eat = data.time_to_eat,
			.time_to_sleep = data.time_to_sleep,
			.life_time = data.life_time, .eating_times = data.eating_times,
			.starting_time = 0, .first_starting_time = 0,
			.next = eating, .l_fork = NULL,
			.r_fork = NULL, .env.display_mutex = NULL,
			.env.philo_list = NULL, .env.forks_list = NULL,
			.env.number_of_philos = 0, },
			sizeof(t_philo));
		i++;
	}
	return (philos);
}

void	await(t_philo *philo_list, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		pthread_join(philo_list[i].thread_id, NULL);
		i++;
	}
}

void	start_simulation(t_data data,
	t_philo *philo_list,
	pthread_mutex_t *forks_list,
	int *err)
{
	pthread_mutex_t	display;
	bool			stop;
	size_t			i;
	const size_t	starting_time = get_time();

	stop = false;
	pthread_mutex_init(&display, NULL);
	i = 0;
	while (i < data.n_philo)
	{
		philo_list[i].starting_time = starting_time;
		philo_list[i].first_starting_time = starting_time;
		philo_list[i].l_fork = &forks_list[i];
		philo_list[i].r_fork = &forks_list[(i + 1) % data.n_philo];
		philo_list[i].env.number_of_philos = data.n_philo;
		philo_list[i].env.philo_list = philo_list;
		philo_list[i].env.display_mutex = &display;
		philo_list[i].env.forks_list = forks_list;
		philo_list[i].env.stop = &stop;
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
	start_simulation(data, philo_list, forks_list, &err);
	if (err)
		panic("Something went wrong with the threads 😱");
}
