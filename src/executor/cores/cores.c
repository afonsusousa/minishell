/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cores.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:29:44 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:39:21 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/executor.h"
#include "../../../includes/minishell.h"
#include <unistd.h>

static void	save_parent_fds(t_minishell *sh, int saved[3], bool in_fork,
			t_ast_list *redirs)
{
	if (!in_fork && redirs != NULL)
	{
		saved[0] = dup(STDIN_FILENO);
		saved[1] = dup(STDOUT_FILENO);
		saved[2] = dup(STDERR_FILENO);
	}
	else
	{
		saved[0] = -1;
		saved[1] = -1;
		saved[2] = -1;
	}
	sh->pipeline.saved_fds = saved;
}

void	restore_fds(t_minishell *sh)
{
	int	*saved;

	saved = sh->pipeline.saved_fds;
	if (saved[0] >= 0)
	{
		dup2(saved[0], STDIN_FILENO);
		close(saved[0]);
	}
	if (saved[1] >= 0)
	{
		dup2(saved[1], STDOUT_FILENO);
		close(saved[1]);
	}
	if (saved[2] >= 0)
	{
		dup2(saved[2], STDERR_FILENO);
		close(saved[2]);
	}
}

int	exec_grouping(t_minishell *sh, const t_ast *node)
{
	int	status;

	if (exec_redirs(sh, node->u_as.s_grouping.redirs))
		return (1);
	status = exec_node(sh, node->u_as.s_grouping.list);
	return (status);
}

// our lives depend upon this args' lifetime
int	exec_core(t_minishell *sh, const t_ast *core, bool in_fork)
{
	int			status;
	int			saved[3];
	t_ast_list	*redirs;

	if (!core || (core->type != AST_COMMAND && core->type != AST_GROUPING))
		return (1);
	if (core->type == AST_GROUPING)
		redirs = core->u_as.s_grouping.redirs;
	else
		redirs = core->u_as.s_command.redirs;
	save_parent_fds(sh, saved, in_fork, redirs);
	if (core->type == AST_COMMAND)
		status = exec_command(sh, core);
	else
		status = exec_grouping(sh, core);
	if (!in_fork)
		restore_fds(sh);
	sh->last_status = status;
	return (status);
}
