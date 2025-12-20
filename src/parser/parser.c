/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:36:12 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:36:13 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "minishell.h"
#include <string.h>

t_ast	*parse_command_line(t_minishell *sh, bool subshell)
{
	int		term;
	t_ast	*node;

	if (sh->aborted_parse || ts_match(sh->ts, TOK_EOF))
		return (NULL);
	node = parse_logical(sh);
	if (!node || sh->aborted_parse)
		return (NULL);
	term = 0;
	if (ts_match(sh->ts, TOK_SEMI))
		term = ';';
	else if (ts_match(sh->ts, TOK_AMP))
		term = '&';
	else if (!subshell && !ts_match(sh->ts, TOK_EOF))
		return (parser_abort_error(sh), ast_free(node), NULL);
	return (ast_make_command_line_node(node, term));
}

void	parse(t_minishell *sh)
{
	int	here_status;

	here_status = 0;
	if (sh->aborted_parse)
	{
		ast_free(sh->ast);
		sh->ast = NULL;
		return ;
	}
	sh->ast = parse_command_line(sh, false);
	if (sh->aborted_parse)
	{
		ast_free(sh->ast);
		sh->ast = NULL;
		return ;
	}
	here_status = heredoc_descend(sh, sh->ast);
	if (here_status != 0)
	{
		ast_free(sh->ast);
		sh->ast = NULL;
		sh->aborted_parse = true;
		sh->last_status = here_status;
	}
}
