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

#include <stdlib.h>
#include <string.h>


typedef struct s_parser
{
	 t_token_stream	ts;
}t_parser;

static t_ast	*parse_command_line(t_parser *p);
static t_ast	*parse_list(t_parser *p);
static t_ast	*parse_or_list(t_parser *p);
static t_ast	*parse_and_list(t_parser *p);
static t_ast	*parse_pipeline(t_parser *p);
// Forward decls for helpers referenced below
static t_ast		*parse_redir(t_parser *p);
static t_ast_list	*parse_command_redirs(t_parser *p);
static t_ast		*parse_simple_command(t_parser *p);
static t_ast		*parse_command(t_parser *p);
static t_ast		*parse_grouping(t_parser *p);
static t_ast_list	*parse_assignments(t_parser *p);

// Typed leaf helper
static t_ast	*ast_make_leaf_typed(t_ast_type type, const char *text, size_t len)
{
	t_ast	*n;
	char	*s;

	n = ast_new(type);
	if (!n)
		return (NULL);
	s = (char *)malloc(len + 1);
	if (!s)
	{
		free(n);
		return (NULL);
	}
	memcpy(s, text, len);
	s[len] = '\0';
	n->as.leaf.text = s;
	n->as.leaf.len = len;
	return (n);
}

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

static t_ast	*ast_make_pipeline_node(t_ast *first_cmd)
{
	// Create a pipeline node and push the first command into its commands list
	t_ast	*n;

	n = ast_new(AST_PIPELINE);
	if (!n)
		return (NULL);
	n->as.pipeline.commands = NULL;
	if (!ast_list_push(&n->as.pipeline.commands, first_cmd))
	{
		free(n);
		return (NULL);
	}
	return (n);
}

static t_ast	*ast_make_command_node(t_ast *core)
{
	 t_ast	*n;

	n = ast_new(AST_COMMAND);
	if (!n)
		return (NULL);
	n->as.command.core = core;
	n->as.command.redirs = NULL;
	return (n);
}

static t_ast	*ast_make_redir_node(t_token_type type)
{
	t_ast *n;

	n = ast_new(AST_REDIR);
	if (!n)
		return (NULL);
	n->as.redir.kind = type;
	return (n);
}

static t_ast	*ast_make_leaf_node(const char *text, size_t len)
{
	// Delegate to typed leaf helper for AST_WORD
	return ast_make_leaf_typed(AST_WORD, text, len);
}

// Removed unused empty ast_make_simple_cmd_node to avoid undefined behavior

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
	t_ast	*cmd;
	t_ast	*pipeline;

	cmd = parse_command(p);
	if (!cmd)
		return (NULL);
	pipeline = ast_make_pipeline_node(cmd);
	if (!pipeline)
		return (NULL);
	while (ts_match(&p->ts, TOK_PIPE))
	{
		cmd = parse_command(p);
		if (!cmd)
			return (NULL);
		if (!ast_list_push(&pipeline->as.pipeline.commands, cmd))
			return (NULL);
	}
	return (pipeline);
}

static t_ast	*parse_grouping(t_parser *p)
{
	// Parse a parenthesized command list into a grouping node
	t_ast	*list;
	t_ast	*grp;

	if (!ts_match(&p->ts, TOK_LPAREN))
		return (NULL);
	list = parse_command_line(p);
	if (!list)
		return (NULL);
	if (!ts_match(&p->ts, TOK_RPAREN))
	{
		// unmatched '(', fail
		ast_free(list);
		return (NULL);
	}
	grp = ast_new(AST_GROUPING);
	if (!grp)
	{
		ast_free(list);
		return (NULL);
	}
	grp->as.grouping.list = list;
	return (grp);
}

