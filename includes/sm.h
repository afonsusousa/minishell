/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:40:02 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:55:51 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SM_H
# define SM_H

# include <linux/limits.h>
# include <stdbool.h>
# include <stddef.h>

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

void			sm_init(t_quote_machine *sm, const char *str);
void			sm_advance(t_quote_machine *sm);
void			sm_trasition(t_quote_machine *sm, t_sm_state new);
void			sm_laststate(t_quote_machine *sm);
void			sm_consume(t_quote_machine *sm);
void			sm_cat(t_quote_machine *sm, const char *str);

#endif // MINISHELL_SUBST_MACHINE_H
