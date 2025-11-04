//
// Created by afonsusousa on 11/4/25.
//

#include "../../includes/parser.h"

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