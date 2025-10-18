//
// Created by afonsusousa on 9/29/25.
//

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../includes/minishell.h"
#include "../includes/envp.h"
#include "../includes/subst.h"

#include <stdio.h>
#include <unistd.h>

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

//bash stores the wildcard in env, then expands it in the getter
size_t needed_space(const t_envp *env, const char *str)
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
            value =  envp_get_var_value(env, str + i);
        if (value != NULL)
        {
            total_length += strlen(value);
            while (str[i] && !isspace((unsigned char)str[i]))
                i++;
            free(value);
            value = NULL;
        }
        else
            total_length++;
    }
    return (total_length);
}

size_t  check_copy(const t_envp *env, char *dest, const char *src_elem)
{
    size_t i;
    char *value;

    i = 0;
    if (src_elem[0] != '$')
        return (0);
    value = envp_get_var_value(env, src_elem + 1);
    while (value != NULL && value[i])
    {
        dest[i] = value[i];
        i++;
    }
    free(value);
    return (i);
}

char *expanded_str(const t_envp *env, const char *str)
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

bool match_wildcard(const char *exp, const char *str)
{
    const char *star = NULL;
    const char *ss = NULL;

    while (*str) {
        if (*exp == '*')
        {
            star = exp++;
            ss = str;
        }
        else if (*exp == *str)
        {
            exp++;
            str++;
        }
        else if (star)
        {
            exp = star + 1;
            str = ++ss;
        }
        else
            return false;
    }
    while (*exp == '*')
        exp++;
    return *exp == '\0';
}

bool	is_expandable(char *str)
{
    size_t  i;
    bool	in_single;
    bool	in_double;

    i = 0;
    in_single = false;
    in_double = false;
    if (!str)
        return (false);
    while (str[i])
    {
        if (str[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (str[i] == '\"' && !in_single)
            in_double = !in_double;
        else if (str[i] == '*' && !in_single && !in_double)
        {
            if (ft_strchr(str, '='))
                return (ft_strchr(str, '=') < &str[i]);
            return (true);
        }
        i++;
    }
    return (false);
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
//TODO: split wild_string into directory and wildcard

//WARNING THIS WILL TAKE A LOT MORE WORK: ./*PATH*/*.C <-- everything will need to be expanded
char **strjoinjoin(char **a, char **b) {
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

char **get_double_from_str(char *str)
{
    char **ret;

    ret = ft_calloc(2, sizeof(char *));
    if (!ret)
        return (NULL);
    ret[0] = ft_strdup(str);
    return ret;
}

//WALLAÇOOOOO CARA DE PAU E PAU DE AÇO MEU ORGULHO
char    **get_matches(char *cwd, char **wildstr)
{
    char            **ret;
    char            *next_call;
    char            path[PATH_MAX];
    DIR         	*dir[2];
    struct dirent   *entry;

    ret = NULL;
    dir[0] = opendir(cwd);
    entry = readdir(dir[0]);
    if (!wildstr || !*wildstr)
    {
        if (access(cwd, F_OK) == 0)
            return (get_double_from_str(cwd));
        return NULL;
    }
    strcpy(path, cwd);
    ft_strlcat(path, "/", ft_strlen(path) + 2);
    while (entry)
    {
        if ((entry->d_name[0] != '.' || (*wildstr)[0] == '.') && match_wildcard(*wildstr, entry->d_name))
        {
            next_call = ft_strjoin(path, entry->d_name);
            dir[1] = opendir(next_call);
            if (dir[1])
            {
                ret = strjoinjoin(ret, get_matches(next_call, wildstr + 1));
                closedir(dir[1]);
            }
            else if (access(next_call, F_OK) == 0 && !wildstr[1])
                ret = strjoinjoin(ret, get_double_from_str(next_call));
            free(next_call);
        }
        entry = readdir(dir[0]);
    }
    closedir(dir[0]);
    return (ret);
}

char    *expand_cwd_wildcards(const char *wild_string)
{
    char cwd[PATH_MAX];
    DIR *dir;
    struct dirent *entry;
    size_t  size;
    size_t  i;
    char    *ret;

    size = 0;
    i = 0;
    getcwd(cwd, PATH_MAX);
    dir = opendir(cwd);
    entry = readdir(dir);
    while (entry)
    {
        if (match_wildcard(wild_string, entry->d_name))
        {
            size += ft_strlen(entry->d_name);
            i++;
        }
        entry = readdir(dir);
    }
    size += i == 1 ? 0 : i - 1;
    if (i == 0)
        return (ft_strdup(wild_string));
    closedir(dir);
    ret = calloc(size + 1, sizeof(char));
    dir = opendir(cwd);
    entry = readdir(dir);
    while (entry && i)
    {
        if (match_wildcard(wild_string,entry->d_name))
        {
            ft_strlcat(ret, entry->d_name, size + 1);
            ft_strlcat(ret, " ", i != 1 ? size : 0);
            i--;
        }
        entry = readdir(dir);
    }
    //closedir(dir);
    return (ret);
}
