//
// Created by wlucas-f on 10/21/25.
//

#include <stddef.h>
#include "../../includes/envp.h"
#include <stdlib.h>
#include "../../lib/libft/libft.h"

t_var	*new_var(const char *name, char *value, bool export);
t_var	*envp_push(t_envp *env, t_var *node);

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

