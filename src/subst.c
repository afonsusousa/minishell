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

size_t needed_space(t_envp *env, char *str)
{
    size_t    i;
    size_t    total_length;
    char     *value;

    i = 0;
    total_length = 0;
    value = NULL;
    while (str[i])
    {
        if (str[i++] == '$')
            value = envp_get_elem_value(env, str + i);
        if (value != NULL)
        {
            total_length += strlen(value);
            while (!isspace((unsigned char)str[i]) && str[i])
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
    size_t  size;
    size_t  written;
    char    *expanded;

    size = needed_space(env, str);
    expanded = calloc(size + 1, sizeof(char));
    if (!expanded)
        return NULL;

    while (str[in])
    {
        written = check_copy(env, expanded + out, str + in);
        if (written > 0)
        {
            out += written;
            in += 1 + key_len(str + in + 1); // skip '$' and its name
            continue;
        }
        expanded[out++] = str[in++];
    }
    expanded[out] = '\0';
    return (expanded);
}
