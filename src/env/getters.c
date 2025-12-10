/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:31:10 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:31:11 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/envp.h"
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"
#include <stddef.h>
#include <stdlib.h>

t_var	*envp_getvar(t_envp *env, const char *name)
{
	t_var	*iter;
	size_t	klen;

	if (!env || !name)
		return (NULL);
	klen = key_len(name);
	iter = env->head;
	while (iter)
	{
		if (iter->len == klen && ft_strncmp(iter->name, name, klen) == 0)
			return (iter);
		iter = iter->next;
	}
	return (NULL);
}

char	*envp_getvar_value(t_envp *env, const char *name, int last_status)
{
	t_var	*var;

	if (!env || !name)
		return (NULL);
	if (*name == '?')
		return (ft_itoa(last_status));
	var = envp_getvar(env, name);
	if (!var || !var->value)
		return (NULL);
	return (ft_strdup(var->value));
}

char	**get_envp_array(t_envp *env, bool populated_only)
{
	char	**ret;
	char	**pos;
	t_var	*var;

	if (!env)
		return (NULL);
	ret = ft_calloc(env->count + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	pos = ret;
	var = env->head;
	while (var)
	{
		if (var->export && var->value)
			*pos++ = strjoin_three(var->name, "=", var->value);
		else if (var->export && populated_only)
			*pos++ = strjoin_three(var->name, "", "");
		var = var->next;
	}
	return (ret);
}
