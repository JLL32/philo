#include "utils.h"
#include "philo.h"
#include "types.h"
#include "string_utils.h"
#include <stdlib.h>
#include <string.h>

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}

void	panic(char *err_msg)
{
	write(0, err_msg, ft_strlen(err_msg));
	write(0, "\n", 1);
	exit(EXIT_FAILURE);
}

void	destroy_forks(pthread_mutex_t *forks, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

void	free_all(t_philo *philo)
{
	pthread_mutex_destroy(&philo->shared->display_mutex);
	destroy_forks(philo->shared->forks_list, philo->shared->number_of_philos);
	free(philo->shared->forks_list);
	free(philo->shared->philo_list);
}

void	put_state(t_philo *philo, char *state)
{
	pthread_mutex_lock(&philo->shared->display_mutex);
	printf("%lu\t\t %d %s\n",
		   time_elapsed(philo->shared->starting_time),
		   philo->id,
		   state);
	if (ft_strcmp(state, DEAD) != 0)
		pthread_mutex_unlock(&philo->shared->display_mutex);
}
