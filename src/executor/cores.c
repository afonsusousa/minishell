//
// Created by afonsusousa on 11/4/25.
//

#include <string.h>
#include <unistd.h>

#include "../../includes/minishell.h"
#include "../../includes/executor.h"

static void save_parent_fds(int saved[2], bool in_fork)
{
    if (!in_fork)
    {
        saved[0] = dup(STDIN_FILENO);
        saved[1] = dup(STDOUT_FILENO);
    }
    else
    {
        saved[0] = -1;
        saved[1] = -1;
    }
}

static void restore_fds(int saved[2])
{
    if (saved[0] >= 0)
    {
        dup2(saved[0], STDIN_FILENO);
        close(saved[0]);
    }
    if (saved[1] >= 0)
    {
        dup2(saved[1], STDOUT_FILENO);
        close(saved[1]);
    }
}

int exec_grouping(t_minishell* sh, const t_ast* node)
{
    int status;

    if (exec_redirs(sh, node->as.grouping.redirs))
        return (1);
    status = exec_node(sh, node->as.grouping.list);
    return (status);
}

int exec_core(t_minishell* sh, const t_ast* core, bool in_fork)
{
    int status;
    int saved[2];

    if (!core || (core->type != AST_COMMAND && core->type != AST_GROUPING))
        return (1);
    save_parent_fds(saved, in_fork);
    if (core->type == AST_COMMAND)
        status = exec_command(sh, core);
    else
        status = exec_grouping(sh, core);
    if (!in_fork)
        restore_fds(saved);
    sh->last_status = status;
    return (status);
}