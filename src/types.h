#ifndef TYPES_H
# define TYPES_H
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/_types/_size_t.h>
# include <time.h>
# include <unistd.h>
# include <limits.h>
# include <stdbool.h>
# define HAS_FORK "has taken a fork"
# define EATING "is eating"
# define SLEEPING "is sleeping"
# define THINKING "is thinking"
# define DEAD "died"

typedef struct s_philo	t_philo;

typedef void			t_state_fn(t_philo *philo);

typedef struct s_eating_times
{
	size_t	n;
	bool	always;
}	t_eating_times;

typedef struct s_shared
{
	size_t			number_of_philos;
	pthread_mutex_t	*forks_list;
	t_philo			*philo_list;
	pthread_mutex_t	display_mutex;
	size_t			starting_time;
	bool			stop;
	size_t			total_meals;
}	t_shared;

typedef struct s_philo
{
	const int		id;
	pthread_t		thread_id;
	const size_t	time_to_eat;
	const size_t	time_to_sleep;
	const size_t	life_time;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	size_t			starting_time;
	t_eating_times	eating_times;
	t_state_fn		*next;
	t_shared		*shared;
}	t_philo;

typedef struct s_data
{
	const size_t			n_philo;
	const size_t			life_time;
	const size_t			time_to_eat;
	const size_t			time_to_sleep;
	const t_eating_times	eating_times;
}	t_data;
#endif
