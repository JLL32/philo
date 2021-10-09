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
	int id;
	size_t time_to_eat;
	size_t time_to_sleep;
	size_t time_to_die;
	size_t eating_times;
	state_fn *next;
} t_philo;

state_fn eating, sleeping, thinking, dead;
