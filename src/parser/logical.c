//
// Created by afonsusousa on 11/4/25.
//

#include "minishell.h"
#include "../../includes/parser.h"

t_ast	*parse_or_list(t_minishell *sh)
{
    t_ast	*lhs;
    t_ast	*rhs;

    lhs = parse_and_list(sh);
    if (!lhs)
        return (NULL);
    while (ts_match(sh->ts, TOK_OR))
    {
        rhs = parse_and_list(sh);
        if (!rhs)
            return (NULL);
        lhs = ast_make_binary_node(AST_OR_LIST, lhs, rhs);
        if (!lhs)
            return (NULL);
    }
    return (lhs);
}

t_ast	*parse_and_list(t_minishell *sh)
{
    t_ast	*lhs;
    t_ast	*rhs;

    lhs = parse_pipeline(sh);
    if (!lhs)
        return (NULL);
    while (ts_match(sh->ts, TOK_AND))
    {
        rhs = parse_pipeline(sh);
        if (!rhs)
            return (NULL);
        lhs = ast_make_binary_node(AST_AND_LIST, lhs, rhs);
        if (!lhs)
            return (NULL);
    }
    return (lhs);
}