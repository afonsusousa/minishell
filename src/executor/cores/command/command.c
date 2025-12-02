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


static void exit_wrapper(char ***argv, char *orig_cmd, int exit_code)
{
    free_argv(*argv);
    free(orig_cmd);
    exit(exit_code);
}

static void execve_error(char ***argv, char *orig_cmd)
{
    struct stat path_stat;
    char *cmd;

    cmd = **argv;
    write(2, "minishell: ", 11);
    write(2, cmd, ft_strlen(cmd));
    write(2, ": ", 2);
    if (!ft_strchr(cmd, '/'))
        exit_wrapper(argv, orig_cmd,
            (write(2, "command not found\n", 18) & 0) | 127);
    if (stat(cmd, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
        exit_wrapper(argv, orig_cmd,
            (write(2, "Is a directory\n", 15) & 0) | 126);
    if (errno == EACCES)
        exit_wrapper(argv, orig_cmd,
            (write(2, "Permission denied\n", 18) & 0) | 126);
    if (errno == ENOENT)
        exit_wrapper(argv, orig_cmd,
            (write(2, "No such file or directory\n", 26) & 0) | 127);
    perror("");
    exit_wrapper(argv, orig_cmd, 126);
}

int execve_wrapper(t_minishell* sh, char*** argv, int argc)
{
    char** env_arr;
    char    *orig_cmd;
    char    *exec_cmd;

    if (!argv || !*argv || !**argv)
        return (0);
    if (is_builtin(**argv))
        return (exec_builtin(sh, *argv, argc));
    orig_cmd = ft_strdup(**argv);
    env_arr = get_envp_array(sh->env, true);
    env_arr = strjoinjoin(env_arr, get_envp_array(sh->ctx, true));
    exec_cmd = find_path(sh, orig_cmd);
    free(**argv);
    **argv = exec_cmd;
    minishell_free(sh);
    execve(exec_cmd, *argv, env_arr);
    free_envp(sh->ctx);
    free_until_null(&env_arr);
    execve_error(argv, orig_cmd);
    return (126);
}

int exec_command(t_minishell* sh, const t_ast* core)
{
    char** argv;
    int status;

    memset(sh->ctx, 0, sizeof(t_envp));
    if (!core || core->type != AST_COMMAND)
        return (1);
    argv = argv_to_arr(sh, core->as.command.argv, (int *)&core->as.command.argc);
    if (exec_redirs(sh, core->as.command.redirs))
        return (free_argv(argv),1);
    if (exec_assignments(sh, core->as.command.assignments, argv != NULL))
        return (free_argv(argv), 1);
    if (!argv)
        return (0);
    status = execve_wrapper(sh, &argv, core->as.command.argc);
    sh->last_status = status;
    return (status);
}