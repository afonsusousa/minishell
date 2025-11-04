#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"
#include "utils.h"

t_ast	*parse_core(t_parser *p);

static inline int	is_redir_token_type(t_token_type t)
{
    return (t == TOK_REDIR_IN
        || t == TOK_REDIR_OUT
        || t == TOK_REDIR_APPEND
        || t == TOK_HEREDOC);
}

static inline int	is_redir_ahead(const t_parser *p)
{
    const t_token *tk;

    tk = ts_peek(&((t_parser *)p)->ts);
    return (tk != NULL && is_redir_token_type(tk->type));
}

t_ast	*ast_make_binary_node(t_ast_type type, t_ast *left, t_ast *right)
{
	t_ast	*n;

	n = ast_new(type);
	if (!n)
		return (NULL);
	n->as.binop.left = left;
	n->as.binop.right = right;
	return (n);
}

t_ast	*ast_make_command_line_node(t_ast *list, int terminator)
{
	t_ast	*n;

	n = ast_new(AST_COMMAND_LINE);
	if (!n)
		return (NULL);
	n->as.command_line.list = list;
	n->as.command_line.terminator = terminator;
	return (n);
}

t_ast	*ast_make_pipeline_node(t_ast *first_core)
{
	t_ast	*n;

	n = ast_new(AST_PIPELINE);
	if (!n)
		return (NULL);
	n->as.pipeline.cores = NULL;
	if (!ast_list_push(&n->as.pipeline.cores, first_core))
		return (free(n), NULL);
	return (n);
}

t_ast	*ast_make_grouping_node(t_ast *list)
{
	 t_ast	*n;

	n = ast_new(AST_GROUPING);
	if (!n)
		return (NULL);
	n->as.grouping.list = list;
	n->as.command.redirs = NULL;
	return (n);
}

t_ast	*ast_make_redir_node(t_token_type type)
{
	t_ast *n;

	n = ast_new(AST_REDIR);
	if (!n)
		return (NULL);
	n->as.redir.kind = type;
	return (n);
}

t_ast	*parse_command_line(t_parser *p)
{
	int		term;
	t_ast	*list_node;

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

t_ast	*parse_list(t_parser *p)
{
	return (parse_or_list(p));
}

t_ast	*parse_or_list(t_parser *p)
{
	t_ast	*lhs;
	t_ast	*rhs;

	lhs = parse_and_list(p);
	if (!lhs)
		return (NULL);
	while (ts_match(&p->ts, TOK_OR))
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

t_ast	*parse_and_list(t_parser *p)
{
	t_ast	*lhs;
	t_ast	*rhs;

	lhs = parse_pipeline(p);
	if (!lhs)
		return (NULL);
	while (ts_match(&p->ts, TOK_AND))
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

t_ast	*parse_pipeline(t_parser *p)
{
	t_ast	*core;
	t_ast	*pipeline;

	core = parse_core(p);
	if (!core)
		return (NULL);
	pipeline = ast_make_pipeline_node(core);
	if (!pipeline)
		return (NULL);
	while (ts_match(&p->ts, TOK_PIPE))
	{
		core = parse_core(p);
		if (!core)
			return (NULL);
		if (!ast_list_push(&pipeline->as.pipeline.cores, core))
			return (NULL);
	}
	return (pipeline);
}

t_ast	*parse_core(t_parser *p)
{
    t_ast			*core;

    if (ts_check(&p->ts, TOK_LPAREN))
        core = parse_grouping(p);
    else if (ts_check(&p->ts, TOK_WORD)
            || ts_check(&p->ts, TOK_ASSIGNMENT_WORD)
            || is_redir_ahead(p))
        core = parse_command(p);
    else
        return (NULL);
    return (core);
}

t_ast	*parse_grouping(t_parser *p)
{
	t_ast	*list;
	t_ast	*grp;

	if (!ts_match(&p->ts, TOK_LPAREN))
		return (NULL);
	list = parse_command_line(p);
	if (!list)
		return (NULL);
	if (!ts_match(&p->ts, TOK_RPAREN))
		return (ast_free(list), NULL);
	grp = ast_new(AST_GROUPING);
	if (!grp)
		return (ast_free(list), NULL);
	grp->as.grouping.list = list;
    grp->as.grouping.redirs = parse_core_redirs(p);
	return (grp);
}

t_ast		*parse_command(t_parser *p)
{
	t_ast		*simple_cmd;
	char	    **words;
	t_ast_list	*redirs;
	const t_token	*peek;

	simple_cmd = ast_new(AST_COMMAND);
	if (!simple_cmd)
		return (NULL);
	simple_cmd->as.command.assignments = parse_assignments(p);
	words = NULL;
	redirs = NULL;
	while (1)
	{
		peek = ts_peek(&p->ts);
		if (!peek)
			break ;
		if (peek->type == TOK_WORD || peek->type == TOK_ASSIGNMENT_WORD)
		{
			ts_advance(&p->ts);
		    words = strjoinjoin(words, get_double_from_str(peek->lexeme));
		}
		else if (is_redir_ahead(p))
			redirs = parse_core_redirs(p);
		else
			break ;
	}
	simple_cmd->as.command.words = (const char **)words;
	simple_cmd->as.command.redirs = redirs;
	return (simple_cmd);
}

const char	**parse_assignments(t_parser *p)
{
    char            **assignments;
    const t_token	*tk;

    assignments = NULL;
    while (1)
    {
        tk = ts_peek(&p->ts);
        if (!tk || (tk->type != TOK_ASSIGNMENT_WORD))
            break;
        ts_advance(&p->ts);
        assignments = strjoinjoin(assignments, get_double_from_str(tk->lexeme));
    }
    return ((const char **) assignments);
}

t_ast_list	*parse_core_redirs(t_parser *p)
{
    t_ast_list	*redirs;
    t_ast		*redir_node;

    redirs = NULL;
    while (is_redir_ahead(p))
    {
        redir_node = parse_redir(p);
        if (redir_node == NULL)
            break;
        if (ast_list_push(&redirs, redir_node) == NULL)
            break;
    }
    return (redirs);
}

t_ast *parse_redir(t_parser *p)
{
    t_ast		*redir;
    const t_token *tk;

    tk = ts_peek(&p->ts);
    if (!tk || !is_redir_token_type(tk->type))
        return (NULL);
    ts_advance(&p->ts);
    redir = ast_make_redir_node(tk->type);
    if (!redir)
        return (NULL);
    tk = ts_peek(&p->ts);
    if (tk && ((tk->type == TOK_WORD)))
    {
        ts_advance(&p->ts);
        redir->as.redir.target = ft_strdup(tk->lexeme);
    }
    else
        return (ast_free(redir), NULL);
    return (redir);
}

t_ast	*parse(const t_token *tokens, const size_t count)
{
	t_parser	p = {0};

	p.ts.data = (t_token *)tokens;
	p.ts.count = count;
	p.ts.capacity = count;
	p.ts.position = 0;
	p.ts.read_position = 0;
	p.ts.tk = NULL;
	return (parse_command_line(&p));
}
