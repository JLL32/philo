#ifndef UTILS_H
# include "time.h"
# include "types.h"
# define UTILS_H
size_t ft_strlen(char *s);
void panic(char *err_msg);
void put_state(const t_philo *philo, char *state);
#endif
