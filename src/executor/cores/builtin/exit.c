#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../lib/libft/libft.h"

static bool is_numeric(const char *str)
{
    int i;

    i = 0;
    if (!str || !*str)
        return (false);
    if (str[i] == '+' || str[i] == '-')
        i++;
    if (!str[i])
        return (false);
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (false);
        i++;
    }
    return (true);
}

int exec_exit(t_minishell *sh, char **argv, const int argc)
{
    int code;

    code = 0;
    if (argc == 2)
    {
        if (!is_numeric(argv[1]))
        {
            write(2, "minishell: exit: ", 17);
            write(2, argv[1], ft_strlen(argv[1]));
            write(2, ": numeric argument required\n", 28);
            minishell_free(sh);
            exit(2);
        }
        code = ft_atoi(argv[1]) % 256;
        if (code < 0)
            code += 256;
    }
    else if (argc > 2)
    {
        if (!is_numeric(argv[1]))
        {
            write(2, "minishell: exit: ", 17);
            write(2, argv[1], ft_strlen(argv[1]));
            write(2, ": numeric argument required\n", 28);
            minishell_free(sh);
            exit(2);
        }
        return ((write(2, "minishell: exit: too many arguments\n", 36) & 0) | 2);
    }
    minishell_free(sh);
    exit(code);
}