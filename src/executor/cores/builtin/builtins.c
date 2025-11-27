#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../includes/utils.h"

int exec_builtin(t_minishell *sh, char **argv, int argc)
{
    if (ft_strcmp("cd", argv[0]) == 0)
        return (exec_cd(sh, argv, argc));
    if (ft_strcmp("echo", argv[0]) == 0)
        return (exec_echo(argv, argc));
    if (ft_strcmp("env", argv[0]) == 0)
        return (exec_env(sh, argv, argc));
    if (ft_strcmp("exit", argv[0]) == 0)
        return (exec_exit(sh, argv, argc));
    if (ft_strcmp("export", argv[0]) == 0)
        return (exec_export(sh, argv, argc));
    if (ft_strcmp("pwd", argv[0]) == 0)
        return (exec_pwd(sh, argv, argc));
    if (ft_strcmp("unset", argv[0]) == 0)
        return (exec_unset(sh, argv, argc));
    return (1);
}
