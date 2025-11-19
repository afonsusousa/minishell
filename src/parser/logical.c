//
// Created by afonsusousa on 11/4/25.
//

#include "minishell.h"
#include "../../includes/parser.h"

t_ast	*parse_or_list(t_minishell *sh)
{
    t_ast	*lhs;
    t_ast	*rhs;

    if (sh->aborted_parse)
        return (NULL);
    lhs = parse_and_list(sh);
    if (!lhs || sh->aborted_parse)
        return (NULL);
    while (!sh->aborted_parse && ts_match(sh->ts, TOK_OR))
    {
        rhs = parse_and_list(sh);
        if (!rhs || sh->aborted_parse)
            return (ast_free(lhs), NULL);
        lhs = ast_make_binary_node(AST_OR_LIST, lhs, rhs);
        if (!lhs)
            return (ast_free(rhs), NULL);
    }
    return (lhs);
}

t_ast	*parse_and_list(t_minishell *sh)
{
    t_ast	*lhs;
    t_ast	*rhs;

    if (sh->aborted_parse)
        return (NULL);
    lhs = parse_pipeline(sh);
    if (!lhs || sh->aborted_parse)
        return (NULL);
    while (!sh->aborted_parse && ts_match(sh->ts, TOK_AND))
    {
        rhs = parse_pipeline(sh);
        if (!rhs || sh->aborted_parse)
            return (ast_free(lhs), NULL);
        lhs = ast_make_binary_node(AST_AND_LIST, lhs, rhs);
        if (!lhs)
            return (ast_free(rhs), NULL);
    }
    return (lhs);
}