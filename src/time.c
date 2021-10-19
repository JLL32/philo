#include "time.h"
#include "types.h"
#include <stddef.h>
#include <unistd.h>

void	block_thread(size_t ms)
{
	const size_t	micro_s = ms * 1000;

	usleep(micro_s - 2e3);
}

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

size_t	time_elapsed(size_t starting_time)
{
	return (get_time() - starting_time);
}

size_t	remaining_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->protect_state);
	const size_t	elapsed = time_elapsed(philo->starting_time);

	if (philo->life_time > elapsed)
	{
		pthread_mutex_unlock(&philo->protect_state);
		return (philo->life_time - elapsed);
	}
	else
	{
		pthread_mutex_unlock(&philo->protect_state);
		return (0);
	}
}
