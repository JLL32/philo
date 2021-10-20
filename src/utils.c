#include "utils.h"
#include "philo.h"
#include "types.h"
#include "string_utils.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_pthread/_pthread_mutex_t.h>

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

void	free_all(t_philo *philo_list, pthread_mutex_t *forks_list)
{
	
	size_t	i;
	size_t size;

	size = philo_list[0].shared->number_of_philos;
	i = 0;
	while (i < size)
	{
		pthread_mutex_destroy(&philo_list[i].protect_state);
		i++;
	}
	pthread_mutex_destroy(&philo_list[0].shared->display_mutex);
	pthread_mutex_destroy(&philo_list[0].shared->protect);
	destroy_forks(forks_list, philo_list[0].shared->number_of_philos);
	free(philo_list[0].shared);
	free(philo_list);
	free(forks_list);
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
