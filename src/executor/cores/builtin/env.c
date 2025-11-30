#include <stdio.h>

#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../includes/utils.h"

int exec_env(const t_minishell *sh, char **argv, const int argc)
{
    (void) argv;
    (void) argc;
    char **env;
    char **iter;

    //review export later
    env = get_envp_array(sh->env, true);
    env =  strjoinjoin(env, get_envp_array(sh->ctx, true));
    iter = env;
    while (iter && *iter)
        printf("%s\n", *iter++);
    free_until_null(&env);
    return (0);
}

