#include "time.h"

void block_thread(size_t ms)
{
	const size_t micro_s = ms * 1000;
	usleep(micro_s);
}

size_t get_time()
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

size_t time_elapsed(size_t starting_time)
{
	return (get_time() - starting_time);
}

/*
** NOTE: Call it only when you are 100% sure that life_time >= time_elapsed
*/

size_t remaining_time(const t_philo *philo)
{
	return (philo->life_time - time_elapsed(philo->starting_time));
}
