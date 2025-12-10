/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:32:08 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:32:10 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/executor.h"
#include "../../../../includes/minishell.h"
#include "builtins.h"
#include "libft.h"
#include <unistd.h>

static int	unset_invalid_option(const char *arg)
{
	write(2, "minishell: unset: -", 19);
	write(2, arg + 1, 1);
	write(2, ": invalid option\n", 17);
	write(2, "unset: usage: unset [name ...]\n", 32);
	return (2);
}

static int	unset_invalid_identifier(const char *arg)
{
	write(2, "minishell: unset: `", 19);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 26);
	return (1);
}

int	exec_unset(const t_minishell *sh, char **argv, const int argc)
{
	int	status;

	(void)argc;
	status = 0;
	if (argv[1] == NULL)
		return (0);
	while (*++argv != NULL)
	{
		if (**argv == '-')
			return (unset_invalid_option(*argv));
		if (!is_valid_var_name(*argv))
		{
			status = unset_invalid_identifier(*argv);
			continue ;
		}
		envp_unsetvar(sh->env, *argv);
	}
	return (status);
}
