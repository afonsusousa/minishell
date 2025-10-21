//
// Created by wlucas-f on 10/21/25.
//

#ifndef MINISHELL_ENVP_CLEAN_H
#define MINISHELL_ENVP_CLEAN_H
#include <stdbool.h>
#include <stddef.h>
typedef struct s_var  {
    char *name;
    char *value;
    bool    export;
    struct s_var *next;
} t_var;

typedef struct s_envp
{
    t_var  *head;
    size_t      count;
} t_envp;

t_var   *envp_push(t_envp *env, t_var *node);
t_var   *envp_get_by_name(t_envp *env, const char *name);
t_var   *envp_remove(t_envp *env, const char *name);
t_var   *new_var(t)

t_envp env;

#endif //MINISHELL_ENVP_CLEAN_H