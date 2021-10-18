#include "philo.h"
#include "time.h"
#include "utils.h"

void *init(void *p)
{
	t_philo *philo;

	philo = p;
	if (philo->id % 2 == 0 && philo->env.number_of_philos != 1)
		block_thread(philo->time_to_eat);
	if (philo->env.number_of_philos == 1)
	{
		put_state(philo, HAS_FORK);
		block_thread(philo->life_time);
		philo->next = dead;
	}
	else
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
	if (philo->eating_times.n == 0)
	{
		philo->next = NULL;
		return ;
	}
	if (time_elapsed(philo->starting_time) >= philo->life_time)
	{
		philo->next = dead;
		return;
	}
	put_state(philo, EATING);
	if (philo->next == NULL)
		return;
	if (remaining_time(philo) > philo->time_to_eat)
	{
		block_thread(philo->time_to_eat);
	}
	else
		block_thread(remaining_time(philo));
	put_forks(philo);
	philo->next = sleeping;
	if(philo->eating_times.always == false)
		philo->eating_times.n--;
}

void sleeping(t_philo *philo)
{
	// NOTE: remaining_life_time can't be negative because it gets updated in in eating()
	put_state(philo, SLEEPING);
	if (philo->next != NULL)
	{
		if (remaining_time(philo) > philo->time_to_sleep)
		{
			block_thread(philo->time_to_sleep);
			philo->next = thinking;
		}
		else
		{
			block_thread(remaining_time(philo));
			philo->next = dead;
		}
	}
}

void thinking(t_philo *philo)
{
	if (philo->eating_times.n)
	{
		put_state(philo, THINKING);
		if (philo->next != NULL)
		{
			if (pick_forks(philo))
			{
				philo->next = dead;
				return;
			}
			philo->next = eating;
			philo->starting_time = get_time();
		}
	}
	else
		philo->next = NULL;
}

void dead(t_philo *philo)
{
	philo->next = NULL;
	put_state(philo, DEAD);
}

