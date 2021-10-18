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
