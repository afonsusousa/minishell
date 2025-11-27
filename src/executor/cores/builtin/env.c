#include <stdio.h>

#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"

int exec_env(const t_minishell *sh, char **argv, const int argc)
{
    (void) argv;
    (void) argc;
    char **env;

    //review export later
    env = get_envp_array(sh->env, false);
    while (env && *env)
        printf("%s\n", *env++);
    return (0);
}

