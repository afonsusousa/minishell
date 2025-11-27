#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"

int exec_unset(const t_minishell *sh, char **argv, const int argc)
{
    (void) argc;

    if (argv[1] == NULL)
        return (0);
    while (*++argv != NULL)
        envp_unsetvar(sh->env, *argv);
    return (0);
}

