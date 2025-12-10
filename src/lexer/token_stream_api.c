/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_stream_api.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:34:57 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:34:58 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/tokens.h"
#include <stddef.h>

static void	ts_read_token(t_token_stream *ts)
{
	if (ts->read_position >= ts->count && ts->count > 0)
		ts->tk = &ts->tokens[ts->count - 1];
	else if (ts->count > 0)
		ts->tk = &ts->tokens[ts->read_position];
	else
		ts->tk = NULL;
	ts->position = ts->read_position;
	ts->read_position += 1;
}

const t_token	*ts_peek(const t_token_stream *ts)
{
	if (ts->read_position >= ts->count && ts->count > 0)
		return (&ts->tokens[ts->count - 1]);
	return (&ts->tokens[ts->read_position]);
}

int	ts_check(const t_token_stream *ts, t_token_type type)
{
	return (ts_peek(ts)->type == type);
}

int	ts_match(t_token_stream *ts, t_token_type type)
{
	if (ts_check(ts, type))
		return (ts_read_token(ts), 1);
	return (0);
}
