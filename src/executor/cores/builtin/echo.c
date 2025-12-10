#include "../../../../includes/utils.h"
#include "../../../../lib/libft/libft.h"
#include <stdbool.h>
#include <stdio.h>

static bool	is_n_flag(const char *arg)
{
	size_t	j;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (false);
	j = 1;
	while (arg[j])
	{
		if (arg[j] != 'n')
			return (false);
		j++;
	}
	return (true);
}

int	exec_echo(char **argv, const int argc)
{
	size_t	i;
	bool	newline;

	i = 0;
	newline = true;
	while (++i < (size_t)argc && is_n_flag(argv[i]))
		newline = false;
	while (i < (size_t)argc)
	{
		printf("%s", argv[i]);
		if (i != (size_t)argc - 1)
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
