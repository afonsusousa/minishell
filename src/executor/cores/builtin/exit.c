#include <stdlib.h>
#include <unistd.h>

#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../lib/libft/libft.h"

int exec_exit(t_minishell *sh, char **argv, const int argc)
{
    int code;

    code = 0;
    if (argc == 2)
        code = ft_atoi(argv[1]);
    else if (argc > 2)
        return ((write(2, "minishell: exit: too many arguments\n", 36) | 0) & 2);
    minishell_free(sh);
    exit(code);
}

