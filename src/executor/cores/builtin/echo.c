#include <stdio.h>
#include <stdbool.h>

#include "../../../../includes/utils.h"
#include "../../../../lib/libft/libft.h"

int exec_echo(char **argv, const int argc)
{
    size_t i;
    bool    n;

    i = 1;
    n = true;
    while (i < (size_t) argc)
    {
        if (ft_strcmp(argv[i], "-n") != 0)
            printf("%s%.*s", argv[i], i != (size_t) argc - 1, " ");
        else
            n = false;
        i++;
    }
    printf("%.*s", n, "\n");
    return (1);
}

