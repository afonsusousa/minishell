//
// Created by afonsusousa on 9/27/25.
//

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/minishell.h"

size_t  key_len(const char *str)
{
    size_t len;

    len = 0;
    while (str[len] != '=' && str[len])
        len++;
    return (len);
}

bool    envp_reserve(t_envp *env, size_t needed)
{
    size_t      to_reserve;
    t_envp_elem *new_data;

    if (env->capacity >= needed)
        return (true);
    to_reserve = env->capacity ? env->capacity : 64;
    while (to_reserve < needed)
        to_reserve <<= 1;
    new_data = (t_envp_elem *)calloc(to_reserve, sizeof(t_envp_elem));
    if (!new_data)
        return (false);
    if (env->vars && env->count)
        memcpy(new_data, env->vars, env->count * sizeof(t_envp_elem));
    free(env->vars);
    env->vars = new_data;
    env->capacity = to_reserve;
    return (true);
}

void    elem_replace_str(t_envp_elem *elem, char *str)
{
    if (elem->str)
        free(elem->str);
    elem->str = str;
    elem->tag_len = key_len(elem->str);
}

bool    envp_elem_set(t_envp    *env, char *str)
{
    size_t i;
    size_t len;

    i = 0;
    len = key_len(str);
    while (env->capacity && i < env->count)
    {
        if (env->vars[i].tag_len == len &&
            strncmp(env->vars[i].str, str, len) == 0)
            return (elem_replace_str(&env->vars[i], str), true);
        i++;
    }
    if (!envp_reserve(env, env->count + 1))
        return (false);
    return (elem_replace_str(&env->vars[env->count++], str), true);
}

t_envp_elem *envp_get_elem(const t_envp *env, const char *str)
{
    size_t i;
    size_t len;

    i = 0;
    len = key_len(str);
    while (env->capacity && i < env->count)
    {
        if (env->vars[i].tag_len == len &&
            strncmp(env->vars[i].str, str, len) == 0)
            return (&env->vars[i]);
        i++;
    }
    return NULL;
}

void elem_free(t_envp_elem *elem)
{
    if (!elem)
        return ;
    if (elem->str)
    {
        free(elem->str);
        elem->str = NULL;
    }
}

void    envp_remove_elem(t_envp *env, char *str)
{
    size_t i;
    size_t len;

    i = 0;
    len = key_len(str);
    while (env->capacity && i < env->count)
    {
        if (env->vars[i].tag_len == len &&
            strncmp(env->vars[i].str, str, len) == 0)
        {
            elem_free(&env->vars[i]);
            if (i < env->count - 1)
                memmove(&env->vars[i],
                        &env->vars[i + 1],
                        (env->count - i - 1) * sizeof(t_envp_elem));
            env->count--;
        }
        i++;
    }
}

int main(int argc, char **argv, char **envp)
{
    (void) argc;
    (void) argv;

    t_envp env = {0};

    for (int i = 0; envp[i] != NULL; i++)
        envp_elem_set(&env, strdup(envp[i]));
    envp_remove_elem(&env, "LS_COLORS");
    envp_elem_set(&env, strdup("STUPIDITY="));
    envp_elem_set(&env, strdup("STUPIDITY=123"));
    for (int i = 0; i < env.count; i++)
    {
        if (env.vars[i].str != NULL)
            printf("%s\n", env.vars[i].str);
    }
}