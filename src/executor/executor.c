//
// Created by afonsusousa on 11/4/25.
//

#include "../../includes/minishell.h"
#include "../../includes/executor.h"

int exec_command_line(t_minishell* sh, t_ast* node)
{
    if (!node || node->type != AST_COMMAND_LINE)
        return (1);
    return (exec_node(sh, node->as.command_line.list));
}

int exec_node(t_minishell* sh, t_ast* node)
{
    if (!node)
        return (1);
    if (node->type == AST_COMMAND_LINE)
        return (exec_command_line(sh, node));
    if (node->type == AST_PIPELINE)
        return (exec_pipeline(sh, node->as.pipeline.cores));
    if (node->type == AST_COMMAND)
        return (exec_command(sh, node, false));
    if (node->type == AST_GROUPING)
        return (exec_grouping(sh, node, false));
    if (node->type == AST_AND_LIST || node->type == AST_OR_LIST)
        return (exec_binop(sh, node));
    return (0);
}

int exec_ast(t_minishell* sh)
{
    int status;

    status = exec_node(sh, sh->ast);
    sh->last_status = status;
    return (status);
}