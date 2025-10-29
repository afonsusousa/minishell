//
// Created by wlucas-f on 10/21/25.
//

#ifndef MINISHELL_ENVP_CLEAN_H
#define MINISHELL_ENVP_CLEAN_H
#include <stdbool.h>
#include <stddef.h>

typedef struct s_var
{
    char *name;
    char *value;
    bool export;
    size_t len;
    struct s_var *next;
    struct s_var *prev;
} t_var;

typedef struct s_envp
{
    t_var *head;
    size_t count;
} t_envp;

t_var *new_var(const char *assign, bool export);

t_var *envp_push(t_envp *env, t_var *node);

t_var *envp_getvar(const t_envp *env, const char *name);

t_var *envp_setvar(t_envp *env, const char *var, bool export);

const char *envp_getvar_value(const t_envp *env, const char *name);

bool envp_unsetvar(t_envp *env, const char *name);

void free_envp(t_envp *env);

t_var *envp_append_var(t_envp *env, const char *append, bool export);

char **get_envp_array(const t_envp *env);

size_t key_len(const char *str);

#endif //MINISHELL_ENVP_CLEAN_H
