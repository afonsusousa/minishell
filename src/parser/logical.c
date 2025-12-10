/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:36:01 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:36:07 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "minishell.h"

t_ast	*parse_logical(t_minishell *sh)
{
	t_ast *lhs;
	t_ast *rhs;
	t_ast_type node_type;

	if (sh->aborted_parse)
		return (NULL);
	lhs = parse_pipeline(sh);
	if (!lhs || sh->aborted_parse)
		return (NULL);
	while (!sh->aborted_parse)
	{
		if (ts_match(sh->ts, TOK_AND))
			node_type = AST_AND_LIST;
		else if (ts_match(sh->ts, TOK_OR))
			node_type = AST_OR_LIST;
		else
			break ;
		rhs = parse_pipeline(sh);
		if (!rhs || sh->aborted_parse)
			return (ast_free(lhs), NULL);
		lhs = ast_make_binary_node(node_type, lhs, rhs);
		if (!lhs)
			return (ast_free(rhs), NULL);
	}
	return (lhs);
}