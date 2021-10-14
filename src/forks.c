#include "forks.h"

void pick_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	put_state(philo, HAS_FORK);
	pthread_mutex_lock(philo->r_fork);
	put_state(philo, HAS_FORK);
}

void put_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

pthread_mutex_t *create_forks(size_t n, int *err)
{
	pthread_mutex_t *forks;
	size_t i;
	
	forks = malloc(sizeof(*forks) * n);
	i = 0;
	while(i < n)
	{
		*err = pthread_mutex_init(&forks[i], NULL);
		if (*err)
			return NULL;
		i++;
	}
	return (forks);
}
