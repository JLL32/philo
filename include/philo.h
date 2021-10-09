#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_philo t_philo;

typedef void state_fn(t_philo *philo);

typedef struct s_philo
{
	int i;
	state_fn *next;
} t_philo;

state_fn eating, sleeping, thinking, dead;
