//
// Created by afonsusousa on 12/1/25.
//

#include <math.h>
#include <stdlib.h>
#include "../../includes/minishell.h"
#include "../../includes/globbing.h"
#include "../../includes/utils.h"
#include "../../includes/sm.h"
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

static bool has_unquoted_wildcard(const t_word *ts)
{
    char *star;

    if (!ts || !ts->content)
        return (false);
    star = ft_strchr(ts->content, '*');
    while (star && ts->quoted_map[star - ts->content])
        star = ft_strchr(star + 1, '*');
    return (star != NULL);
}

static char **build_result(t_word *exp_result, char **matches)
{
    char **result;
    int size;

    if (matches && *matches)
    {
        size = 0;
        while (matches[size])
            size++;
        merge_sort_strings(matches, 0, size - 1);
        result = strjoinjoin(NULL, matches);
    }
    else
        result = strjoinjoin(NULL, get_double_from_str(exp_result->content));
    return (result);
}

char    **expand_cwd_wildcards(t_word *word)
{
    t_word **splits;
    char **matches;

    if (!word || !word->content)
        return (NULL);
    if (!has_unquoted_wildcard(word))
        return (get_double_from_str(word->content));
    splits = word_split(word, '/');
    if (!splits)
        return (NULL);
    if (splits[1])
        matches = get_matches(splits[0]->content, &splits[1]);
    else
        matches = get_matches("", splits);
    word_free_until_null(splits);
    if (!matches)
        return (get_double_from_str(word->content));
    return (matches);
}

char **expand_argv_word(const t_minishell *sh, const char *word)
{
    char    *tilde_exp;
    t_word  *exp_word;
    char    **matches;
    char    **ret;

    if (!word)
        return (NULL);
    tilde_exp = expand_tilde((t_minishell *)sh, (char *)word);
    if (tilde_exp)
        exp_word = expanded(sh, tilde_exp, EXPAND_VARS | CONSUME_QUOTES);
    else
        exp_word = expanded(sh, word, EXPAND_VARS | CONSUME_QUOTES);
    free(tilde_exp);
    if (!exp_word)
        return (NULL);
    if (has_unquoted_wildcard(exp_word))
        matches = expand_cwd_wildcards(exp_word);
    else
        matches = NULL;
    ret = build_result(exp_word, matches);
    if (ret)
        return (word_free(exp_word), ret);
    return (word_free(exp_word), get_double_from_str(word));
}