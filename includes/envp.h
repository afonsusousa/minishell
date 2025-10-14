#ifndef MINISHELL_ENVP_H
# define MINISHELL_ENVP_H

# include <stddef.h>
# include <stdbool.h>
# include "minishell.h"

size_t		key_len(const char *str);
bool		envp_reserve(t_envp *env, size_t needed);
void		var_replace_str(t_var *elem, char *str);
t_var		*envp_var_set(t_envp *env, char *str);
t_var	    *envp_get_var(const t_envp *env, const char *str);
char		*envp_get_var_value(const t_envp *env, const char *str);
void		free_var(t_var *elem);
void		envp_remove_var(t_envp *env, const char *str);
t_var		*envp_var_append(t_envp *env, const char *str);
void        free_envp(t_envp *env);

#endif

