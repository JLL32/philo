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
		philo->eating_times.n--;
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
	set_state(philo, eating);
	pthread_mutex_lock(&philo->protect_state);
	philo->starting_time = get_time();
	pthread_mutex_unlock(&philo->protect_state);
}

void	dead(t_philo *philo)
{
	set_state(philo, NULL);
	philo->shared->stop = true;
	put_state(philo, DEAD);
}
