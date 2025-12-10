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

int	exec_unset(const t_minishell *sh, char **argv, const int argc)
{
	(void)argc;
	if (argv[1] == NULL)
		return (0);
	while (*++argv != NULL)
		envp_unsetvar(sh->env, *argv);
	return (0);
}
