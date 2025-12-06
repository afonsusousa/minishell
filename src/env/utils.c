//
// Created by wlucas-f on 10/21/25.
//

#include <stddef.h>
#include "../../includes/envp.h"
#include "../../includes/globbing.h"
#include "../../includes/utils.h"
#include <stdlib.h>
#include "../../lib/libft/libft.h"

size_t	key_len(const char *str)
{
	size_t	klen;

	klen = 0;
	while (is_valid(str[klen]))
		klen++;
	return (klen);
}

t_var	*new_var(const char *name, char *value, bool export)
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

t_var	*envp_push(t_envp *env, t_var *node)
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
