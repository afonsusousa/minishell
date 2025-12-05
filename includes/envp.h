//
// Created by wlucas-f on 10/21/25.
//

#ifndef MINISHELL_ENVP_CLEAN_H
#define MINISHELL_ENVP_CLEAN_H

#include <stddef.h>
#include <stdbool.h>

#define EXPORT (1 << 0)
#define USE_CTX (1 << 1)

typedef struct s_minishell t_minishell;

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

// Node Construction
t_var       *new_var(const char *assign, int flags);
t_var       *envp_push(const t_minishell *sh, t_var *node);

// Getters
t_var       *envp_getvar(const t_minishell *sh, const char *name);
char        **get_envp_array(const t_minishell *sh, bool export);

// Setters
t_var       *envp_setvar(const t_minishell *sh, const char *var, int flags);
t_var       *envp_setvar_pair(const t_minishell *sh, const char *name, const char *value, int flags);
char        *envp_getvar_value(const t_minishell *sh, const char *name);
bool        envp_unsetvar(const t_minishell *sh, const char *name);
t_var       *envp_append_var(const t_minishell *sh, const char *append, int flags);

// Cleanup
void        free_envp(t_envp *env);

// Utils
size_t      key_len(const char *str);

#endif //MINISHELL_ENVP_CLEAN_H
