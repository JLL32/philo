#ifndef UTILS_H
# include "time.h"
# include "types.h"
# define UTILS_H

size_t ft_strlen(char *s);
void panic(char *err_msg);
void put_state(t_philo *philo, char *state);
void free_all(t_philo *philo);
void destroy_forks(pthread_mutex_t *forks, size_t size);
#endif
