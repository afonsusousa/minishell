//
// Created by afonsusousa on 11/4/25.
//

#include <string.h>

#include "../../includes/minishell.h"
#include "../../includes/executor.h"

int exec_grouping(t_minishell* sh, t_ast* node, bool in_fork)
{
    int status;
    exec_redirs(sh, node->as.command.redirs, in_fork);
    status = exec_node(sh, node->as.grouping.list);
    return (status);
}

int exec_core(t_minishell* sh, t_ast* core, bool in_fork)
{
    int status;

    if (!core || (core->type != AST_COMMAND && core->type != AST_GROUPING))
        return (1);
    status = 0;
   //subs
    if (core->type == AST_COMMAND)
        status = exec_command(sh, core, in_fork);
    else
        status = exec_grouping(sh, core, in_fork);
    sh->last_status = status;
    return (status);
}