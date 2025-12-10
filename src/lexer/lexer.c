/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 03:29:15 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:21:56 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"
#include <stddef.h>
#include <string.h>

t_token_type	evaluate_assign(const t_token *t)
{
	size_t	i;
	size_t	len;
	bool	first;

	i = 0;
	first = true;
	len = ft_strlen(t->lexeme);
	if (ft_isdigit(t->lexeme[0]))
		return (t->type);
	while (i < len)
	{
		if (first && t->lexeme[i] == '=')
		{
			if (i && i + 1 < len && ((!is_space(t->lexeme[i - 1]) 
						|| t->lexeme[i - 1] == '+')
					|| !is_space(t->lexeme[i + 1])))
				return (TOK_ASSIGNMENT_WORD);
			first = false;
		}
		if (!is_valid(t->lexeme[i]) && t->lexeme[i] != '+')
			return (t->type);
		i++;
	}
	return (t->type);
}

static int	handle_quotes(t_lexer *lx, int *sq, int *dq)
{
	if (*dq == 0 && lx->ch == '\'')
	{
		*sq = !*sq;
		lexer_read_char(lx);
		return (1);
	}
	if (*sq == 0 && lx->ch == '"')
	{
		*dq = !*dq;
		lexer_read_char(lx);
		return (1);
	}
	return (0);
}

void	lexer_read_word(t_lexer *lexer, t_token *token)
{
	size_t	start;
	int		sq;
	int		dq;

	start = lexer->position;
	sq = 0;
	dq = 0;
	while (lexer->ch != '\0')
	{
		if (handle_quotes(lexer, &sq, &dq))
			continue ;
		if (!sq && !dq && is_break(lexer->ch))
			break ;
		lexer_read_char(lexer);
	}
	token->type = TOK_WORD;
	token->lexeme = ft_strndup(lexer->input + start, lexer->position - start);
}

t_token	*lexer_next_token(t_lexer *lexer)
{
	while (is_space(lexer->ch))
		lexer_read_char(lexer);
	if (lexer->ch == '\0')
		lexer->tk = token_new(TOK_EOF);
	else if (lexer_next_dmeta(lexer) || lexer_next_redir(lexer)
		|| lexer_next_smeta(lexer))
		lexer_read_char(lexer);
	else
	{
		lexer->tk = token_new(TOK_WORD);
		if (lexer->tk == NULL)
			return (NULL);
		lexer_read_word(lexer, lexer->tk);
		lexer->tk->type = evaluate_assign(lexer->tk);
	}
	return (lexer->tk);
}

void	init_lexer(t_lexer *lx, char *input)
{
	lx->input = input;
	lx->input_len = strlen(input);
	lx->position = 0;
	lx->read_position = 0;
	lx->ch = '\0';
	lexer_read_char(lx);
}