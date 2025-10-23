//
// Created by wlucas-f on 10/21/25.
//

#include <stddef.h>
#include "../includes/envp_clean.h"

#include <stdlib.h>

#include "../includes/envp.h"
#include "../lib/libft/libft.h"

char    *ft_strndup(const char *str, size_t size)
{
    char	*t;
    char	*r;

    if (!str)
        return (NULL);
    t = (char *)malloc(size + 1);
    if (!t)
        return (NULL);
    r = t;
    while ((size_t)(t - r) < size && *str)
        *t++ = *str++;
    *t = 0;
    return (r);
}

char    *name_from_assign(char *assign)
{
    if (!ft_strchr(assign, '='))
        return (ft_strdup(assign));
    return (ft_strndup(assign, key_len((assign))));
}

char    *value_from_assign(char *assign)
{
    if (!ft_strchr(assign, '='))
       return (ft_strdup(assign));
    return (ft_strdup(assign + key_len(assign) + 1));
}

t_var   *new_var(char *assign, bool export)
{
    t_var *var;
    char *name;
    char *value;

    if (!name)
        return NULL;
    var = ft_calloc(sizeof(t_var), 1);
    if (!var)
       return NULL;
    var->name = name_from_assign(assign);
    if (ft_strchr(assign, '='))
        var->value = value_from_assign(assign);
    else
        var->value = NULL;
    var->export = export;
    var->len = ft_strlen(name);
    return (var);
}

t_var   *envp_push(t_envp *env, t_var *node)
{
    t_var *iter;
   if (!env || !node)
       return NULL;
    iter = env->head;
    if (!env->head)
    {
        env->head = node;
        return (node);
    }
    while (iter->next)
      iter = iter->next;
    iter->next = node;
    node->prev = iter;
    return (node);
}

size_t key_len(const char *str)
{
    size_t klen;

    klen = 0;
    while (str[klen] && str[klen] != '=')
        klen++;
    return (klen);
}

t_var   *envp_getvar(t_envp *env, const char *name)
{
    t_var *iter;
    size_t klen;
    if (!env || !name)
       return NULL;
    klen = key_len(name);
    iter = env->head;
    while (iter)
    {
        if (klen > iter->length && ft_strncmp(iter->name, name, klen) == 0)
            return (iter);
        if (ft_strncmp(iter->name, name, iter->length) == 0)
            return (iter);
        iter = iter->next;
    }
    return NULL;
}

t_var     *envp_setvar(t_envp *env, const char *var, bool export)
{
    t_var *new;
    size_t  klen;

    if (!env || !var)
        return NULL;
    klen = key_len(var);
    new = envp_get_by_name(env, var);
    if (new && ft_strchr(var, '='))
    {
        free(new->value);
        new->value = ft_strdup(var + klen);
        return (new);
    }
    new = new_var(var, export);
    envp_push(env, new);
    return (new);
}

char     *envp_getvar_value(t_envp *env, const char *name)
{
    t_var *var;

    if (!env || !name)
        return NULL;
    var = envp_get_by_name(env, name);
    if (!var)
        return NULL;
    return (var->value);
}

bool   envp_unsetvar(t_envp *env, const char *name)
{
    t_var *var;

    var = envp_get_by_name(env, name);
    if (!var)
        return (false);
    if (!var->prev)
    {
        var->next->prev = NULL;
        env->head = var->next;
    }
    else
        var->prev->next = var->next;
    if (!var->next)
        var->prev->next = NULL;
    else
        var->next->prev = var->prev;
    free(var->value);
    free(var->name);
    return (free(var), true);
}

void    envp_clean(t_envp *env)
{
    t_var *iter;
    t_var *tmp;

    iter = env->head;
    tmp = NULL;
    while (iter)
    {
        free(iter->value);
        free(iter->name);
        tmp = iter->next;
        free(iter);
        iter = tmp;
    }
    env->head = NULL;
}

t_var *envp_append_var(t_envp *env, const char *append, bool export)
{
    t_var	*var;
    char    *joined;

    var = envp_getvar(env, append);
    if (!var)
        return (envp_setvar(env, append, export));

}