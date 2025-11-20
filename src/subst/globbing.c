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

//WALLAÇOOOOO CARA DE PAU E PAU DE AÇO MEU ORGULHO
static DIR *setup_get_matches(char *cwd, char *path, struct dirent **entry)
{
    DIR *dir;

    if (cwd[0])
        dir = opendir(cwd);
    else
        dir = opendir(".");
    if (cwd[0])
    {
        strcpy(path, cwd);
        ft_strlcat(path, "/", ft_strlen(path) + 2);
    }
    else
        path[0] = '\0';
    *entry = readdir(dir);
    return (dir);
}

// maybe there'll be an issue with hidden files
static char **process_entry(struct dirent *entry, char **wildstr, const char *path)
{
    char *next_call;
    DIR *dir;
    char **ret;

    ret = NULL;
    if ((entry->d_name[0] != '.' || (*wildstr)[0] == '.')
        && match_wildcard(*wildstr, entry->d_name))
    {
        next_call = ft_strjoin(path, entry->d_name);
        dir = opendir(next_call);
        if (dir)
        {
            ret = strjoinjoin(ret, get_matches(next_call, wildstr + 1));
            closedir(dir);
        }
        else if (access(next_call, F_OK) == 0 && !wildstr[1])
            ret = strjoinjoin(ret, get_double_from_str(next_call));
        free(next_call);
    }
    return (ret);
}

char    **get_matches(char *cwd, char **wildstr)
{
    char            **ret;
    char            path[PATH_MAX];
    DIR         	*dir[2];
    struct dirent   *entry;

    ret = NULL;
    dir[0] = setup_get_matches(cwd, path, &entry);
    if (!wildstr || !*wildstr)
    {
        if (access(cwd, F_OK) == 0)
            return (get_double_from_str(cwd));
        return (NULL);
    }
    while (entry)
    {
        ret = strjoinjoin(ret, process_entry(entry, wildstr, path));
        entry = readdir(dir[0]);
    }
    closedir(dir[0]);
    return (ret);
}

//ISSUE
char    **expand_cwd_wildcards(const char *wild_string)
{
    char **splt;
    char **matches;
    bool has_wild;

    if (!wild_string)
        return (NULL);
    has_wild = ft_strchr(wild_string, '*');
    if (!has_wild)
        return (get_double_from_str(wild_string));
    splt = ft_split(wild_string, '/');
    if (!splt)
        return (NULL);
    if (splt[1])
        matches = get_matches(splt[0], &splt[1]);
    else
        matches = get_matches("", &splt[0]);
    free_until_null(&splt);
    if (!matches)
        return (get_double_from_str(wild_string));
    return (matches);
}

