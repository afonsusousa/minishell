/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_nodes_leafs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:25:24 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 18:45:06 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include <stdlib.h>

t_ast	*ast_make_redir_node(t_token_type type)
{
	t_ast	*n;

	n = ast_new(AST_REDIR);
	if (!n)
		return (NULL);
	n->u_as.s_redir.kind = type;
	if (type == TOK_HEREDOC)
		n->u_as.s_redir.u_target.heredoc = -1;
	else
		n->u_as.s_redir.u_target.file_name = NULL;
	return (n);
}

t_ast	*ast_make_command_node(const char **assignments, char **argv, int argc,
		t_ast_list *redirs)
{
	t_ast	*n;

	n = ast_new(AST_COMMAND);
	if (!n)
		return (NULL);
	n->u_as.s_command.assignments = assignments;
	n->u_as.s_command.argv = (const char **)argv;
	n->u_as.s_command.argc = argc;
	n->u_as.s_command.redirs = redirs;
	return (n);
}
