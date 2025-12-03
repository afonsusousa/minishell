//
// Created by afonsusousa on 10/18/25.
//

#include <stdlib.h>
#include <unistd.h>
#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../includes/utils.h"
#include "../../../../includes/globbing.h"
#include "../../../../includes/sm.h"
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
    t_word  **argv_words;
    t_word  **expanded_part;
    t_word  **split;
    char    **argv;
    int     i;

    argv_words = NULL;
    *argc = 0;
    if (!words)
        return (NULL);
    while (words && *words)
    {
        expanded_part = expand_argv_word(sh, *words);
        if (has_unquoted_var(*words))
        {
            if (expanded_part && expanded_part[0] && has_char_fn(expanded_part[0]->content, is_space))
            {
                split = word_split(expanded_part[0], is_space, true);
                i = 0;
                while (expanded_part[++i])
                    split = word_array_append_word(split, expanded_part[i]);
                word_free_until_null(expanded_part);
                expanded_part = split;
            }
        }
        *argc += expanded_part != NULL;
        argv_words = word_array_join(argv_words, expanded_part);
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