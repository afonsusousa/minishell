/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:33:33 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:21:45 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"
#include "../../includes/minishell.h"

int	exec_command_line(t_minishell *sh, const t_ast *node)
{
	if (!node || node->type != AST_COMMAND_LINE)
		return (1);
	return (exec_node(sh, node->u_as.s_command_line.list));
}

int	exec_node(t_minishell *sh, const t_ast *node)
{
	if (!node)
		return (1);
	if (node->type == AST_COMMAND_LINE)
		return (exec_command_line(sh, node));
	if (node->type == AST_PIPELINE)
		return (exec_pipeline(sh, node->u_as.s_pipeline.cores));
	if (node->type == AST_COMMAND)
		return (exec_command(sh, node));
	if (node->type == AST_GROUPING)
		return (exec_grouping(sh, node));
	if (node->type == AST_AND_LIST || node->type == AST_OR_LIST)
		return (exec_logical(sh, node));
	return (0);
}

int	exec_ast(t_minishell *sh)
{
	int	status;

	status = exec_node(sh, sh->ast);
	sh->last_status = status;
	return (status);
}