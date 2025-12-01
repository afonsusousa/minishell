//
// Created for tracked string split functionality
//

#include <stdlib.h>
#include "../../includes/sm.h"
#include "../../includes/utils.h"

static size_t skip_separators(const char *str, size_t pos, char sep)
{
    while (str[pos] && str[pos] == sep)
        pos++;
    return (pos);
}

static size_t find_word_end(const char *str, size_t pos, char sep)
{
    while (str[pos] && str[pos] != sep)
        pos++;
    return (pos);
}

static t_word *create_split_part(t_word *ts, size_t start, size_t len)
{
    t_word *ret;
    size_t k;

    ret = malloc(sizeof(t_word));
    if (!ret)
        return (NULL);
    ret->content = ft_strndup(&ts->content[start], len);
    if (!ret->content)
        return (free(ret), NULL);
    ret->len = len;
    ret->quoted_map = malloc(sizeof(bool) * (len + 1));
    if (!ret->quoted_map)
        return (free(ret->content), free(ret), NULL);
    k = 0;
    while (k < len)
    {
        ret->quoted_map[k] = ts->quoted_map[start + k];
        k++;
    }
    ret->quoted_map[len] = false;
    return (ret);
}

static void free_partial(t_word **result, size_t count)
{
    while (count > 0)
        word_free(result[--count]);
    free(result);
}

t_word **word_split(t_word *ts, char sep)
{
    t_word **ret;
    size_t split_count;
    size_t i;
    size_t j;
    size_t start;

    if (!ts || !ts->content)
        return (NULL);
    split_count = count_words(ts->content, sep);
    ret = malloc(sizeof(t_word*) * (split_count + 1));
    if (!ret)
        return (NULL);
    i = 0;
    j = 0;
    while (i < split_count)
    {
        j = skip_separators(ts->content, j, sep);
        start = j;
        j = find_word_end(ts->content, j, sep);
        ret[i] = create_split_part(ts, start, j - start);
        if (!ret[i])
            return (free_partial(ret, i), NULL);
        i++;
    }
    ret[split_count] = NULL;
    return (ret);
}