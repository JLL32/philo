#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_size_t.h>
#include <time.h>
#include <unistd.h>

typedef struct s_philo t_philo;

typedef void state_fn(t_philo *philo);

typedef struct s_philo
{
	const int id;
	const size_t time_to_eat;
	const size_t time_to_sleep;
	const size_t life_time;
	size_t remaining_life_time;
	size_t eating_times;
	size_t starting_time;
	state_fn *next;
} t_philo;

state_fn eating, sleeping, thinking, dead;
