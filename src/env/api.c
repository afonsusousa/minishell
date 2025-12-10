//
// Created by wlucas-f on 10/21/25.
//

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
	value = expanded_cstr(env, eq + 1, last_status,
			EXPAND_VARS | CONSUME_QUOTES);
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

t_var	*envp_appendvar_str(t_envp *env, const char *assign, int last_status,
		int flags)
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
	value = expanded_cstr(env, eq + 1, last_status,
			EXPAND_VARS | CONSUME_QUOTES);
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
