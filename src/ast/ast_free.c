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

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "utils.h"

static void	free_pipeline(const t_ast *node)
{
	ast_list_free(node->as.pipeline.cores);
}

static void	free_grouping(const t_ast *node)
{
	ast_free(node->as.grouping.list);
	ast_list_free(node->as.grouping.redirs);
}

static void	free_simple_command(const t_ast *node)
{
	free_until_null((char ***)&node->as.command.assignments);
	free_until_null((char ***)&node->as.command.argv);
	ast_list_free(node->as.command.redirs);
}

void	ast_free(t_ast *node)
{
	if (node == NULL)
		return ;
	if (node->type == AST_COMMAND_LINE)
		ast_free(node->as.command_line.list);
	else if (node->type == AST_PIPELINE)
		free_pipeline(node);
	else if (node->type == AST_GROUPING)
		free_grouping(node);
	else if (node->type == AST_COMMAND)
		free_simple_command(node);
	else if (node->type == AST_GROUPING)
		ast_free(node->as.grouping.list);
	else if (node->type == AST_REDIR)
		free((char *)node->as.redir.target);
	else if (node->type == AST_OR_LIST || node->type == AST_AND_LIST)
	{
		ast_free(node->as.binop.left);
		ast_free(node->as.binop.right);
	}
	free(node);
}
