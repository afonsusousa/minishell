//
// Created by afonsusousa on 9/27/25.
//

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/minishell.h"

char    *ft_strndup(const char *src, size_t len)
{
    size_t i;
    char *ret;

    i = 0;
    ret = malloc(sizeof(char) * len + 1);
    if (!ret)
        return (NULL);
    while (i < len && src[i])
    {
        ret[i] = src[i];
        i++;
    }
    return (ret);
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
    if (env->data && env->count)
        memcpy(new_data, env->data, env->count * sizeof(t_envp_elem));
    free(env->data);
    env->data = new_data;
    env->capacity = to_reserve;
    return (true);
}

t_envp_elem *elem_set(t_envp_elem *target, const char *elem, bool export)
{
    char *start;

    start = (char *) elem;
    while (*start != '=' && *start)
        start++;
    target->tag = ft_strndup(elem, start - elem);
    if (!target->tag)
        return (NULL);
    target->value = strdup(start);
    if (!target->value)
        return (free(target->tag), NULL);
    target->export = true;
    return (target);
}

t_envp_elem *envp_get_elem(const t_envp *env, const char *tag)
{
    size_t  i;
    bool    found;

    i = 0;
    found = false;
    while (i < env->count)
    {
        if (strcmp(env->data[i].tag, tag) == 0)
        {
            found = true;
            break;
        }
        i++;
    }
    if (found)
        return (&env->data[i]);
    return (NULL);
}

void elem_free(t_envp_elem **elem)
{
    if (!elem || !*elem)
        return ;
    if ((*elem)->tag)
        free((*elem)->tag);
    if ((*elem)->value)
        free((*elem)->value);
    *elem = NULL;
}

bool envp_save_elem(t_envp *env, const char *elem)
{
    size_t  i;

    i = 0;
    if (!envp_reserve(env, env->count + 1))
        return (false);
    while (i < env->capacity)
    {
        if (env->data[i].tag == NULL)
            break ;
        i++;
    }
    if (elem_set(&env->data[i], elem, true) == NULL)
        return (false);
    env->count++;
    return (true);
}

bool    envp_remove_elem(t_envp *env, const char *tag)
{
    t_envp_elem *remove;

    remove = envp_get_elem(env, tag);
    if (!remove)
        return (false);
    elem_free(&remove);
    env->count--;
}

int main(int argc, char **argv, char **envp)
{
    (void) argc;
    (void) argv;

    t_envp env = {0};

    for (int i = 0; envp[i] != NULL; i++)
        envp_save_elem(&env, envp[i]);
    for (int i = 0; i < env.count; i++)
        printf("%s=%s\n", env.data[i].tag, env.data[i].value);
}