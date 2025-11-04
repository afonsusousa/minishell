#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../includes/utils.h"

int export(t_minishell *sh, char **argv);

int exec_builtin(t_minishell *sh, char **argv)
{
    if (ft_strcmp("export", argv[0]) == 0)
        return (export(sh, argv));
    return (1);
}