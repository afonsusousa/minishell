/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descent.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:35:35 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:03:28 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"
#include "../../../includes/minishell.h"
#include "../../../includes/parser.h"
#include <stdlib.h>

static int	redir_leaf(t_minishell *sh, const t_ast_list *redir)
{
	int	status;

	status = 0;
	while (redir)
	{
		if (redir->node->u_as.s_redir.kind == TOK_HEREDOC)
		{
			sh->heredoc.del = expanded(sh->env,
					redir->node->u_as.s_redir.u_target.file_name,
					sh->last_status, CONSUME_QUOTES);
			free((char *)redir->node->u_as.s_redir.u_target.file_name);
			redir->node->u_as.s_redir.u_target.file_name = NULL;
			status = heredoc_setup(sh,
					&redir->node->u_as.s_redir.u_target.heredoc);
			if (redir->node->u_as.s_redir.u_target.file_name)
				redir->node->u_as.s_redir.kind = TOK_OPEN_HEREDOC;
			if (status)
				return (status);
		}
		redir = redir->next;
	}
	return (0);
}

static int	descend_pipeline(t_minishell *sh, const t_ast_list *cores)
{
	int	status;

	while (cores)
	{
		status = heredoc_descend(sh, cores->node);
		if (status)
			return (status);
		cores = cores->next;
	}
	return (0);
}

int	heredoc_descend(t_minishell *sh, t_ast *node)
{
	int	status;

	if (!node)
		return (0);
	if (node->type == AST_COMMAND_LINE)
		return (heredoc_descend(sh, node->u_as.s_command_line.list));
	if (node->type == AST_PIPELINE)
		return (descend_pipeline(sh, node->u_as.s_pipeline.cores));
	if (node->type == AST_COMMAND)
		return (redir_leaf(sh, node->u_as.s_command.redirs));
	if (node->type == AST_GROUPING)
	{
		status = heredoc_descend(sh, node->u_as.s_grouping.list);
		if (status)
			return (status);
		return (redir_leaf(sh, node->u_as.s_grouping.redirs));
	}
	if (node->type == AST_AND_LIST || node->type == AST_OR_LIST)
	{
		status = heredoc_descend(sh, node->u_as.s_binop.left);
		if (status)
			return (status);
		return (heredoc_descend(sh, node->u_as.s_binop.right));
	}
	return (0);
}
