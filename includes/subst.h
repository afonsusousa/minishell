#ifndef MINISHELL_SUBST_H
# define MINISHELL_SUBST_H

# include <stddef.h>
# include "envp.h"

size_t	needed_space(t_envp *env, char *str);
size_t	check_copy(t_envp *env, char *dest, char *src_elem);
char	*expanded_str(t_envp *env, char *str);
void    envp_elem_append(t_envp *env, char *str);
#endif

