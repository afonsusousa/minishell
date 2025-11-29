#include <stdio.h>

#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../includes/utils.h"

int exec_env(const t_minishell *sh, char **argv, const int argc)
{
    (void) argv;
    (void) argc;
    char **env;

    //review export later
    env = get_envp_array(sh->env, true);
    env =  strjoinjoin(env, get_envp_array(sh->ctx, true));
    while (env && *env)
        printf("%s\n", *env++);
    free_until_null(&env);
    return (0);
}

