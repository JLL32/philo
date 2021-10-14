#include "philo.h"

void *init(void *p)
{
	t_philo *philo = p;
	if (philo->id % 2 == 0 && number_of_philos != 1)
		block_thread(philo->time_to_eat);
	pick_forks(philo);
	philo->starting_time = get_time();
	while(philo->next)
	{
		philo->next(philo);
	}
	return philo;
}

void eating(t_philo *philo)
{
	if (philo->eating_times == 0)
	{
		philo->next = dead;
		return ;
	}
	if (time_elapsed(philo->starting_time) >= philo->life_time)
	{
		philo->next = dead;
		return;
	}
	put_state(philo, EATING);
	if (remaining_time(philo) > philo->time_to_eat)
	{
		block_thread(philo->time_to_eat);
	}
	else
		block_thread(remaining_time(philo));
	put_forks(philo);
	philo->next = sleeping;
	philo->eating_times--;
}

void sleeping(t_philo *philo)
{
	// NOTE: remaining_life_time can't be negative because it gets updated in in eating()
	if (remaining_time(philo) > philo->time_to_sleep)
	{
		put_state(philo, SLEEPING);
		block_thread(philo->time_to_sleep);
		philo->next = thinking;
	}
	else
	{
		put_state(philo, SLEEPING);
		block_thread(remaining_time(philo));
		philo->next = dead;
	}
}

void thinking(t_philo *philo)
{
	if (philo->eating_times)
	{
		put_state(philo, THINKING);
		pick_forks(philo);
		philo->next = eating;
		philo->starting_time = get_time();
	}
	else
		philo->next = dead;
}

void dead(t_philo *philo)
{
	philo->next = NULL;
	put_state(philo, DEAD);
}

