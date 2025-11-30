//
// Created by wlucas-f on 10/21/25.
//

#include <stddef.h>
#include "../../includes/envp.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "globbing.h"
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
    const char *end;

    end = ft_strchr(assign, '=');
    if (!end)
        return (ft_strdup(assign));
    return (ft_substr(assign, 0, end - assign));
}

char    *value_from_assign(const char *assign)
{
    const char  *start;

    start = ft_strchr(assign, '=');
    if (!start)
       return (NULL);
    return (ft_strdup(start + 1));
}

t_var   *new_var(const char *assign, const bool export)
{
    t_var *var;
    var = ft_calloc(sizeof(t_var), 1);
    if (!var)
       return NULL;
    var->name = name_from_assign(assign);
    var->value = value_from_assign(assign);
    var->export = export;
    var->len = key_len(assign);
    return (var);
}

t_var   *new_var_pair(const char *name, const char *value, const bool export)
{
    t_var *var;
    var = ft_calloc(sizeof(t_var), 1);
    if (!var)
        return NULL;
    var->name = ft_strdup(name);
    var->value = ft_strdup(value);
    var->export = export;
    var->len = key_len(name);
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
    while (iter->next)
        iter = iter->next;
    iter->next = node;
    node->prev = iter;
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

// expannsions here
t_var     *envp_setvar(t_envp *env, const char *var, bool export)
{
    t_var *new;
    size_t  klen;

    if (!env || !var)
        return NULL;
    klen = key_len(var);
    new = envp_getvar(env, var);
    if (new)
    {
        if (new->value)
            free(new->value);
        if (var[klen] == '=')
            new->value = expanded_gambiarra(env, &var[klen + 1], CONSUME_QUOTES & EXPAND_VARS);
        new->export = export;
        return (new);
    }
    new = new_var(var, export);
    envp_push(env, new);
    return (new);
}

t_var     *envp_setvar_pair(t_envp *env, const char *name, const char *value, bool export)
{
    t_var *new;

    if (!env || !name)
        return NULL;
    new = envp_getvar(env, name);
    if (new)
    {
        if (new->value)
            free(new->value);
        if (value)
            new->value = expanded_gambiarra(env, value, CONSUME_QUOTES & EXPAND_VARS);
        else
            new->value = NULL;
        new->export = export;
        return (new);
    }
    new = new_var_pair(name, value, export);
    envp_push(env, new);
    return (new);
}

char     *envp_getvar_value(const t_minishell *sh, const char *name)
{
    t_var *var;

    if (!sh->env || !name)
        return NULL;
    var = envp_getvar(sh->env, name);
    if (!var && *name == '?' && !is_valid(name[1]))
        return (ft_itoa(sh->last_status));
    if (!var)
        return (NULL);
    return (ft_strdup(var->value));
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
    char    *exp;

    var = envp_getvar(env, append);
    if (!var)
        return (envp_setvar(env, append, export));
    exp = NULL;
    exp = expanded_gambiarra(env, ft_strchr(append, '=') + 1,
        CONSUME_QUOTES & EXPAND_VARS);
    join = ft_strjoin(var->value, exp);
    if (exp)
        free(exp);
    if (var->value)
        free(var->value);
    var->value = join;
    return (var);
}

//review every single export
char    **get_envp_array(const t_envp *env, bool populated_only)
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
        if (!var->export)
        {
            var = var->next;
            continue ;
        }
        if (var->value)
            *pos++ = strjoin_three(var->name, "=", var->value);
        else if (populated_only)
            *pos++ = strjoin_three(var->name, "", "");
        var = var->next;
    }
    return (ret);
}