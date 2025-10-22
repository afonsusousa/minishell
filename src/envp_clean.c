//
// Created by wlucas-f on 10/21/25.
//

#include <stddef.h>
#include "../includes/envp_clean.h"

#include <stdlib.h>

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

t_var   *new_var(char *name, char *value, bool export)
{
    if (!name)
        return NULL;
    t_var *var;
    var = ft_calloc(sizeof(t_var), 1);
    if (!var)
       return NULL;
    var->name = name;
    var->value = value;
    var->export = export;
    var->length = ft_strlen(name);
    return (var);
}

t_var   *envp_push(t_envp *env, t_var *node)
{
   if (!env || !node)
       return NULL;
    t_var *iter;
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

t_var   *envp_get_by_name(t_envp *env, const char *name)
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

t_var     *envp_set(t_envp *env, const char *var, bool export)
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
    new = new_var(ft_strndup(var, klen), ft_strdup(var + klen + 1), export);
    envp_push(env, new);
    return (new);
}

char     *envp_get_value(t_envp *env, const char *name)
{
    t_var *var;

    if (!env || !name)
        return NULL;
    var = envp_get_by_name(env, name);
    if (!var)
        return NULL;
    return (var->value);
}

bool   envp_unset(t_envp *env, const char *name)
{
    t_var *var;

    var = envp_get_by_name(env, name);
    if (!var)
        return false;
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
}