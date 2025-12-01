//
// Created for tracked string utilities
//

#include <stdlib.h>
#include "../../includes/sm.h"
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"

void word_free(t_word *ts)
{
    if (!ts)
        return;
    free(ts->content);
    free(ts->quoted_map);
    free(ts);
}

t_word *word_dup(const t_word *ts)
{
    t_word *new_ts;
    size_t i;

    if (!ts)
        return (NULL);
    new_ts = malloc(sizeof(t_word));
    if (!new_ts)
        return (NULL);
    new_ts->content = ft_strdup(ts->content);
    if (!new_ts->content)
        return (free(new_ts), NULL);
    new_ts->len = ts->len;
    new_ts->quoted_map = malloc(sizeof(bool) * (ts->len + 1));
    if (!new_ts->quoted_map)
        return (free(new_ts->content), free(new_ts), NULL);
    i = 0;
    while (i <= ts->len)
    {
        new_ts->quoted_map[i] = ts->quoted_map[i];
        i++;
    }
    return (new_ts);
}

void word_free_until_null(t_word **splits)
{
    size_t i;

    if (!splits)
        return;
    i = 0;
    while (splits[i])
    {
        word_free(splits[i]);
        i++;
    }
    free(splits);
}
