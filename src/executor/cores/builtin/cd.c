#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../lib/libft/libft.h"

// rage against the norme
int exec_cd(const t_minishell *sh, char **argv, const int argc)
{
    char path[ARG_MAX];
    char oldpwd[ARG_MAX];
    char *home;

    if (argc > 2)
        return ((write(2, "minishell: cd: too many arguments\n", 34) & 0) | 2);
    if (argc == 1)
    {
        home = envp_getvar_value(sh, "HOME");
        if (home)
            ft_strlcpy(path, home, ARG_MAX);
        else
            return ((write(2, "bash: cd: HOME not set\n", 24)  & 0) | 1);
    }
    else if (!*argv[1])
        ft_strlcpy(path, ".", 2);
    else
        ft_strlcpy(path, argv[1], ARG_MAX);
    getcwd(oldpwd, ARG_MAX);
    if (chdir(path) == -1)
        return (perror("cd:"), 1);
    envp_setvar_pair(sh->env, "OLDPWD", oldpwd, true);
    envp_setvar_pair(sh->env, "PWD", getcwd(path, ARG_MAX), true);
    return (0);
}
