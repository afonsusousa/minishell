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
static char **expand_argv_word(const t_minishell *sh, const char *word)
{
    char *exp;
    char **words;
    char **matches;
    char **result;
    size_t i;

    if (!word)
        return (NULL);
    exp = expanded(sh->env, word, true, true);
    if (!exp)
        return (NULL);
    words = ft_split(exp, ' ');
    free(exp);
    if (!words)
        return (get_double_from_str(word));
    result = NULL;
    i = 0;
    while (words[i])
    {
        matches = expand_cwd_wildcards(words[i]);
        if (!matches)
            result = strjoinjoin(result, get_double_from_str(words[i]));
        else
            result = strjoinjoin(result, matches);
        i++;
    }
    free_until_null(&words);
    if (!result)
        return (get_double_from_str(word));
    return (result);
}

char** argv_to_arr(const t_minishell* sh, const  char **words)
{
    char    **argv;
    char    **expanded_part;
    int     size;

    size = 0;
    argv = NULL;
    if (!words)
        return (NULL);
    while (words && *words)
    {
        expanded_part = expand_argv_word(sh, *words++);
        argv = strjoinjoin(argv, expanded_part);
    }
    while (argv && argv[size])
        size++;
    merge_sort_strings(&argv[1], 0, size - 2);
    return (argv);
}

void free_argv(char** argv)
{
    size_t i;

    if (!argv)
        return;
    i = 0;
    while (argv[i])
        free(argv[i++]);
    free(argv);
}

//TODO: local access (./)
char *find_path(char* cmd, char** envp)
{
    size_t i;
    char** split_path;
    char* try;

    i = 0;
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