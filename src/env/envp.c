//
// Created by wlucas-f on 10/21/25.
//

#include <stddef.h>
#include "../../includes/envp.h"
#include <ctype.h>
#include <stdlib.h>
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"

size_t key_len(const char *str)
{
    size_t klen;

    klen = 0;
    while (is_valid(str[klen]))
        klen++;
    return (klen);
}

char    *name_from_assign(const char *assign)
{
    if (!ft_strchr(assign, '='))
        return (ft_strdup(assign));
    return (ft_strndup(assign, key_len((assign))));
}

char    *value_from_assign(const char *assign)
{
    if (!ft_strchr(assign, '='))
       return (NULL);
    return (ft_strdup(assign + key_len(assign) + 1));
}

t_var   *new_var(const char *assign, bool export)
{
    t_var *var;
    var = ft_calloc(sizeof(t_var), 1);
    if (!var)
       return NULL;
    var->name = name_from_assign(assign);
    var->value = value_from_assign(assign);
    var->export = export;
    var->len = ft_strlen(var->name);
    return (var);
}

t_var *envp_push(t_envp *env, t_var *node)
{
    t_var *iter;

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
    while (iter && ft_strcmp(iter->name, node->name) < 0)
        iter = iter->next;
    if (!iter)
    {
        iter = env->head;
        while (iter->next)
            iter = iter->next;
        iter->next = node;
        node->prev = iter;
        node->next = NULL;
        env->count++;
        return (node);
    }
    node->next = iter;
    node->prev = iter->prev;
    if (iter->prev)
        iter->prev->next = node;
    else
        env->head = node;
    iter->prev = node;
    env->count++;
    return (node);
}


t_var *envp_getvar(const t_envp *env, const char *name)
{
    t_var *iter;
    size_t klen;

    if (!env || !name)
        return NULL;
    klen = key_len(name);
    iter = env->head;
    while (iter)
    {
        if (iter->len == klen && ft_strncmp(iter->name, name, klen) == 0)
            return iter;
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
    new = envp_getvar(env, var);
    if (new && ft_strchr(var, '='))
    {
        free(new->value);
        //expanded
        new->value = ft_strdup(var + klen);
        return (new);
    }
    new = new_var(var, export);
    envp_push(env, new);
    return (new);
}

const char     *envp_getvar_value(const t_envp *env, const char *name)
{
    t_var *var;

    if (!env || !name)
        return NULL;
    var = envp_getvar(env, name);
    if (!var)
        return NULL;
    return ((const char *)var->value);
}

bool   envp_unsetvar(t_envp *env, const char *name)
{
    t_var *var;

    var = envp_getvar(env, name);
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

void    free_envp(t_envp *env)
{
    t_var *iter;
    t_var *tmp;

    if (!env)
        return ;
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
    char    *join;

    var = envp_getvar(env, append);
    if (!var)
        return (envp_setvar(env, append, export));
    //expanded
    join = ft_strjoin(var->value, ft_strchr(append, '=') + 1);
    free(var->value);
    var->value = join;
    return (var);
}

char    **get_envp_array(const t_envp *env)
{
    char **ret;
    char **pos;
    t_var *var;

    if (!env)
        return (NULL);
    ret = ft_calloc(env->count + 1,sizeof(char *));
    if (!ret)
        return NULL;
    pos = ret;
    var = env->head;
    while (var && (size_t)(pos - ret) < env->count)
    {
        if (var->export)
            *pos++ = strjoin_three(var->name, "=", var->value);
        var = var->next;
    }
    return (ret);
}