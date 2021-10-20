#include "string_utils.h"

void	ft_memcpy(void *dest, void *src, size_t n)
{
	char	*csrc;
	char	*cdest;
	size_t	i;

	cdest = (char *)dest;
	csrc = (char *)src;
	i = 0;
	while (i < n)
	{
		cdest[i] = csrc[i];
		i++;
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1)
	{
		if (*s1 != *s2)
		{
			break ;
		}
		s1++;
		s2++;
	}
	return ((*(const unsigned char *)s1 - *(const unsigned char *)s2));
}
