/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:56:30 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:56:31 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBBING_H
# define GLOBBING_H

# include "minishell.h"
# include <stddef.h>

# define CONSUME_QUOTES 0x01
# define ECHO 0x02
# define EXPAND_VARS 0x04

// Forward declaration
typedef struct s_minishell	t_minishell;
typedef struct s_word		t_word;

size_t						needed_space(const t_envp *env, const char *str);
size_t						check_copy(const t_envp *env, char *dest,
								const char *src_elem);
char						*expanded_str(const t_envp *env, const char *str,
								bool follow_dq);
t_word						**get_matches(char *cwd, t_word **wildstr);
t_word						**expand_cwd_wildcards(t_word *word);
int							heredoc_fd(t_minishell *sh, const char *delimiter);
t_word						*expanded(t_envp *env, const char *str,
								int last_status, int flags);
char						*expanded_cstr(t_envp *env, const char *str,
								int last_status, int flags);
char						*expanded_gambiarra(t_envp *env, const char *str,
								int flags);
t_word						**expand_argv_word(const t_minishell *sh,
								const char *word);

#endif
