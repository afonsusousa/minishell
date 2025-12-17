/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cores.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:35:42 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:35:44 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "../../includes/utils.h"
#include "globbing.h"
#include <unistd.h>
#include "executor.h"

t_ast	*parse_core(t_minishell *sh)
{
	t_ast	*core;

	if (sh->aborted_parse)
		return (NULL);
	if (ts_check(sh->ts, TOK_LPAREN))
		core = parse_grouping(sh);
	else if (ts_check(sh->ts, TOK_WORD) || ts_check(sh->ts, TOK_ASSIGNMENT_WORD)
		|| is_redir_ahead(sh->ts))
		core = parse_command(sh);
	else
		return (parser_abort_error(sh), NULL);
	if (!core || sh->aborted_parse)
		return (NULL);
	return (core);
}

t_ast	*parse_grouping(t_minishell *sh)
{
	t_ast	*list;
	t_ast	*grp;

	if (sh->aborted_parse)
		return (NULL);
	if (!ts_match(sh->ts, TOK_LPAREN))
		return (parser_abort_error(sh), NULL);
	list = parse_command_line(sh, true);
	if (!list || sh->aborted_parse)
		return (NULL);
	if (!ts_match(sh->ts, TOK_RPAREN))
		return (parser_abort_error(sh), ast_free(list), NULL);
	grp = ast_new(AST_GROUPING);
	if (!grp)
		return (parser_abort_error(sh), ast_free(list), NULL);
	grp->u_as.s_grouping.list = list;
	grp->u_as.s_grouping.redirs = parse_core_redirs(sh);
	if (sh->aborted_parse)
		return (ast_free(grp), NULL);
	return (grp);
}

static void	parse_cmd_pieces(t_minishell *sh, char ***argv, int *argc,
		t_ast_list **redirs)
{
	while (!sh->aborted_parse)
	{
		if ((ts_match(sh->ts, TOK_WORD) || ts_match(sh->ts,
					TOK_ASSIGNMENT_WORD)) && ++(*argc))
			*argv = str_arr_append(*argv, sh->ts->tk->lexeme);
		else if (is_redir_ahead(sh->ts))
			ast_list_push_list(redirs, parse_core_redirs(sh));
		else
			break ;
	}
}

t_ast	*parse_command(t_minishell *sh)
{
	const char	**assignments;
	int			argc;
	char		**argv;
	t_ast_list	*redirs;
	t_ast		*node;

	if (sh->aborted_parse)
		return (NULL);
	assignments = parse_assignments(sh);
	argc = 0;
	argv = NULL;
	redirs = NULL;
	parse_cmd_pieces(sh, &argv, &argc, &redirs);
	if (sh->aborted_parse)
		return (free_until_null((char ***)&argv),
			ast_list_free(redirs),
			free_until_null((char ***)&assignments), NULL);
	node = ast_make_command_node(assignments, argv, argc, redirs);
	if (!node)
		return (free_until_null((char ***)&argv),
			ast_list_free(redirs),
			free_until_null((char ***)&assignments), node);
	return (node);
}

const char	**parse_assignments(t_minishell *sh)
{
	char	**assignments;

	assignments = NULL;
	while (!sh->aborted_parse)
	{
		if (!sh->ts->tk || !ts_match(sh->ts, TOK_ASSIGNMENT_WORD))
			break ;
		assignments = str_arr_append(assignments, sh->ts->tk->lexeme);
	}
	return ((const char **)assignments);
}
