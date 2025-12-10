/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:56:30 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 18:41:42 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBBING_H
# define GLOBBING_H

# include "minishell.h"
# include <stddef.h>
# include <linux/limits.h>

# define CONSUME_QUOTES 0x01
# define ECHO 0x02
# define EXPAND_VARS 0x04

// Forward declaration
typedef struct s_minishell	t_minishell;
typedef struct s_word		t_word;

typedef enum s_sm_state
{
	DEFAULT = 0,
	IN_DQ,
	IN_SQ,
	IN_VAR
}				t_sm_state;

typedef struct s_quote_machine
{
	t_sm_state	curr;
	t_sm_state	prev;
	const char	*str;
	size_t		str_pos;
	size_t		str_len;
	char		ch;
	char		buffer[ARG_MAX];
	bool		quoted_map[ARG_MAX];
	size_t		buff_pos;
}				t_quote_machine;

typedef struct s_word
{
	char		*content;
	bool		*quoted_map;
	size_t		len;
}				t_word;

// Quote Machine Actions
void	sm_init(t_quote_machine *sm, const char *str);
void	sm_advance(t_quote_machine *sm);
void	sm_trasition(t_quote_machine *sm, t_sm_state new);
void	sm_laststate(t_quote_machine *sm);
void	sm_consume(t_quote_machine *sm);
void	sm_cat(t_quote_machine *sm, const char *str);

// Expanding Protein
size_t	needed_space(const t_envp *env, const char *str);
size_t	check_copy(const t_envp *env, char *dest,
			const char *src_elem);
char	*expanded_str(const t_envp *env, const char *str,
			bool follow_dq);
t_word	**get_matches(char *cwd, t_word **wildstr);
t_word	**expand_cwd_wildcards(t_word *word);
int		heredoc_fd(t_minishell *sh, const char *delimiter);
t_word	*expanded(t_envp *env, const char *str,
			int last_status, int flags);
char	*expanded_cstr(t_envp *env, const char *str,
			int last_status, int flags);
char	*expanded_gambiarra(t_envp *env, const char *str,
			int flags);
t_word	**expand_argv_word(const t_minishell *sh,
			const char *word);

// t_word utilities
void	word_free(t_word *ts);
t_word	*word_dup(const t_word *ts);
t_word	*word_new(const char *content, bool all_quoted);
void	word_free(t_word *ts);
t_word	**word_split(t_word *ts, bool (*is_separator)(char),
			bool quote_aware);
void	word_free_until_null(t_word **splits);
t_word	*word_dup(const t_word *ts);
t_word	**word_array_join(t_word **a, t_word **b);
t_word	**word_array_append_cstr(t_word **arr,
			const char *content, bool all_quoted);
t_word	**word_array_append_word(t_word **arr,
			const t_word *word);
char	*word_to_cstr(const t_word *word);
char	**word_to_cstr_array(t_word **words);
size_t	word_array_len(t_word **arr);
#endif
