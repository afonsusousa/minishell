/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:36:24 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:25:07 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "../../lib/libft/libft.h"
#include "minishell.h"

t_ast_list	*parse_core_redirs(t_minishell *sh)
{
	t_ast_list	*redirs;
	t_ast		*redir_node;

	redirs = NULL;
	while (!sh->aborted_parse && is_redir_ahead(sh->ts))
	{
		redir_node = parse_redir(sh);
		if (!redir_node)
			break ;
		if (ast_list_push(&redirs, redir_node) == NULL)
			break ;
	}
	return (redirs);
}

t_ast	*parse_redir(t_minishell *sh)
{
	t_ast			*redir;
	t_token_type	redir_type;

	if (sh->aborted_parse)
		return (NULL);
	redir_type = is_redir_token_type(sh->ts);
	if (!redir_type)
		return (NULL);
	redir = ast_make_redir_node(redir_type);
	if (!redir)
		return (NULL);
	if (ts_match(sh->ts, TOK_WORD))
		redir->u_as.s_redir.u_target.file_name = ft_strdup(sh->ts->tk->lexeme);
	else
		return (parser_abort_error(sh), ast_free(redir), NULL);
	return (redir);
}