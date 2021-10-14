#ifndef PHILO_H
# define PHILO_H
# include "types.h"
# include "time.h"
# include "utils.h"
# include "forks.h"

void *init(void *p);
void eating(t_philo *philo);
void sleeping(t_philo *philo);
void thinking(t_philo *philo);
void dead(t_philo *philo);

#endif
