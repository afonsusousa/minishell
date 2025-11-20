//
// Created by afonsusousa on 11/4/25.
//

#include <stdlib.h>

#include "../../includes/parser.h"

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

t_ast	*ast_make_command_node(const char **assignments, char **argv, int argc, t_ast_list *redirs)
{
    t_ast	*n;

    n = ast_new(AST_COMMAND);
    if (!n)
        return (NULL);
    n->as.command.assignments = assignments;
    n->as.command.argv = (const char **)argv;
    n->as.command.argc = argc;
    n->as.command.redirs = redirs;
    return (n);
}