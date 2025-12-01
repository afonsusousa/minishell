//
// Created by afonsusousa on 11/4/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../includes/utils.h"
#include "../../../../lib/libft/libft.h"

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

static void execve_error(const char *cmd)
{
    struct stat path_stat;

    write(2, "minishell: ", 11);
    if (stat(cmd, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
    {
        write(2, cmd, ft_strlen(cmd));
        write(2, ": Is a directory\n", 17);
        exit(126);
    }
    if (errno == ENOENT || (errno == 13 && !ft_strchr(cmd, '/')))
    {
        write(2, cmd, ft_strlen(cmd));
        write(2, ": command not found\n", 20);
        exit(127);
    }
    perror(cmd);
    exit(126);
}

int execve_wrapper(t_minishell* sh, char** argv, int argc)
{
    char** env_arr;

    if (!argv || !argv[0])
        return (0);
    if (is_builtin(argv[0]))
        return (exec_builtin(sh, argv, argc));
    env_arr = get_envp_array(sh->env, true);
    env_arr = strjoinjoin(env_arr, get_envp_array(sh->ctx, true));
    argv[0] = find_path(sh, argv[0]);
    minishell_free(sh);
    execve(argv[0], argv, env_arr);
    execve_error(argv[0]);
    return (0);
}

int exec_command(t_minishell* sh, const t_ast* core)
{
    char** argv;
    int status;

    memset(sh->ctx, 0, sizeof(t_envp));
    if (!core || core->type != AST_COMMAND)
        return (1);
    argv = argv_to_arr(sh, core->as.command.argv);
    if (exec_redirs(sh, core->as.command.redirs))
        return (1);
    if (exec_assignments(sh, core->as.command.assignments, argv != NULL))
        return (1);
    if (!argv)
        return (0);
    status = execve_wrapper(sh, argv, core->as.command.argc);
    free_envp(sh->ctx);
    free_argv(argv);
    sh->last_status = status;
    return (status);
}