#ifndef MINISHELL_ENVP_H
# define MINISHELL_ENVP_H

# include <stddef.h>
# include <stdbool.h>
# include "minishell.h"

size_t		key_len(const char *str);
bool		envp_reserve(t_envp *env, size_t needed);
void		elem_replace_str(t_envp_elem *elem, char *str);
bool		envp_elem_set(t_envp *env, char *str);
t_envp_elem	*envp_get_elem(const t_envp *env, const char *str);
char		*envp_get_elem_value(const t_envp *env, const char *str);
void		elem_free(t_envp_elem *elem);
void		envp_remove_elem(t_envp *env, const char *str);
void		envp_elem_append(t_envp *env, char *str);

#endif

