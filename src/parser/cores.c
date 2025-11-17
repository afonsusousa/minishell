//
// Created by afonsusousa on 11/4/25.
//

#include "globbing.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

t_ast	*parse_core(t_minishell *sh)
{
    t_ast			*core;

    if (sh->aborted_parse)
        return (NULL);
    if (ts_check(sh->ts, TOK_LPAREN))
        core = parse_grouping(sh);
    else if (ts_check(sh->ts, TOK_WORD)
            || ts_check(sh->ts, TOK_ASSIGNMENT_WORD)
            || is_redir_ahead(sh->ts))
        core = parse_command(sh);
    else
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
        return (NULL);
    list = parse_command_line(sh);
    if (!list || sh->aborted_parse)
        return (NULL);
    if (!ts_match(sh->ts, TOK_RPAREN))
        return (ast_free(list), NULL);
    grp = ast_new(AST_GROUPING);
    if (!grp)
        return (ast_free(list), NULL);
    grp->as.grouping.list = list;
    grp->as.grouping.redirs = parse_core_redirs(sh);
    return (grp);
}

t_ast		*parse_command(t_minishell *sh)
{
    t_ast		    *simple_cmd;
    int             argc;
    char	        **argv;
    t_ast_list	    *redirs;
    const t_token	*peek;

    if (sh->aborted_parse)
        return (NULL);
    simple_cmd = ast_new(AST_COMMAND);
    if (!simple_cmd)
        return (NULL);
    simple_cmd->as.command.assignments = parse_assignments(sh);
    argc = 0;
    argv = NULL;
    redirs = NULL;
    while (!sh->aborted_parse)
    {
        peek = ts_peek(sh->ts);
        if (!peek)
            break ;
        if (peek->type == TOK_WORD || peek->type == TOK_ASSIGNMENT_WORD)
        {
            ts_advance(sh->ts);
            argv = strjoinjoin(argv, get_double_from_str(peek->lexeme));
            argc++;
        }
        else if (is_redir_ahead(sh->ts))
            redirs = parse_core_redirs(sh);
        else
            break ;
    }
    if (sh->aborted_parse)
        return (ast_free(simple_cmd), NULL);
    simple_cmd->as.command.argv = (const char **)argv;
    simple_cmd->as.command.argc = argc;
    simple_cmd->as.command.redirs = redirs;
    return (simple_cmd);
}

const char	**parse_assignments(t_minishell *sh)
{
    char            **assignments;
    const t_token	*tk;

    assignments = NULL;
    while (!sh->aborted_parse)
    {
        tk = ts_peek(sh->ts);
        if (!tk || (tk->type != TOK_ASSIGNMENT_WORD))
            break;
        ts_advance(sh->ts);
        assignments = strjoinjoin(assignments, get_double_from_str(tk->lexeme));
    }
    return ((const char **) assignments);
}
