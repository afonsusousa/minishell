/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_nodes_branches.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:25:19 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 18:45:12 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include <stdlib.h>

t_ast	*ast_make_binary_node(t_ast_type type, t_ast *left, t_ast *right)
{
	t_ast	*n;

	n = ast_new(type);
	if (!n)
		return (NULL);
	n->u_as.s_binop.left = left;
	n->u_as.s_binop.right = right;
	return (n);
}

t_ast	*ast_make_command_line_node(t_ast *list, int terminator)
{
	t_ast	*n;

	n = ast_new(AST_COMMAND_LINE);
	if (!n)
		return (NULL);
	n->u_as.s_command_line.list = list;
	n->u_as.s_command_line.terminator = terminator;
	return (n);
}

t_ast	*ast_make_pipeline_node(t_ast *first_core)
{
	t_ast	*n;

	n = ast_new(AST_PIPELINE);
	if (!n)
		return (NULL);
	n->u_as.s_pipeline.cores = NULL;
	if (!ast_list_push(&n->u_as.s_pipeline.cores, first_core))
		return (free(n), NULL);
	return (n);
}

t_ast	*ast_make_grouping_node(t_ast *list)
{
	t_ast	*n;

	n = ast_new(AST_GROUPING);
	if (!n)
		return (NULL);
	n->u_as.s_grouping.list = list;
	n->u_as.s_command.redirs = NULL;
	return (n);
}
