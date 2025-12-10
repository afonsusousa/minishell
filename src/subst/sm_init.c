/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sm_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 19:16:24 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 19:35:06 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/globbing.h"
#include "../../lib/libft/libft.h"

void	sm_init(t_quote_machine *sm, const char *str)
{
	size_t	i;

	sm->str = str;
	sm->ch = *str;
	sm->str_pos = 0;
	sm->buff_pos = 0;
	sm->curr = DEFAULT;
	sm->prev = DEFAULT;
	sm->str_len = ft_strlen(str);
	ft_bzero(sm->buffer, ARG_MAX);
	i = 0;
	while (i < ARG_MAX)
		sm->quoted_map[i++] = false;
}
