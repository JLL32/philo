#include "philo.h"
#include "time.h"
#include "utils.h"
#include <pthread.h>

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
	if (philo->id % 2 == 0 && philo->shared->number_of_philos != 1)
		block_thread(philo->time_to_eat);
	if (philo->shared->number_of_philos == 1)
	{
		put_state(philo, HAS_FORK);
		block_thread(philo->life_time);
		set_state(philo, dead);
	}
	else
		pick_forks(philo);
	philo->starting_time = get_time();
	while (philo->next)
	{
		philo->next(philo);
	}
	return (philo);
}

void	eating(t_philo *philo)
{
	put_state(philo, EATING);
	if (philo->next == NULL)
		return ;
	block_thread(philo->time_to_eat);
	put_forks(philo);
	pthread_mutex_lock(&philo->protect_state);
	philo->next = sleeping;
	pthread_mutex_unlock(&philo->protect_state);
	if (philo->eating_times.always == false)
		philo->eating_times.n--;
}

void	sleeping(t_philo *philo)
{
	put_state(philo, SLEEPING);
	if (philo->next == NULL)
		return ;
	block_thread(philo->time_to_sleep);
	pthread_mutex_lock(&philo->protect_state);
	set_state(philo, thinking);
	pthread_mutex_unlock(&philo->protect_state);
}

void	thinking(t_philo *philo)
{
	if (philo->eating_times.n == 0)
	{
		pthread_mutex_lock(&philo->protect_state);
		set_state(philo, NULL);
		pthread_mutex_unlock(&philo->protect_state);
		return ;
	}
	put_state(philo, THINKING);
	if (philo->next == NULL)
		return ;
	pick_forks(philo);
	pthread_mutex_lock(&philo->protect_state);
	set_state(philo, eating);
	philo->starting_time = get_time();
	pthread_mutex_unlock(&philo->protect_state);
}

void	dead(t_philo *philo)
{
	set_state(philo, NULL);
	put_state(philo, DEAD);
}
