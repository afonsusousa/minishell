/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:33:08 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:33:11 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/executor.h"
#include "../../../includes/minishell.h"
#include "../../../includes/sig.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

bool	is_core_builtin(t_minishell *sh, const t_ast *core)
{
	char	**argv;
	int		argc;
	bool	ret;

	if (!core || core->type != AST_COMMAND || !core->u_as.s_command.argv
		|| !core->u_as.s_command.argv[0])
		return (false);
	argc = 0;
	argv = argv_to_arr(sh, core->u_as.s_command.argv, &argc);
	if (!argv || !*argv)
	{
		free_argv(argv);
		return (false);
	}
	ret = is_builtin(*argv);
	free_argv(argv);
	return (ret);
}

int	pipeline_fork_error(t_minishell *sh, int fd[2])
{
	size_t	i;

	if (fd[0] != -1)
		close(fd[0]);
	if (fd[1] != -1)
		close(fd[1]);
	if (sh->pipeline.prev_read != -1)
		close(sh->pipeline.prev_read);
	i = 0;
	while (i < sh->pipeline.count)
	{
		if (sh->pipeline.pids[i] > 0)
			kill(sh->pipeline.pids[i], SIGTERM);
		i++;
	}
	return (wait_pids(&sh->pipeline));
}

int	wait_pids(const t_pipeline *pipeline)
{
	size_t	i;
	int		status;
	int		st;

	i = 0;
	status = 0;
	while (i < pipeline->count)
	{
		signal(SIGINT, SIG_IGN);
		if (waitpid(pipeline->pids[i], &st, 0) > 0 && i == pipeline->count - 1)
		{
			if (WIFEXITED(st))
				status = WEXITSTATUS(st);
			else if (WIFSIGNALED(st))
				status = 128 + WTERMSIG(st);
			if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
				write(1, "\n", 1);
		}
		signal(SIGINT, sigint_handler);
		i++;
	}
	return (status);
}
