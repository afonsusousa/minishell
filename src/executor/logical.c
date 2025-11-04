//
// Created by afonsusousa on 10/18/25.
//

#include "../../includes/minishell.h"
#include "../../includes/executor.h"

int exec_logical(t_minishell* sh, t_ast* node)
{
    int left_status;

    if (!node || (node->type != AST_AND_LIST && node->type != AST_OR_LIST))
        return (1);
    left_status = exec_node(sh, node->as.binop.left);
    if (node->type == AST_AND_LIST)
    {
        if (left_status == 0)
            return (exec_node(sh, node->as.binop.right));
        return (left_status);
    }
    if (node->type == AST_OR_LIST)
    {
        if (left_status != 0)
            return (exec_node(sh, node->as.binop.right));
        return (left_status);
    }
    return (1);
}