#include "forks.h"
#include "time.h"

int pick_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	if (remaining_time(philo) == 0)
	{
		return (1);
	}
	put_state(philo, HAS_FORK);
	pthread_mutex_lock(philo->r_fork);
	if (remaining_time(philo) == 0)
	{
		return (1);
	}
	put_state(philo, HAS_FORK);
	return (0);
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
