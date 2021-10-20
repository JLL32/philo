#include "parse_arg.h"

static int	chk_arg_length(char *arg)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	if (arg[0] == '+' || arg[0] == '-')
	{
		i++;
		len--;
	}
	while (arg[i] == '0')
		i++;
	while (arg[i++])
		len++;
	return (len);
}

static int	is_numeric_positive(char *arg)
{
	int	i;

	if (!arg)
		return (0);
	i = 0;
	if (arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!(arg[i] >= '0' && arg[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

static int	check_arg(char *arg)
{
	int	len;

	len = chk_arg_length(arg);
	if (!is_numeric_positive(arg) || len > 11 || len < 0)
		return (0);
	else
		return (1);
}

static int	parse_arg_size_t(char *arg, int *err)
{
	int			i;
	size_t		value;

	if (!check_arg(arg))
	{
		*err = 1;
		return (0);
	}
	i = 0;
	value = 0;
	if (arg[i] == '+')
		i++;
	while (arg[i])
		value = value * 10 + (arg[i++] - '0');
	if (value > INT_MAX || value == 0)
	{
		*err = 1;
		return (0);
	}
	return (value);
}

t_data	get_data(int argc, char **argv, int *err)
{
	size_t			n_philo;
	size_t			life_time;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	t_eating_times	eating_times;

	if (argc < 5 || argc > 6)
	{
		*err = 1;
		return ((t_data){});
	}
	n_philo = parse_arg_size_t(argv[1], err);
	life_time = parse_arg_size_t(argv[2], err);
	time_to_eat = parse_arg_size_t(argv[3], err);
	time_to_sleep = parse_arg_size_t(argv[4], err);
	if (argc == 5)
	{
		eating_times = (t_eating_times){1, true};
	}
	else
		eating_times = (t_eating_times){parse_arg_size_t(argv[5], err), false};
	if (*err)
		return ((t_data){});
	return (
		(t_data){n_philo, life_time, time_to_eat, time_to_sleep, eating_times});
}
