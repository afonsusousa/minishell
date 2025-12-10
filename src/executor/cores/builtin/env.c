/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:31:52 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:31:55 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/executor.h"
#include "../../../../includes/minishell.h"
#include "../../../../includes/utils.h"
#include <stdio.h>

int	exec_env(const t_minishell *sh, char **argv, const int argc)
{
	char	**env;
	char	**iter;

	(void)argv;
	(void)argc;
	// review export later
	env = get_envp_array(sh->env, true);
	iter = env;
	while (iter && *iter)
		printf("%s\n", *iter++);
	free_until_null(&env);
	return (0);
}
