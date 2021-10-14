#ifndef FORKS_h
#include "types.h"
#include "utils.h"
# define FORKS_h

void pick_forks(t_philo *philo);
void put_forks(t_philo *philo);
pthread_mutex_t *create_forks(size_t n, int *err);

#endif
