/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:36:57 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:36:58 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/globbing.h"
#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"
#include <ctype.h>
#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static bool	match_pattern(const t_word *pattern, const char *str, size_t *i)
{
	const char	*star = NULL;
	const char	*ss = NULL;

	while (*str)
	{
		if (pattern->content[*i] == '*' && !pattern->quoted_map[*i])
		{
			star = &pattern->content[(*i)++];
			ss = str;
		}
		else if (pattern->content[*i] == *str)
		{
			(*i)++;
			str++;
		}
		else if (star)
		{
			*i = (star - pattern->content) + 1;
			str = ++ss;
		}
		else
			return (false);
	}
	return (true);
}

static bool	match_wildcard(const t_word *pattern, const char *str)
{
	size_t		i;

	i = 0;
	if (!pattern || !pattern->content || !str)
		return (false);
	if (!match_pattern(pattern, str, &i))
		return (false);
	while (pattern->content[i] == '*' && !pattern->quoted_map[i])
		i++;
	return (pattern->content[i] == '\0');
}

// WALLAÇOOOOO CARA DE PAU E PAU DE AÇO MEU ORGULHO
static DIR	*setup_get_matches(char *cwd, char *path, struct dirent **entry)
{
	DIR	*dir;

	if (cwd[0])
		dir = opendir(cwd);
	else
		dir = opendir(".");
	if (!dir)
		return (NULL);
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

// review . and ..
static t_word	**process_entry(struct dirent *entry, t_word **wildstr,
		const char *path)
{
	char	*next_call;
	DIR		*dir;
	t_word	**ret;

	ret = NULL;
	if ((ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name,
				"..") == 0))
		return (NULL);
	if ((entry->d_name[0] != '.' || (*wildstr)->content[0] == '.')
		&& match_wildcard(*wildstr, entry->d_name))
	{
		next_call = ft_strjoin(path, entry->d_name);
		if (!next_call)
			return (NULL);
		dir = opendir(next_call);
		if (dir)
		{
			ret = word_array_join(ret, get_matches(next_call, wildstr + 1));
			closedir(dir);
		}
		else if (access(next_call, F_OK) == 0 && !wildstr[1])
			ret = word_array_append_cstr(ret, next_call, true);
		free(next_call);
	}
	return (ret);
}

t_word	**get_matches(char *cwd, t_word **wildstr)
{
	t_word			**ret;
	char			path[PATH_MAX];
	DIR				*dir;
	struct dirent	*entry;

	if (!wildstr || !*wildstr)
	{
		if (cwd && access(cwd, F_OK) == 0)
			return (word_array_append_cstr(NULL, cwd, true));
		return (NULL);
	}
	ret = NULL;
	dir = setup_get_matches(cwd, path, &entry);
	if (!dir)
		return (NULL);
	while (entry)
	{
		ret = word_array_join(ret, process_entry(entry, wildstr, path));
		entry = readdir(dir);
	}
	closedir(dir);
	return (ret);
}
