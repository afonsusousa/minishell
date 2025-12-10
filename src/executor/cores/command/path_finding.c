/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finding.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:32:24 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:32:32 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/executor.h"
#include "../../../../includes/minishell.h"
#include "../../../../includes/utils.h"
#include "../../../../lib/libft/libft.h"
#include <stdlib.h>
#include <unistd.h>

static char	*search_path(t_minishell *sh, char *cmd)
{
	size_t	i;
	char	**split_path;
	char	*try;
	char	*path;

	path = envp_getvar_value(sh->env, "PATH", sh->last_status);
	if (!path)
		return (NULL);
	split_path = ft_split(path, ':');
	free(path);
	if (!split_path)
		return (NULL);
	i = 0;
	while (split_path[i] && ft_strcmp(cmd, ".") != 0 && *cmd)
	{
		try = strjoin_three(split_path[i++], "/", cmd);
		if (!try)
			return (free_until_null(&split_path), NULL);
		if (access(try, X_OK) == 0)
			return (free_until_null(&split_path), try);
		free(try);
	}
	return (free_until_null(&split_path), ft_strdup(cmd));
}

char	*find_path(t_minishell *sh, char *cmd)
{
	char		*expanded_cmd;
	const t_var	*path;

	expanded_cmd = expand_tilde(sh, cmd);
	if (!expanded_cmd)
		expanded_cmd = cmd;
	path = envp_getvar(sh->env, "PATH");
	if (ft_strchr(expanded_cmd, '/') || !path || !path->value || !*path->value)
	{
		if (access(expanded_cmd, F_OK) == 0)
			return (expanded_cmd);
		if (expanded_cmd != cmd)
			free(expanded_cmd);
		return (ft_strdup(cmd));
	}
	if (expanded_cmd != cmd)
		free(expanded_cmd);
	return (search_path(sh, cmd));
}
