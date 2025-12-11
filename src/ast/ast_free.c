/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by afonsusousa       #+#    #+#             */
/*   Updated: 2025/09/24 04:11:19 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	free_pipeline(const t_ast *node)
{
	ast_list_free(node->u_as.s_pipeline.cores);
}

static void	free_grouping(const t_ast *node)
{
	ast_free(node->u_as.s_grouping.list);
	ast_list_free(node->u_as.s_grouping.redirs);
}

static void	free_simple_command(const t_ast *node)
{
	free_until_null((char ***)&node->u_as.s_command.assignments);
	free_until_null((char ***)&node->u_as.s_command.argv);
	ast_list_free(node->u_as.s_command.redirs);
}

static void free_redir(const t_ast *node)
{
    if (node->u_as.s_redir.kind == TOK_HEREDOC)
    {
        if (node->u_as.s_redir.u_target.heredoc >= 0)
            close(node->u_as.s_redir.u_target.heredoc);
    }
    else
        free((char *)node->u_as.s_redir.u_target.file_name);
}

void	ast_free(t_ast *node)
{
	if (node == NULL)
		return ;
	if (node->type == AST_COMMAND_LINE)
		ast_free(node->u_as.s_command_line.list);
	else if (node->type == AST_PIPELINE)
		free_pipeline(node);
	else if (node->type == AST_GROUPING)
		free_grouping(node);
	else if (node->type == AST_COMMAND)
		free_simple_command(node);
	else if (node->type == AST_REDIR)
	    free_redir(node);
	else if (node->type == AST_OR_LIST || node->type == AST_AND_LIST)
	{
		ast_free(node->u_as.s_binop.left);
		ast_free(node->u_as.s_binop.right);
	}
	free(node);
}
