//
// Created by wlucas-f on 10/21/25.
//

#include <stddef.h>
#include "../../includes/envp.h"
#include "../../includes/globbing.h"
#include <stdlib.h>
#include <string.h>
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"

size_t	key_len(const char *str)
{
	size_t	klen;

	klen = 0;
	while (is_valid(str[klen]))
		klen++;
	return (klen);
}

static t_var	*new_var(const char *name, char *value, bool export)
{
	t_var	*var;

	var = ft_calloc(sizeof(t_var), 1);
	if (!var)
		return (NULL);
	var->name = ft_strdup(name);
	var->value = value;
	var->export = export;
	var->len = ft_strlen(name);
	return (var);
}

static t_var	*envp_push(t_envp *env, t_var *node)
{
	t_var	*iter;

	if (!env || !node)
		return (NULL);
	node->prev = NULL;
	node->next = NULL;
	if (!env->head)
	{
		env->head = node;
		env->count = 1;
		return (node);
	}
	iter = env->head;
	while (iter->next)
		iter = iter->next;
	iter->next = node;
	node->prev = iter;
	env->count++;
	return (node);
}

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

t_var	*envp_setvar(t_envp *env, const char *name, const char *value, int flags)
{
	t_var	*var;

	if (!env || !name)
		return (NULL);
	var = envp_getvar(env, name);
	if (var)
	{
		free(var->value);
		if (value)
			var->value = ft_strdup(value);
		else
			var->value = NULL;
		if (flags & EXPORT)
			var->export = true;
		return (var);
	}
	if (value)
		var = new_var(name, ft_strdup(value), flags & EXPORT);
	else
		var = new_var(name, NULL, flags & EXPORT);
	return (envp_push(env, var));
}

t_var	*envp_appendvar(t_envp *env, const char *name, const char *value, int flags)
{
	t_var	*var;
	char	*joined;

	var = envp_getvar(env, name);
	if (!var)
		return (envp_setvar(env, name, value, flags));
	if (!value)
		return (var);
	if (var->value)
		joined = ft_strjoin(var->value, value);
	else
		joined = ft_strdup(value);
	free(var->value);
	var->value = joined;
	return (var);
}

bool	envp_unsetvar(t_envp *env, const char *name)
{
	t_var	*var;

	if (!env || !name)
		return (false);
	var = envp_getvar(env, name);
	if (!var)
		return (false);
	if (!var->prev)
	{
		if (var->next)
			var->next->prev = NULL;
		env->head = var->next;
	}
	else
		var->prev->next = var->next;
	if (var->next)
		var->next->prev = var->prev;
	env->count--;
	free(var->value);
	free(var->name);
	free(var);
	return (true);
}

t_var	*envp_setvar_str(t_envp *env, const char *assign, int last_status, int flags)
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
	value = expanded_cstr(env, eq + 1, last_status, EXPAND_VARS | CONSUME_QUOTES);
	var = envp_getvar(env, name);
	if (var)
	{
		free(var->value);
		var->value = value;
		if (flags & EXPORT)
			var->export = true;
	}
	else
		var = envp_push(env, new_var(name, value, flags & EXPORT));
	free(name);
	return (var);
}

t_var	*envp_appendvar_str(t_envp *env, const char *assign, int last_status, int flags)
{
	char	*name;
	char	*eq;
	char	*value;
	char	*joined;
	t_var	*var;

	if (!env || !assign)
		return (NULL);
	eq = ft_strchr(assign, '=');
	if (!eq)
		return (envp_setvar(env, assign, NULL, flags));
	name = ft_substr(assign, 0, eq - assign);
	if (name[ft_strlen(name) - 1] == '+')
		name[ft_strlen(name) - 1] = '\0';
	value = expanded_cstr(env, eq + 1, last_status, EXPAND_VARS | CONSUME_QUOTES);
	var = envp_getvar(env, name);
	if (!var)
		var = envp_push(env, new_var(name, value, flags & EXPORT));
	else
	{
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
	free(name);
	return (var);
}

void	free_envp(t_envp *env)
{
	t_var	*iter;
	t_var	*tmp;

	if (!env)
		return ;
	iter = env->head;
	while (iter)
	{
		free(iter->value);
		free(iter->name);
		tmp = iter->next;
		free(iter);
		iter = tmp;
	}
	env->head = NULL;
	env->count = 0;
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