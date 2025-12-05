//
// Created by wlucas-f on 10/21/25.
//

#include <stddef.h>
#include "../../includes/envp.h"
#include "../../includes/minishell.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"

static t_envp *get_env(const t_minishell *sh, int flags)
{
    if (!sh)
        return (NULL);
    if (flags & USE_CTX)
        return (sh->ctx);
    return (sh->env);
}

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

t_var   *new_var(const char *assign, const int flags)
{
    t_var *var;
    var = ft_calloc(sizeof(t_var), 1);
    if (!var)
       return NULL;
    var->name = name_from_assign(assign);
    var->value = value_from_assign(assign);
    var->export = (flags & EXPORT);
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
    if (value)
        var->value = ft_strdup(value);
    else
        var->value = NULL;
    var->export = export;
    var->len = key_len(name);
    return (var);
}

t_var *envp_push(const t_minishell *sh, t_var *node)
{
    t_var *iter;

    if (!sh || !sh->env || !node)
        return (NULL);
    node->prev = NULL;
    node->next = NULL;
    if (!sh->env->head)
    {
        sh->env->head = node;
        sh->env->count = 1;
        return (node);
    }
    iter = sh->env->head;
    while (iter->next)
        iter = iter->next;
    iter->next = node;
    node->prev = iter;
    sh->env->count++;
    return (node);
}


t_var *envp_getvar(const t_minishell *sh, const char *name)
{
    t_var *iter;
    size_t klen;

    if (!sh || !sh->env || !name)
        return NULL;
    klen = key_len(name);
    iter = sh->env->head;
    while (iter)
    {
        if (iter->len == klen && ft_strncmp(iter->name, name, klen) == 0)
            return iter;
        iter = iter->next;
    }
    return NULL;
}

t_var     *envp_setvar(const t_minishell *sh, const char *var, int flags)
{
    t_var *new;
    size_t  klen;
    t_envp *env;

    env = get_env(sh, flags);
    if (!env || !var)
        return NULL;
    klen = key_len(var);
    new = envp_getvar(sh, var);
    if (new)
    {
        free(new->value);
        if (var[klen] == '=')
            new->value = ft_strdup(&var[klen + 1]);
        else
            new->value = NULL;
        new->export = (flags & EXPORT);
        return (new);
    }
    new = new_var(var, flags);
    envp_push(sh, new);
    return (new);
}

t_var     *envp_setvar_pair(const t_minishell *sh, const char *name, const char *value, int flags)
{
    t_var *new;
    t_envp *env;

    env = get_env(sh, flags);
    if (!env || !name)
        return NULL;
    new = envp_getvar(sh, name);
    if (new)
    {
        free(new->value);
        if (value)
            new->value = ft_strdup(value);
        else
            new->value = NULL;
        new->export = (flags & EXPORT);
        return (new);
    }
    new = new_var_pair(name, value, (flags & EXPORT));
    envp_push(sh, new);
    return (new);
}

char     *envp_getvar_value(const t_minishell *sh, const char *name)
{
    t_var *var;

    if (!sh || !sh->env || !name)
        return NULL;
    var = envp_getvar(sh, name);
    if (!var && *name == '?')
        return (ft_itoa(sh->last_status));
    if (!var || !var->value)
        return (NULL);
    return (ft_strdup(var->value));
}

bool   envp_unsetvar(const t_minishell *sh, const char *name)
{
    t_var *var;

    if (!sh || !sh->env || !name)
        return (false);
    var = envp_getvar(sh, name);
    if (!var)
        return (false);
    if (!var->prev)
    {
        if (var->next)
            var->next->prev = NULL;
        sh->env->head = var->next;
    }
    else
        var->prev->next = var->next;
    if (var->next)
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

t_var *envp_append_var(const t_minishell *sh, const char *append, int flags)
{
    t_var	*var;
    char    *new_part;
    char    *joined;
    t_envp *env;

    env = get_env(sh, flags);
    if (!env || !append)
        return (NULL);
    var = envp_getvar(sh, append);
    if (!var)
        return (envp_setvar(sh, append, flags));
    new_part = ft_strchr(append, '=');
    if (!new_part)
        return (var);
    new_part++;
    if (var->value)
        joined = ft_strjoin(var->value, new_part);
    else
        joined = ft_strdup(new_part);
    free(var->value);
    var->value = joined;
    return (var);
}

//review every single export
char    **get_envp_array(const t_minishell *sh, bool populated_only)
{
    char **ret;
    char **pos;
    t_var *var;

    if (!sh || !sh->env)
        return (NULL);
    ret = ft_calloc(sh->env->count + 1,sizeof(char *));
    if (!ret)
        return NULL;
    pos = ret;
    var = sh->env->head;
    while (var && (size_t)(pos - ret) < sh->env->count)
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