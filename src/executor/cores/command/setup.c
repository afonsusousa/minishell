//
// Created by afonsusousa on 10/18/25.
//

#include <stdlib.h>
#include <unistd.h>
#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../includes/utils.h"
#include "../../../../includes/globbing.h"

static t_word **split_expanded(t_word **expanded)
{
    t_word **split;
    int i;

    if (!expanded || !expanded[0])
        return (expanded);
    split = word_split(expanded[0], is_space, true);
    if (!split)
        return (expanded);
    if (!split[1])
        return (word_free_until_null(split), expanded);
    word_free(expanded[0]);
    i = 1;
    while (expanded[i])
        split = word_array_append_word(split, expanded[i++]);
    free(expanded);
    return (split);
}

char **argv_to_arr(const t_minishell *sh, const char **words, int *argc)
{
    t_word  **argv_words;
    t_word  **expanded;
    char    **argv;
    int     i;

    argv_words = NULL;
    *argc = 0;
    if (!words)
        return (NULL);
    while (words && *words)
    {
        expanded = expand_argv_word(sh, *words);
        expanded = split_expanded(expanded);
        i = 0;
        while (expanded && expanded[i])
            i++;
        *argc += i;
        argv_words = word_array_join(argv_words, expanded);
        words++;
    }
    argv = word_to_cstr_array(argv_words);
    word_free_until_null(argv_words);
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