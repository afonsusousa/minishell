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

//
// Created by afonsusousa on 9/29/25.
//

#include "../includes/subst.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

char    **get_matches(char *cwd, char **wildstr, )
{
    char **ret = NULL;
    char *next_call;
    char path[PATH_MAX];
    DIR *dir;
    struct dirent *entry;
    dir = opendir(cwd);
    entry = readdir(dir);

    strcpy(path, cwd);
    strlcat(path, "/", ft_strlen(path) + 2);
    if (!wildstr)
        return ();
    while (entry)
    {
        if (match_wildcard(*wildstr, entry->d_name))
        {
            next_call = ft_strjoin(path, entry->d_name);
            if (opendir(next_call))
                ret = ft_strjoinjoin(ret, get_matches(next_call, wildstr + 1));
            else if (access(next_call))
            {
                ret = ft_calloc(2 , sizeof(char *));
                ret[0] = next_call;
            }
        }
        entry = readdir(dir);
    }
    return (ret);
}

char    *get_those_dirs(char *wild_string)
{
    size_t  i;
    char **dirs_to_retrieve;
    char path[PATH_MAX];

    i = 0;
    dirs_to_retrieve = ft_split(wild_string, '/');
    while (dirs_to_retrieve != NULL)
    {
       if (ft_strncmp(dirs_to_retrieve[0], ".", 1) == 0)
           continue;
    }
}
char    *expand_cwd_wildcards(const char *wild_string)
{
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
