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

static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
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
		if (!ft_isdigit(arg[i]))
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

int	parse_arg_size_t(char *arg, int *err)
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
