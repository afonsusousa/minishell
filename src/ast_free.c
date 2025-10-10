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

#include <stdlib.h>
#include "ast.h"

static void	free_pipeline(const t_ast *node)
{
	t_ast_list	*c;

	c = node->as.pipeline.commands;
	while (c != NULL)
	{
		ast_free(c->node);
		c = c->next;
	}
	ast_list_free(node->as.pipeline.commands);
}

static void	free_command(const t_ast *node)
{
	t_ast_list	*r;

	ast_free(node->as.command.core);
	r = node->as.command.redirs;
	while (r != NULL)
	{
		ast_free(r->node);
		r = r->next;
	}
	ast_list_free(node->as.command.redirs);
}

static void	free_simple_command_lists(const t_ast_list *lst)
{
	while (lst != NULL)
	{
		ast_free(lst->node);
		lst = lst->next;
	}
}

static void	free_simple_command(const t_ast *node)
{
	free_simple_command_lists(node->as.simple_command.assignments);
	ast_list_free(node->as.simple_command.assignments);
	free_simple_command_lists(node->as.simple_command.words);
	ast_list_free(node->as.simple_command.words);
	free_simple_command_lists(node->as.simple_command.redirs);
	ast_list_free(node->as.simple_command.redirs);
}

void	ast_free(t_ast *node)
{
	if (node == NULL)
		return ;
	if (node->type == AST_COMMAND_LINE)
		ast_free(node->as.command_line.list);
	else if (node->type == AST_PIPELINE)
		free_pipeline(node);
	else if (node->type == AST_COMMAND)
		free_command(node);
	else if (node->type == AST_SIMPLE_COMMAND)
		free_simple_command(node);
	else if (node->type == AST_GROUPING)
		ast_free(node->as.grouping.list);
	else if (node->type == AST_REDIR || node->type == AST_HEREDOC)
		ast_free(node->as.redir.target);
	else if (node->type == AST_OR_LIST || node->type == AST_AND_LIST)
	{
		ast_free(node->as.binop.left);
		ast_free(node->as.binop.right);
	}
	else if (node->type == AST_WORD || node->type == AST_ASSIGNMENT)
	{
		free((char *)node->as.leaf.text);
		node->as.leaf.text = NULL;
	}
	free(node);
}
