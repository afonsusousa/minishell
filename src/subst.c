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

#include <math.h>

#include "../includes/utils.h"

#include <stdio.h>
#include <unistd.h>

int   handle_quotes(bool *sq, bool *dq, char c)
{
    if (!dq && c == '\'')
    {
        *sq = !*sq;
        return (1);
    }
    if (!sq && c == '"')
    {
        *dq = !*dq;
        return (1);
    }
    return (0);
}

char *expanded_segment(const t_envp *env, const char *str, size_t n, bool freedom)
{
    size_t  i;
    size_t  j;
    char *ret;
    char *value;

    i = 0;
    j = 0;
    ret = "";
    value = NULL;
    while (str[i] && (freedom || str[i] = ))
    {
        if (str[i] == '$')
        {
            value = envp_getvar_value(env, str + 1);
            ret = ft_strjoin(ret, value);
            j += ft_strlen(value);
        }
        else
            ret = ft_strjoin()
        i++;
    }
}
char *expanded_str(const t_envp *env, const char *str)
{
    bool sq;
    bool dq;
    char *ret;
    char *next;

    sq = false;
    dq = false;
    while (*str)
    {
        next = NULL;
        if (handle_quotes(&sq, &dq, *str))
            continue ;
        if (sq)
            next = ft_strchr(str, '\'');
        else if (dq)
            next = ft_strchr(str, '"');
        if (next)
        {
            if (sq)
                ret = ft_strnjoin(ret, str, next - str);
            if (dq)
                ret = ft_strjoin(ret, expanded_segment(env, str, next - str, true));
            str += next - str + 1;
            continue ;
        }
        if (*str == '$')
            ret = ft_strjoin(ret, envp_getvar_value(env, str + 1));

    }
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

char **get_double_from_str(char *str)
{
    char **ret;

    ret = ft_calloc(2, sizeof(char *));
    if (!ret)
        return (NULL);
    ret[0] = ft_strdup(str);
    return ret;
}

//TODO: split wild_string into directory and wildcard
//WALLAÇOOOOO CARA DE PAU E PAU DE AÇO MEU ORGULHO
// cwd -> diretorio de origem onde procurar
//wildstr -> e uma split(padrao, '/')
// /home/afonsusousa/*/*/* -> cwd = "/home/afonsusousa" && wildstr = ["*","*", "*"]
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
   // only puporse here will be to address get_matches with the right parameters
    //currenty does nothing
    return (ft_strdup(wild_string));
}
