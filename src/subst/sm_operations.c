/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sm_operations.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:37:01 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 19:17:53 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/globbing.h"
#include "../../lib/libft/libft.h"
#include <linux/limits.h>

void	sm_advance(t_quote_machine *sm)
{
	if (sm->str_pos < sm->str_len)
		sm->ch = sm->str[++sm->str_pos];
	else
		sm->ch = '\0';
}

void	sm_trasition(t_quote_machine *sm, t_sm_state new)
{
	sm->prev = sm->curr;
	sm->curr = new;
	sm_advance(sm);
}

void	sm_laststate(t_quote_machine *sm)
{
	sm->curr = sm->curr ^ sm->prev;
	sm->prev = sm->curr ^ sm->prev;
	sm->curr = sm->curr ^ sm->prev;
}

void	sm_consume(t_quote_machine *sm)
{
	sm->buffer[sm->buff_pos] = sm->ch;
	sm->quoted_map[sm->buff_pos] = (sm->curr == IN_SQ || sm->curr == IN_DQ);
	sm->buff_pos++;
	sm_advance(sm);
}

void	sm_cat(t_quote_machine *sm, const char *str)
{
	bool	quoted;

	if (!str)
		return ;
	quoted = (sm->curr == IN_VAR && (sm->prev == IN_DQ || sm->prev == IN_SQ))
		|| sm->curr == IN_DQ || sm->curr == IN_SQ;
	while (*str && sm->buff_pos < ARG_MAX)
	{
		sm->buffer[sm->buff_pos] = *str++;
		sm->quoted_map[sm->buff_pos] = quoted;
		sm->buff_pos++;
	}
}
