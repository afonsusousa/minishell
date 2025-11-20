//
// Created by afonsusousa on 11/4/25.
//

#include <stdio.h>
#include <unistd.h>

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
    list = parse_command_line(sh);
    if (!list || sh->aborted_parse)
        return (NULL);
    if (!ts_match(sh->ts, TOK_RPAREN))
        return (parser_abort_error(sh), ast_free(list), NULL);
    grp = ast_new(AST_GROUPING);
    if (!grp)
        return (parser_abort_error(sh), ast_free(list), NULL);
    grp->as.grouping.list = list;
    grp->as.grouping.redirs = parse_core_redirs(sh);
    if (sh->aborted_parse)
        return (ast_free(grp), NULL);
    return (grp);
}

t_ast		*parse_command(t_minishell *sh)
{
    const char      **assignments;
    int             argc;
    char	        **argv;
    t_ast_list	    *redirs;

    if (sh->aborted_parse)
        return (NULL);
    assignments = parse_assignments(sh);
    argc = 0;
    argv = NULL;
    redirs = NULL;
    while (!sh->aborted_parse)
    {
        if ((ts_match(sh->ts, TOK_WORD) || ts_match(sh->ts, TOK_ASSIGNMENT_WORD))
            && ++argc)
            argv = strjoinjoin(argv, get_double_from_str(sh->ts->tk->lexeme));
        else if (is_redir_ahead(sh->ts))
            redirs = parse_core_redirs(sh);
        else
            break ;
    }
    if (sh->aborted_parse)
        return (free_until_null((char ***) &assignments), NULL);
    return (ast_make_command_node(assignments, argv, argc, redirs));
}

const char	**parse_assignments(t_minishell *sh)
{
    char            **assignments;

    assignments = NULL;
    while (!sh->aborted_parse)
    {
        if (!sh->ts->tk || !ts_match(sh->ts, TOK_ASSIGNMENT_WORD))
            break;
        assignments = strjoinjoin(assignments, get_double_from_str(sh->ts->tk->lexeme));
    }
    return ((const char **) assignments);
}
