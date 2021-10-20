#include "philo.h"
#include "time.h"
#include "utils.h"
#include <pthread.h>
#include <stdbool.h>

void set_state(t_philo *philo, t_state_fn next_state)
{
	pthread_mutex_lock(&philo->protect_state);
	philo->next = next_state;
	pthread_mutex_unlock(&philo->protect_state);
}

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
	set_state(philo, sleeping);
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
	set_state(philo, thinking);
}

void	thinking(t_philo *philo)
{
	if (philo->eating_times.n == 0)
	{
		set_state(philo, NULL);
		return ;
	}
	put_state(philo, THINKING);
	pick_forks(philo);
	pthread_mutex_lock(&philo->protect_state);
	philo->starting_time = get_time();
	pthread_mutex_unlock(&philo->protect_state);
	set_state(philo, eating);
}

void	dead(t_philo *philo)
{
	set_state(philo, NULL);
	pthread_mutex_lock(&philo->shared->protect);
	philo->shared->stop = true;
	pthread_mutex_unlock(&philo->shared->protect);
	put_state(philo, DEAD);
}
