#ifndef MINISHELL_SUBST_H
# define MINISHELL_SUBST_H

# include "minishell.h"
# include <stddef.h>
# include "envp.h"

size_t	needed_space(const t_envp *env, const char *str);
size_t	check_copy(const t_envp *env, char *dest, const char *src_elem);
char	*expanded_str(const t_envp *env, const char *str);
bool    match_wildcard(const char *exp, const char *str);
char    *expand_cwd_wildcards(const char *wild_string);
int     heredoc_fd(t_minishell *sh, const char *delimiter);
#endif

