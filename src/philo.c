#include "philo.h"
#include "time.h"
#include "utils.h"
#include <pthread.h>
#include <stdbool.h>

void	*init(void *p)
{
	t_philo	*philo;

	philo = p;
	if (philo->id % 2 == 0)
		block_thread(philo->time_to_eat);
	pick_forks(philo);
	pthread_mutex_lock(&philo->protect_state);
	philo->starting_time = get_time();
	pthread_mutex_unlock(&philo->protect_state);
	while (philo->next)
	{
		philo->next(philo);
	}
	return (philo);
}

void	eating(t_philo *philo)
{
	put_state(philo, EATING);
	block_thread(philo->time_to_eat);
	put_forks(philo);
	philo->next = sleeping;
	if (philo->eating_times.always == false)
	{
		philo->eating_times.n--;
		pthread_mutex_lock(&philo->shared->protect);
		philo->shared->total_meals--;
		pthread_mutex_unlock(&philo->shared->protect);
	}
}

void	sleeping(t_philo *philo)
{
	put_state(philo, SLEEPING);
	block_thread(philo->time_to_sleep);
	philo->next = thinking;
}

void	thinking(t_philo *philo)
{
	if (philo->eating_times.n == 0)
	{
		philo->next = NULL;
		return ;
	}
	put_state(philo, THINKING);
	pick_forks(philo);
	pthread_mutex_lock(&philo->protect_state);
	philo->starting_time = get_time();
	pthread_mutex_unlock(&philo->protect_state);
	philo->next = eating;
}
