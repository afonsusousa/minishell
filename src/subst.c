//
// Created by afonsusousa on 9/29/25.
//

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/minishell.h"
#include "../includes/envp.h"

//
// Created by afonsusousa on 9/29/25.
//

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/minishell.h"
#include "../includes/envp.h"

size_t    handle_escape(const char *str, bool *escaped)
{
    size_t  count;

    count = 0;
    while (*str == '\\')
        count++;
    if (count % 2)
    {
        *escaped = true;
        return (count);
    }
    *escaped = false;
    return (count);
}

size_t needed_space(t_envp *env, char *str)
{
    size_t    i;
    size_t    total_length;
    bool     escaped;
    char     *value;

    i = handle_escape(str, &escaped);
    total_length = 0;
    value = NULL;
    while (str[i])
    {
        i += handle_escape(&str[i], &escaped);
        if (!escaped && str[i++] == '$')
            value = envp_get_elem_value(env, str + i);
        if (value != NULL)
        {
            total_length += strlen(value);
            while (str[i] && !isspace((unsigned char)str[i]))
                i++;
            value = NULL;
        }
        else
            total_length++;
    }
    return (total_length);
}

size_t  check_copy(t_envp *env, char *dest, char *src_elem)
{
    size_t i;
    char *value;

    i = 0;
    if (src_elem[0] != '$')
        return (0);
    value = envp_get_elem_value(env, src_elem + 1);
    while (value != NULL && value[i])
    {
        dest[i] = value[i];
        i++;
    }
    return (i);
}

char *expanded_str(t_envp *env, char *str)
{
    size_t  in = 0;
    size_t  out = 0;
    size_t  written;
    bool    escaped;
    char    *expanded;

    expanded = calloc(needed_space(env, str) + 1, sizeof(char));
    if (!expanded)
        return NULL;
    while (str[in])
    {
        in += handle_escape(&str[in], &escaped);
        if (!escaped && str[in] == '$')
        {
            written = check_copy(env, expanded + out, str + in);
            if (written > 0)
            {
                out += written;
                in += 1 + key_len(str + in + 1);
                continue;
            }
        }
        expanded[out++] = str[in++];
    }
    expanded[out] = '\0';
    return (expanded);
}

bool    match_wildcard(char *exp, char *str)
{
    char *loc[2];

    loc[0] = NULL;
    loc[1] = NULL;
    while (*str)
    {
        if (*exp == '*')
        {
            loc[0] = str;
            loc[1] = ++exp;
            if (*exp == '\0')
                return (true);
            continue ;
        }
        if (*str != *exp)
        {
            if (!loc[0])
                return (false);
            str = ++(loc[0]);
            exp = loc[1];
        }
        exp++;
        str++;
    }
    while (*exp == '*')
        exp++;
    return (*exp == '\0');
}