#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

#include "utils.h"
#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../lib/libft/libft.h"

// rage against the norme
int exec_cd(const t_minishell *sh, char **argv, const int argc)
{
    char path[ARG_MAX];
    char oldpwd[ARG_MAX];
    char *subst;

    if (argc > 2)
        return ((write(2, "minishell: cd: too many arguments\n", 34) & 0) | 2);
    if (argc == 1)
    {
        subst = envp_getvar_value(sh, "HOME");
        if (subst)
            ft_strlcpy(path, subst, ARG_MAX);
        else
            return ((write(2, "minishell: cd: HOME not set\n", 24)  & 0) | 1);
    }
    else if (!*argv[1])
        ft_strlcpy(path, ".", 2);
    else if (ft_strcmp(argv[1], "-") == 0)
    {
        subst = envp_getvar_value(sh, "OLDPWD");
        if (!subst)
            return ((write(2, "minishell: cd: OLDPWD not set\n", 30)  & 0) | 1);
        ft_strlcpy(path, subst, ARG_MAX);
        write(1, path, ft_strlen(path));
        write(1, "\n", 1);
        free(subst);
    }
    else
        ft_strlcpy(path, argv[1], ARG_MAX);
    getcwd(oldpwd, ARG_MAX);
    if (chdir(path) == -1)
    {
        write(2, "minishell: cd: ", 15);
        write(2, path, ft_strlen(path));
        write(2, ": ", 2);
        return (perror(NULL), 1);
    }
    envp_setvar_pair(sh->env, "OLDPWD", oldpwd, true);
    envp_setvar_pair(sh->env, "PWD", getcwd(path, ARG_MAX), true);
    return (0);
}
