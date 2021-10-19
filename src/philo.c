#include "philo.h"
#include "time.h"
#include "utils.h"

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
		philo->next = dead;
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
	philo->next = sleeping;
	if (philo->eating_times.always == false)
		philo->eating_times.n--;
}

void	sleeping(t_philo *philo)
{
	put_state(philo, SLEEPING);
	if (philo->next == NULL)
		return ;
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
	if (philo->next == NULL)
		return ;
	pick_forks(philo);
	philo->next = eating;
	philo->starting_time = get_time();
}

void	dead(t_philo *philo)
{
	philo->next = NULL;
	put_state(philo, DEAD);
}
