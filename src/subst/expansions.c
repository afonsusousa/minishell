//
// Created by afonsusousa on 12/1/25.
//

#include <stdlib.h>
#include "../../includes/minishell.h"
#include "../../includes/globbing.h"
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"

char *expand_tilde(const t_minishell *sh, char *cmd)
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
char **expand_argv_word(const t_minishell *sh, const char *word)
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