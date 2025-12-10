//
// Created by afonsusousa on 11/24/25.
//

#include "../../../includes/executor.h"
#include "../../../includes/minishell.h"
#include "../../../includes/sig.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

bool	is_core_builtin(const t_ast *core)
{
	if (!core)
		return (false);
	if (core->type == AST_COMMAND && !core->as.command.argv)
		return (true);
	if (core->type == AST_COMMAND && core->as.command.argv[0])
		return (is_builtin(core->as.command.argv[0]));
	return (false);
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