static t_ast	*parse_command(t_parser *p)
{
	 t_ast			*core;
	 t_ast_list		*trailing_redirs;
	 t_ast			*cmd;

	if (ts_check(&p->ts, TOK_LPAREN))
		core = parse_grouping(p);
	else
		core = parse_simple_command(p);
	if (!core)
		return (NULL);
	trailing_redirs = parse_command_redirs(p);
	cmd = ast_make_command_node(core);
	if (!cmd)
		return (NULL);
	cmd->as.command.redirs = trailing_redirs;
	return (cmd);
}

static inline int	is_redir_token_type(t_token_type t)
{
	return (t == TOK_REDIR_IN
		|| t == TOK_REDIR_OUT
		|| t == TOK_REDIR_APPEND
		|| t == TOK_HEREDOC);
}

static inline int	is_trailing_redir_ahead(const t_parser *p)
{
	const t_token *tk;

	tk = ts_peek(&((t_parser *)p)->ts);
	return (tk != NULL && is_redir_token_type(tk->type));
}

static t_ast_list	*parse_command_redirs(t_parser *p)
{
	// Parse zero or more trailing redirections using list helpers only
	t_ast_list	*redirs;
	t_ast		*redir_node;

	redirs = NULL;
	while (is_trailing_redir_ahead(p))
	{
		redir_node = parse_redir(p);
		if (redir_node == NULL)
			break;
		if (ast_list_push(&redirs, redir_node) == NULL)
		{
			// allocation failure; stop collecting
			break;
		}
	}
	return (redirs);
}

static t_ast *parse_redir(t_parser *p)
{
	// Parse a redirection operator followed by a WORD target
	t_ast		*redir;
	const t_token *op;
	const t_token *tk;

	op = ts_advance(&p->ts);
	if (!op || !is_redir_token_type(op->type))
		return (NULL);
	redir = ast_make_redir_node(op->type);
	if (!redir)
		return (NULL);
	tk = ts_peek(&p->ts);
	if (tk && tk->type == TOK_WORD)
	{
		// consume the filename token
		ts_advance(&p->ts);
		redir->as.redir.target = ast_make_leaf_node(tk->lexeme, tk->len);
	}
	else
	{
		// missing filename; you may want to signal an error later
		redir->as.redir.target = NULL;
	}
	return (redir);
}

static t_ast_list	*parse_assignments(t_parser *p)
{
	// Collect leading assignment words into AST_ASSIGNMENT leaf nodes
	t_ast_list	*assignments;
	const t_token	*tk;
	t_ast		*node;

	assignments = NULL;
	while (1)
	{
		tk = ts_peek(&p->ts);
		if (!tk || tk->type != TOK_ASSIGNMENT_WORD)
			break;
		// consume assignment token
		ts_advance(&p->ts);
		node = ast_make_leaf_typed(AST_ASSIGNMENT, tk->lexeme, tk->len);
		if (!node)
			break;
		if (!ast_list_push(&assignments, node))
			break;
	}
	return (assignments);
}

static t_ast		*parse_simple_command(t_parser *p)
{
	t_ast			*simple_cmd;
	t_ast_list		*words;
	t_ast_list		*redirs;
	const t_token	*peek;
	t_ast			*node;

	simple_cmd = ast_new(AST_SIMPLE_COMMAND);
	if (!simple_cmd)
		return (NULL);
	simple_cmd->as.simple_command.assignments = parse_assignments(p);
	while (1)
	{
		peek = ts_peek(&p->ts);
		if (!peek)
			break;
		if (peek->type == TOK_WORD)
		{
			ts_advance(&p->ts);
			node = ast_make_leaf_node(peek->lexeme, peek->len);
			if (!node || !ast_list_push(&words, node))
				break;
		}
		else if (is_trailing_redir_ahead(p))
		{
			node = parse_redir(p);
			if (!node || !ast_list_push(&redirs, node))
				break;
		}
		else
			break;
	}
	simple_cmd->as.simple_command.words = words;
	simple_cmd->as.simple_command.redirs = redirs;
	return (simple_cmd);
}
