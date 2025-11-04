//
// Created by afonsusousa on 11/4/25.
//

#include "../../includes/parser.h"
#include "../../includes/utils.h"

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

