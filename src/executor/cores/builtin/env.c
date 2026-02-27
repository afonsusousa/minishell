/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:31:52 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 19:00:22 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/executor.h"
#include "../../../../includes/minishell.h"
#include "../../../../includes/utils.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

static int	exec_env_with_args(const t_minishell *sh, char **argv)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		execve_wrapper((t_minishell *) sh, &argv);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (127);
}

int	exec_env(const t_minishell *sh, char **argv, const int argc)
{
	char	**env;
	char	**iter;

	if (argc > 1)
		return (exec_env_with_args(sh, argv + 1));
	env = get_envp_array(sh->env, true);
	iter = env;
	while (iter && *iter)
		printf("%s\n", *iter++);
	free_until_null(&env);
	return (0);
}
