#include "philo.h"
#include "time.h"
#include "parse_arg.h"

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

t_philo *create_philos(t_data data)
{
	t_philo *philos;
	size_t i;

	philos = malloc(data.n_philo * sizeof(*philos));
	i = 0;
	while(i < data.n_philo)
	{
		memcpy(&philos[i], &(t_philo){
			.id = i + 1,
			.thread_id = 0,
			.time_to_eat = data.time_to_eat,
			.time_to_sleep = data.time_to_sleep,
			.life_time = data.life_time,
			.eating_times = data.eating_times,
			.starting_time = 0,
			.first_starting_time = 0,
			.next = eating,
			.display_mutex = NULL,
			.l_fork = NULL,
			.r_fork = NULL,
		}, sizeof(t_philo)); // NOTE: replace memcpy later
		i++;
	}
	return philos;
}

void start_simulation(t_data data, int *err)
{
	pthread_mutex_t display;
	size_t i;
	const size_t starting_time = get_time();
	t_philo *philo_list = create_philos(data);
	pthread_mutex_t *forks = create_forks(data.n_philo, err);

	pthread_mutex_init(&display, NULL);
	i = 0;
	while(i < data.n_philo)
	{
		philo_list[i].starting_time = starting_time;
		philo_list[i].first_starting_time = starting_time;
		philo_list[i].display_mutex = &display;
		philo_list[i].l_fork = &forks[i];
		philo_list[i].r_fork = &forks[(i + 1) % data.n_philo];
		*err = pthread_create(&(philo_list[i].thread_id), NULL, init, &philo_list[i]);
		if (*err)
			return ;
		i++;
	}
	i = 0;
	while (i < data.n_philo)
	{
		pthread_join(philo_list[i].thread_id, NULL);
		i++;
	}
}

int main(int argc, char **argv) {

	static int err;
	const t_data data = get_data(argc, argv, &err);
	if (err)
		panic("Invalid arguments 😱");
	number_of_philos = data.n_philo;
	start_simulation(data, &err);
	if (err)
		panic("Something went wrong with the threads 😱");
	// freee philos and the data inside
	// free pthread_t
}
