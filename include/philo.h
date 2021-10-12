#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_size_t.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

typedef struct s_philo t_philo;

typedef void state_fn(t_philo *philo);

typedef struct s_philo
{
	const int id;
	pthread_t thread_id;
	const size_t time_to_eat;
	const size_t time_to_sleep;
	const size_t life_time;
	pthread_mutex_t *l_fork;
	pthread_mutex_t *r_fork;
	pthread_mutex_t *display_mutex;
	size_t first_starting_time;
	size_t starting_time;
	size_t eating_times;
	state_fn *next;
} t_philo;

typedef struct s_data
{
	const size_t n_philo;
	const size_t life_time;
	const size_t time_to_eat;
	const size_t time_to_sleep;
	const size_t eating_times;
} t_data;

state_fn eating, sleeping, thinking, dead;
