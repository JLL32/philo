#ifndef TIME_H
# define TIME_H
# include <sys/time.h>
# include "types.h"

void	block_thread(size_t ms);
size_t	get_time(void);
size_t	time_elapsed(size_t starting_time);
size_t	remaining_time(const t_philo *philo);
#endif
