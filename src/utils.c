#include "utils.h"

size_t ft_strlen(char *s)
{
	size_t i;
	i = 0;
	while(s[i])
	{
		i++;
	}
	return (i);
}

void panic(char *err_msg)
{
	write(0, err_msg, ft_strlen(err_msg)); // NOTE: replace strlen later
	write(0, "\n", 1);
	exit(EXIT_FAILURE);
}

void put_state(const t_philo *philo, char *state)
{
	pthread_mutex_lock(philo->display_mutex);
	printf("%lu\t\t %d %s\n", time_elapsed(philo->first_starting_time), philo->id, state);
	if (philo->next == NULL)
		exit(0);
	pthread_mutex_unlock(philo->display_mutex);
}
