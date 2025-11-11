//
// Created by afonsusousa on 9/29/25.
//

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../../includes/minishell.h"
#include "../../includes/globbing.h"
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"

#include <stdio.h>
#include <unistd.h>


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



//TODO: split wild_string into directory and wildcard
//WALLAÇOOOOO CARA DE PAU E PAU DE AÇO MEU ORGULHO
// cwd -> diretorio de origem onde procurar
//wildstr -> e uma split(padrao, '/')
// /home/afonsusousa/*cu/*.c/* -> cwd = "/home/afonsusousa" && wildstr = ["*cu","*.c", "*"]
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
        //TODO: figure out hidden files
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

//ISSUE
char    **expand_cwd_wildcards(const char *wild_string)
{
    char **splt;

    splt = ft_split(wild_string, '/');
    if (!splt)
        return (NULL);
    return (get_matches(splt[0], &splt[1]));
}
