/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afonsusousa <student@42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 00:00:00 by afonsusousa       #+#    #+#             */
/*   Updated: 2025/09/25 00:00:00 by afonsusousa      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"


typedef struct s_parser
{
	 t_token_stream	ts;
}t_parser;

static t_ast	*parse_command_line(t_parser *p);
static t_ast	*parse_list(t_parser *p);
static t_ast	*parse_or_list(t_parser *p);
static t_ast	*parse_and_list(t_parser *p);
static t_ast	*parse_pipeline(t_parser *p);

static t_ast	*ast_make_binary_node(t_ast_type type, t_ast *left, t_ast *right)
{
	t_ast	*n;

	n = ast_new(type);
	if (!n)
		return (NULL);
	n->as.binop.left = left;
	n->as.binop.right = right;
	return (n);
}

static t_ast	*ast_make_command_line_node(t_ast *list, int terminator)
{
	t_ast	*n;

	n = ast_new(AST_COMMAND_LINE);
	if (!n)
		return (NULL);
	n->as.command_line.list = list;
	n->as.command_line.terminator = terminator;
	return (n);
}

static t_ast	*parse_command_line(t_parser *p)
{
	int		term;
	t_ast	*list_node;

	if (!p)
		return (NULL);
	list_node = parse_list(p);
	if (!list_node)
		return (NULL);
	term = 0;
	if (ts_match(&p->ts, TOK_SEMI))
		term = ';';
	else if (ts_match(&p->ts, TOK_AMP))
		term = '&';
	return (ast_make_command_line_node(list_node, term));
}

static t_ast	*parse_list(t_parser *p)
{
	return (parse_or_list(p));
}

static t_ast	*parse_or_list(t_parser *p)
{
	t_ast	*lhs;
	t_ast	*rhs;

	lhs = parse_and_list(p);
	if (!lhs)
		return (NULL);
	while (ts_match(&p->ts, TOK_OR_IF))
	{
		rhs = parse_and_list(p);
		if (!rhs)
			return (NULL);
		lhs = ast_make_binary_node(AST_OR_LIST, lhs, rhs);
		if (!lhs)
			return (NULL);
	}
	return (lhs);
}

static t_ast	*parse_and_list(t_parser *p)
{
	t_ast	*lhs;
	t_ast	*rhs;

	lhs = parse_pipeline(p);
	if (!lhs)
		return (NULL);
	while (ts_match(&p->ts, TOK_AND_IF))
	{
		rhs = parse_pipeline(p);
		if (!rhs)
			return (NULL);
		lhs = ast_make_binary_node(AST_AND_LIST, lhs, rhs);
		if (!lhs)
			return (NULL);
	}
	return (lhs);
}

static t_ast	*parse_pipeline(t_parser *p)
{
	t_ast	*lhs;
	t_ast	*rhs;
	lhs = parse_command(p);
	if (!lhs)
		return (NULL);
	while (ts_match(&p->ts, TOK_PIPE))
	{
		rhs = parse_command(p);
		if (!rhs)
			return (NUll);
		lhs  = ast_make_binary_node(AST_PIPELINE, lhs, rhs);
		if (!lhs)
			return (NULL);
	}
	return (lhs);
}

static t_ast	*parse_command(t_parser *p)
{

}
