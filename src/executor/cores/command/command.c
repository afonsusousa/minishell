//
// Created by afonsusousa on 11/4/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../includes/utils.h"

static int exec_assignments(t_minishell* sh, const char **a, bool context)
{
    t_envp *env;

    env = sh->env;
    if (context)
        env = sh->ctx;
    while (a && *a)
        //sub??
        if (envp_setvar(env, *a++, context) == NULL)
            return (1);
    return (0);
}

int execve_wrapper(t_minishell* sh, char** argv)
{
    char** env_arr;

    if (!argv || !argv[0])
        return (0);
    env_arr = get_envp_array(sh->env);
    env_arr = strjoinjoin(env_arr, get_envp_array(sh->ctx));
    if (is_builtin(argv[0]))
        return (exec_builtin(sh, argv));
    argv[0] = find_path(argv[0], env_arr);
    minishell_free(sh);
    execve(argv[0], argv, env_arr);
    perror("execve");
    exit(127);
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
    argv = argv_to_arr(sh, node->as.command.argv);
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