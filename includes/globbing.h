#ifndef MINISHELL_SUBST_H
# define MINISHELL_SUBST_H

# include <linux/limits.h>
# include "minishell.h"
# include <stddef.h>
# include "envp.h"

#define CONSUME_QUOTES 0x01
#define EXPAND_VARS 0x02

typedef enum s_state
{
    DEFAULT = 0,
    IN_DQ,
    IN_SQ,
    IN_VAR
} t_state ;



size_t	needed_space(const t_envp *env, const char *str);
size_t	check_copy(const t_envp *env, char *dest, const char *src_elem);
char    *expanded_str(const t_envp *env, const char *str, bool follow_dq);
bool    match_wildcard(const char *exp, const char *str);
char    **get_matches(char *cwd, char **wildstr);
char    **expand_cwd_wildcards(const char *wild_string);
int     heredoc_fd(t_minishell *sh, const char *delimiter);
char    *expanded(const t_minishell *sh, const char *str, int flags);
char    *expanded_gambiarra(t_envp *env, const char *str, int flags);

#endif

