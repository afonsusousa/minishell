//
// Created by afonsusousa on 11/4/25.
//

#include <string.h>

#include "../../../../../includes/minishell.h"
#include "../../../../../includes/executor.h"

static int exec_assignments(t_minishell* sh, const char **a, bool context)
{
    t_envp *env;

    env = sh->env;
    if (context)
        env = sh->ctx;
    while (a && *a)
        if (envp_setvar(env, *a++, context) == NULL)
            return (1);
    return (0);
}

int exec_command(t_minishell* sh, t_ast* node, bool in_fork)
{
    char** argv;
    int status;
    t_envp local_env;
    (void)in_fork;

    memset(sh->ctx, 0, sizeof(t_envp));
    if (!node || node->type != AST_COMMAND)
        return (1);
    argv = words_to_argv(sh, node->as.command.words);
    if (argv && is_builtin(argv[0]))
        return (exec_builtin(sh, argv));
    if (exec_redirs(sh, node->as.command.redirs, in_fork))
        return (1);
    if (exec_assignments(sh, node->as.command.assignments, argv != NULL))
        return (1);
    if (!argv)
        return (0);
    status = execve_wrapper(sh, argv);
    //restore_fds();
    free_argv(argv);
    free_envp(&local_env);
    sh->last_status = status;
    return (status);
}