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

// parameter expansion -> wildcard expansion
// THERE CAN ONLY BE WILDCARDS ON SINGLE WORD (if it were spaced, it would imply quotes!)
static char **expand_argv_word(const t_minishell *sh, const char *word)
{
    char *exp;
    char *tilde_exp;
    char **matches;
    char **result;
    int  size;

    if (!word)
        return (NULL);
    tilde_exp = expand_tilde((t_minishell *)sh, (char *)word);
    if (tilde_exp)
        exp = expanded(sh, tilde_exp, EXPAND_VARS | CONSUME_QUOTES);
    else
        exp = expanded(sh, word, EXPAND_VARS | CONSUME_QUOTES);
    if (tilde_exp)
        free(tilde_exp);
    if (!exp)
        return (NULL);
    result = NULL;
    if (*word != '\'' && *word != '"' && ft_strchr(exp, '*'))
        matches = expand_cwd_wildcards(exp);
    else
        matches = NULL;
    if (!matches || !*matches)
        result = strjoinjoin(result, get_double_from_str(exp));
    else
    {
        size = 0;
        while (matches[size])
            size++;
        merge_sort_strings(matches, 0, size - 1);
        result = strjoinjoin(result, matches);
    }
    if (!result)
        return (free(exp), get_double_from_str(word));
    return (free(exp), result);
}

char **argv_to_arr(const t_minishell *sh, const char **words)
{
    char **argv;
    char **expanded_part;
    char **split;
    int i;

    argv = NULL;
    if (!words)
        return (NULL);
    while (words && *words)
    {
        expanded_part = expand_argv_word(sh, *words);
        if (!ft_strchr(*words, '"') && !ft_strchr(*words, '\''))
        {
            if (expanded_part && expanded_part[0] && ft_strchr(expanded_part[0], ' '))
            {
                split = ft_split(expanded_part[0], ' ');
                i = 1;
                while (expanded_part[i])
                {
                    split = strjoinjoin(split, get_double_from_str(expanded_part[i]));
                    i++;
                }
                free_until_null(&expanded_part);
                expanded_part = split;
            }
        }
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