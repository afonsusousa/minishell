//
// Created by wlucas-f on 11/1/25.
//
#include "../../includes/minishell.h"

int export(t_minishell *sh, char **argv)
{

}

static int exec_assignments(t_minishell* sh, t_ast_list* a, bool global)
{
    t_envp *env;

    env = sh->ctx;
    if (global)
        env = sh->env;
    while (a)
    {
        if (envp_append_var(env, a->node->as.leaf.text, !global) == NULL)
            return (1);
        a = a->next;
    }
    return (0);
}