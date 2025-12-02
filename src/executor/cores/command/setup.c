//
// Created by afonsusousa on 10/18/25.
//

#include <stdlib.h>
#include <unistd.h>
#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../includes/utils.h"
#include "../../../../includes/globbing.h"
#include "../../../../lib/libft/libft.h"

static bool has_unquoted_var(const char *word)
{
    bool in_sq;
    bool in_dq;

    in_sq = false;
    in_dq = false;
    while (*word)
    {
        if (*word == '\'' && !in_dq)
            in_sq = !in_sq;
        else if (*word == '"' && !in_sq)
            in_dq = !in_dq;
        else if (*word == '$' && !in_sq && !in_dq && *(word + 1))
            return (true);
        word++;
    }
    return (false);
}

char **argv_to_arr(const t_minishell *sh, const char **words, int *argc)
{
    char **argv;
    char **expanded_part;
    char **split;
    int i;

    argv = NULL;
    *argc = 0;
    if (!words)
        return (NULL);
    while (words && *words)
    {
        expanded_part = expand_argv_word(sh, *words);
        if (has_unquoted_var(*words))
        {
            if (expanded_part && expanded_part[0] && ft_strchr(expanded_part[0], ' '))
            {
                split = ft_split(expanded_part[0], ' ');
                i = 0;
                while (expanded_part[++i])
                    split = strjoinjoin(split, get_double_from_str(expanded_part[i]));
                free_until_null(&expanded_part);
                expanded_part = split;
            }
        }
        *argc += expanded_part != NULL;
        argv = strjoinjoin(argv, expanded_part);
        words++;
    }
    return (argv);
}

void free_argv(char **argv)
{
    size_t i;

    if (!argv)
        return;
    i = 0;
    while (argv[i])
        free(argv[i++]);
    free(argv);
}