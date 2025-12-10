/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:32:57 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:33:02 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/executor.h"
#include "../../../includes/minishell.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	exec_pipeline_child(t_minishell *sh, const t_ast_list *core,
		int fd[2])
{
	int	status;

	if (sh->pipeline.prev_read != -1)
	{
		if (dup2(sh->pipeline.prev_read, STDIN_FILENO) < 0)
			(perror("dup2"), exit(1));
		close(sh->pipeline.prev_read);
	}
	if (core->next)
	{
		if (dup2(fd[WRITE_END], STDOUT_FILENO) < 0)
			(perror("dup2"), exit(1));
		close(fd[READ_END]);
		close(fd[WRITE_END]);
	}
	sh->pipeline.prev_read = -1;
	sh->pipeline.count = 0;
	status = exec_core(sh, core->node, true);
	minishell_free(sh);
	exit(status);
}

static void	exec_pipeline_parent(t_minishell *sh, const t_ast_list *core,
		int fd[2], pid_t pid)
{
	sh->pipeline.pids[sh->pipeline.count++] = pid;
	if (sh->pipeline.prev_read != -1)
		close(sh->pipeline.prev_read);
	if (core->next)
	{
		close(fd[WRITE_END]);
		sh->pipeline.prev_read = fd[READ_END];
	}
	else
		sh->pipeline.prev_read = -1;
}

static int	exec_pipeline_core(t_minishell *sh, const t_ast_list *core)
{
	int		pipefd[2];
	pid_t	pid;

	pipefd[READ_END] = -1;
	pipefd[WRITE_END] = -1;
	if (core->next && pipe(pipefd) < 0)
	{
		if (sh->pipeline.prev_read != -1)
			close(sh->pipeline.prev_read);
		return (perror("pipe"), 1);
	}
	pid = fork();
	if (pid < 0)
	{
		if (core->next)
			(close(pipefd[READ_END]), close(pipefd[WRITE_END]));
		if (sh->pipeline.prev_read != -1)
			close(sh->pipeline.prev_read);
		return (perror("fork"), 1);
	}
	if (pid == 0)
		exec_pipeline_child(sh, core, pipefd);
	exec_pipeline_parent(sh, core, pipefd, pid);
	return (0);
}

int	exec_pipeline(t_minishell *sh, const t_ast_list *cores)
{
	const t_ast_list *curr;

	sh->pipeline.count = 0;
	sh->pipeline.prev_read = -1;
	ft_memset(sh->pipeline.pids, -1, 1024);
	if (!cores)
		return (0);
	if (!cores->next && is_core_builtin(cores->node))
		return (exec_core(sh, cores->node, false));
	curr = cores;
	while (curr && sh->pipeline.count < 1024)
	{
		if (exec_pipeline_core(sh, curr) != 0)
			return (1);
		curr = curr->next;
	}
	return (wait_pids((&sh->pipeline)));
}