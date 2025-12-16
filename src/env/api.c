/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:30:54 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:12:45 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/envp.h"
#include "../../includes/globbing.h"
#include "../../lib/libft/libft.h"
#include <stddef.h>
#include <stdlib.h>

t_var	*new_var(const char *name, char *value, bool export);
t_var	*envp_push(t_envp *env, t_var *node);

t_var	*envp_setvar_str(t_envp *env, const char *assign, int last_status,
		int flags)
{
	char	*name;
	char	*eq;
	char	*value;
	t_var	*var;

	if (!env || !assign)
		return (NULL);
	eq = ft_strchr(assign, '=');
	if (!eq)
		return (envp_setvar(env, assign, NULL, flags));
	name = ft_substr(assign, 0, eq - assign);
	if (flags & EXPAND_VARS)
		value = expanded_cstr(env, eq + 1, last_status, flags);
	else
		value = ft_strdup(eq + 1);
	var = envp_getvar(env, name);
	if (var)
	{
		free(var->value);
		var->value = value;
		var->export = (flags & EXPORT) | (!(flags & EXPORT) && var->export);
	}
	else
		var = envp_push(env, new_var(name, value, flags & EXPORT));
	return (free(name), var);
}

static void	append_var_value(t_var *var, char *value)
{
	char	*joined;

	if (var->value && value)
		joined = ft_strjoin(var->value, value);
	else if (value)
		joined = value;
	else
		joined = NULL;
	if (value && joined != value)
		free(value);
	free(var->value);
	var->value = joined;
}

t_var	*envp_appendvar_str(t_envp *env, const char *assign, int last_status,
		int flags)
{
	char	*name;
	char	*eq;
	char	*value;
	t_var	*var;

	if (!env || !assign)
		return (NULL);
	eq = ft_strchr(assign, '=');
	if (!eq)
		return (envp_setvar(env, assign, NULL, flags));
	name = ft_substr(assign, 0, eq - assign);
	if (name[ft_strlen(name) - 1] == '+')
		name[ft_strlen(name) - 1] = '\0';
	if (flags & EXPAND_VARS)
		value = expanded_cstr(env, eq + 1, last_status, flags);
	else
		value = ft_strdup(eq + 1);
	var = envp_getvar(env, name);
	if (!var)
		var = envp_push(env, new_var(name, value, flags & EXPORT));
	else
		append_var_value(var, value);
	free(name);
	return (var);
}
