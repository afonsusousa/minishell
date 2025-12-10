/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_stream.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:35:15 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:35:17 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/minishell.h"
#include "../../includes/tokens.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void	token_stream_init(const t_minishell *sh)
{
	t_lexer	lx;
	size_t	capacity;

	init_lexer(&lx, sh->line);
	capacity = 16;
	sh->ts->tokens = (t_token *)calloc(capacity, sizeof(t_token));
	if (sh->ts->tokens)
		sh->ts->capacity = capacity;
	else
		sh->ts->capacity = 0;
	sh->ts->count = 0;
	sh->ts->position = 0;
	sh->ts->read_position = 0;
	sh->ts->tk = NULL;
	token_stream_fill(sh->ts, &lx);
}

void	token_stream_free(t_token_stream *ts)
{
	size_t	i;

	i = 0;
	if (!ts)
		return ;
	while (i < ts->count)
	{
		if (ts->tokens[i].type == TOK_WORD
			|| ts->tokens[i].type == TOK_ASSIGNMENT_WORD)
			free(ts->tokens[i].lexeme);
		i++;
	}
	if (ts->tokens)
		free(ts->tokens);
	memset(ts, 0, sizeof(t_token_stream));
}

bool	token_stream_reserve(t_token_stream *ts, size_t needed)
{
	size_t	to_reserve;
	t_token	*new_data;

	if (ts->capacity >= needed)
		return (true);
	to_reserve = ts->capacity ? ts->capacity : 32;
	while (to_reserve < needed)
		to_reserve <<= 1;
	new_data = (t_token *)calloc(to_reserve, sizeof(t_token));
	if (!new_data)
		return (false);
	if (ts->tokens && ts->count)
		memcpy(new_data, ts->tokens, ts->count * sizeof(t_token));
	free(ts->tokens);
	ts->tokens = new_data;
	ts->capacity = to_reserve;
	return (true);
}

bool	token_stream_push(t_token_stream *ts, const t_token *token)
{
	if (!token_stream_reserve(ts, ts->count + 1))
		return (false);
	ts->tokens[ts->count++] = *token;
	return (true);
}

bool	token_stream_fill(t_token_stream *ts, t_lexer *lexer)
{
	t_token	*heap_tok;

	heap_tok = lexer_next_token(lexer);
	while (true)
	{
		if (!token_stream_push(ts, heap_tok))
			return (free(heap_tok), false);
		if (heap_tok->type == TOK_EOF)
		{
			free(heap_tok);
			break ;
		}
		free(heap_tok);
		heap_tok = lexer_next_token(lexer);
	}
	ts->position = 0;
	ts->read_position = 0;
	if (ts->count > 0)
		ts->tk = &ts->tokens[ts->read_position];
	else
		ts->tk = NULL;
	return (true);
}
