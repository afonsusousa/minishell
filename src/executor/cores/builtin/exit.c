/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:31:57 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:38:10 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/executor.h"
#include "../../../../includes/minishell.h"
#include "../../../../lib/libft/libft.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

static bool	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (false);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

static void	exit_error(t_minishell *sh, const char *arg)
{
	write(2, "minishell: exit: ", 17);
	write(2, arg, ft_strlen(arg));
	write(2, ": numeric argument required\n", 28);
	minishell_free(sh);
	exit(2);
}

int	exec_exit(t_minishell *sh, char **argv, const int argc)
{
	int	code;

	code = 0;
	if (argc == 2)
	{
		if (!is_numeric(argv[1]))
			exit_error(sh, argv[1]);
		code = ft_atoi(argv[1]) % 256;
		if (code < 0)
			code += 256;
	}
	else if (argc > 2)
	{
		if (!is_numeric(argv[1]))
			exit_error(sh, argv[1]);
		return ((write(2, "minishell: exit: too many arguments\n",
					36) & 0) | 2);
	}
	free_argv(argv);
	restore_fds(sh);
	minishell_free(sh);
	exit(code);
}
