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

static char *expand_tilde(const t_minishell *sh, char *cmd)
{
    char *home;
    char *expanded;

    if (cmd[0] != '~')
        return (NULL);
    home = envp_getvar_value(sh, "HOME");
    if (!home)
        return (ft_strdup(cmd));
    if (cmd[1] == '\0')
        expanded = ft_strdup(home);
    else if (cmd[1] == '/')
        expanded = strjoin_three(home, "", &cmd[1]);
    else
        expanded = ft_strdup(cmd);
    free(home);
    if (!expanded)
        return (ft_strdup(cmd));
    return (expanded);
}

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

static char *search_path(t_minishell *sh, char *cmd)
{
    size_t i;
    char **split_path;
    char *try;
    char *path;

    path = envp_getvar_value(sh, "PATH");
    if (!path)
        return (NULL);
    split_path = ft_split(path, ':');
    free(path);
    if (!split_path)
        return (NULL);
    i = 0;
    while (split_path[i] && ft_strcmp(cmd, ".") != 0)
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

char *find_path(t_minishell *sh, char *cmd)
{
    char *expanded_cmd;

    expanded_cmd = expand_tilde(sh, cmd);
    if (!expanded_cmd)
        expanded_cmd = cmd;
    if (ft_strchr(expanded_cmd, '/'))
    {
        if (access(expanded_cmd, F_OK) == 0)
            return (expanded_cmd);
        if (expanded_cmd != cmd)
            free(expanded_cmd);
        return (ft_strdup(cmd));
    }
    if (expanded_cmd != cmd)
        free(expanded_cmd);
    return (search_path(sh, cmd));
}