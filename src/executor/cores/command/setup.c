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
    char **matches;
    char **result;
    int  size;

    if (!word)
        return (NULL);
    exp = expanded(sh, word, EXPAND_VARS | CONSUME_QUOTES);
    if (!exp)
        return (NULL);
    result = NULL;
    if (*word != '\'' && *word != '"' && ft_strchr(exp, '*'))
        matches = expand_cwd_wildcards(exp);
    else
        matches = NULL;
    if (!matches)
        result = strjoinjoin(result, get_double_from_str(exp));
    else
    {
        size = 0;
        while (matches[size])
            size++;
        merge_sort_strings(matches, 0, size - 1);
        result = strjoinjoin(result, matches);
    }
    free(exp);
    if (!result)
        return (get_double_from_str(word));
    return (result);
}

char **argv_to_arr(const t_minishell *sh, const char **words)
{
    char **argv;
    char **expanded_part;

    argv = NULL;
    if (!words)
        return (NULL);
    while (words && *words)
    {
        expanded_part = expand_argv_word(sh, *words++);
        argv = strjoinjoin(argv, expanded_part);
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

char *find_path(char *cmd, char **envp)
{
    size_t i;
    char **split_path;
    char *try;

    i = 0;
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, F_OK) == 0 && access(cmd, X_OK) != 0)
            return (ft_strdup(cmd));
        if (access(cmd, F_OK) == 0)
            return (ft_strdup(cmd));
        return (ft_strdup(cmd));
    }
    while (*envp && (ft_strncmp("PATH=", *envp, 5)))
        envp++;
    if (!*envp)
        return (NULL);
    split_path = ft_split(*envp + 5, ':');
    if (!split_path)
        return (NULL);
    while (split_path[i])
    {
        try = strjoin_three(split_path[i++], "/", cmd);
        if (!try)
            return (free_until_null(&split_path), NULL);
        if (access(try, X_OK) == 0)
            return (free_until_null(&split_path), try);
        free(try);
    }
    return (free_until_null(&split_path), ft_strdup(cmd));
}