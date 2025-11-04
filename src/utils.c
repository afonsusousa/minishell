//
// Created by wlucas-f on 10/23/25.
//

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../includes/utils.h"
#include "../lib/libft/libft.h"

int count_words(const char* str, const char sep)
{
    int count;
    bool in_word;

    count = 0;
    in_word = false;
    while (*str)
    {
        if (!in_word && *str != sep)
        {
            in_word = true;
            count++;
        }
        else if (*str == sep)
            in_word = false;
        str++;
    }
    return (count);
}


void free_until_null(char*** str_v)
{
    size_t i;

    i = 0;
    if (!*str_v)
        return;
    while ((*str_v)[i])
        free((*str_v)[i++]);
    free(*str_v);
    *str_v = NULL;
}

char* strjoin_three(char* s1, char* s2, char* s3)
{
    int i;
    char* ret;

    i = 0;
    ret = malloc(ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1);
    if (!ret)
        return (NULL);
    if (!s1 || !s2 || !s3)
        return (NULL);
    while (*s1)
        ret[i++] = *s1++;
    while (*s2)
        ret[i++] = *s2++;
    while (*s3)
        ret[i++] = *s3++;
    ret[i] = 0;
    return (ret);
}

char **strjoinjoin(char **a, char **b)
{
    size_t len_a = 0, len_b = 0, i = 0, j = 0;
    char **result;

    if (a)
        while (a[len_a]) len_a++;
    if (b)
        while (b[len_b]) len_b++;
    result = malloc(sizeof(char *) * (len_a + len_b + 1));
    if (!result)
        return NULL;
    for (i = 0; i < len_a && a; i++)
        result[i] = a[i];
    if (a)
        free(a);
    for (j = 0; j < len_b && b; j++)
        result[i + j] = b[j];
    if (b)
        free(b);
    result[len_a + len_b] = NULL;
    return result;
}

char    *ft_strnjoin(const char *s1, const char *s2, size_t scnd)
{
    char *ret;
    char *og;

    ret = ft_calloc(ft_strlen(s1) + scnd + 1, sizeof(char));
    og = ret;
    if (!ret)
        return (NULL);
    while (s1 && *s1)
        *ret++ = *s1++;
    while (s2 && *s2 && scnd--)
        *ret++ = *s2++;
    *ret = 0;
    return (og);
}
char    *ft_strndup(const char *str, size_t size)
{
    char	*t;
    char	*r;

    if (!str)
        return (NULL);
    t = (char *)malloc(size + 1);
    if (!t)
        return (NULL);
    r = t;
    while ((size_t)(t - r) < size && *str)
        *t++ = *str++;
    *t = 0;
    return (r);
}

bool is_valid(char c)
{
    return ((c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || c == '_');
}

char **get_double_from_str(const char *str)
{
    char **ret;

    ret = ft_calloc(2, sizeof(char *));
    if (!ret)
        return (NULL);
    ret[0] = ft_strdup(str);
    return ret;
}

