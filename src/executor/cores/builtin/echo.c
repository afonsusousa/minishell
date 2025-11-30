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
    if (argc > 1 && ft_strcmp(argv[1], "-n") == 0)
    {
        n = false;
        i = 2;
    }
    while (i < (size_t) argc)
    {
        printf("%s", argv[i]);
        if (i != (size_t) argc - 1)
            printf(" ");
        i++;
    }
    if (n)
        printf("\n");
    return (0);
}

