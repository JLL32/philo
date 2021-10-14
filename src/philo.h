#ifndef PHILO_H
# define PHILO_H
# include "types.h"
# include "time.h"
# include "utils.h"
# include "forks.h"

# define HAS_FORK "has taken a fork"
# define EATING "is eating"
# define SLEEPING "is sleeping"
# define THINKING "is thinking"
# define DEAD "died"

/* state_fn eating, sleeping, thinking, dead; */

void *init(void *p);
void eating(t_philo *philo);
void sleeping(t_philo *philo);
void thinking(t_philo *philo);
void dead(t_philo *philo);

#endif
