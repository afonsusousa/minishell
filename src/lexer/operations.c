/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:35:09 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:35:11 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

void	lexer_read_char(t_lexer *lexer)
{
	if (lexer->read_position >= lexer->input_len)
		lexer->ch = '\0';
	else
		lexer->ch = lexer->input[lexer->read_position];
	lexer->position = lexer->read_position;
	lexer->read_position++;
}

char	lexer_peek_char(const t_lexer *lexer)
{
	if (lexer->read_position >= lexer->input_len)
		return ('\0');
	return (lexer->input[lexer->read_position]);
}

// >| is a valid redirection, but not posix... review later
bool	lexer_next_redir(t_lexer *lexer)
{
	char	peek;

	peek = lexer_peek_char(lexer);
	if (lexer->ch == '>' && peek == '>')
		lexer->tk = token_new(TOK_REDIR_APPEND);
	else if (lexer->ch == '>' && peek == '|')
		lexer->tk = token_new(TOK_REDIR_OUT);
	else if (lexer->ch == '<' && peek == '<')
		lexer->tk = token_new(TOK_HEREDOC);
	else if (lexer->ch == '0' && peek == '<')
		lexer->tk = token_new(TOK_REDIR_0_IN);
	else if (lexer->ch == '1' && peek == '<')
		lexer->tk = token_new(TOK_REDIR_1_IN);
	else if (lexer->ch == '2' && peek == '<')
		lexer->tk = token_new(TOK_REDIR_2_IN);
	else if (lexer->ch == '0' && peek == '>')
		lexer->tk = token_new(TOK_REDIR_0_OUT);
	else if (lexer->ch == '1' && peek == '>')
		lexer->tk = token_new(TOK_REDIR_1_OUT);
	else if (lexer->ch == '2' && peek == '>')
		lexer->tk = token_new(TOK_REDIR_2_OUT);
	else
		return (false);
	return (lexer_read_char(lexer), true);
}

bool	lexer_next_dmeta(t_lexer *lexer)
{
	char	peek;

	peek = lexer_peek_char(lexer);
	if (lexer->ch == '|' && peek == '|')
		lexer->tk = token_new(TOK_OR);
	else if (lexer->ch == '&' && peek == '&')
		lexer->tk = token_new(TOK_AND);
	else
		return (false);
	lexer_read_char(lexer);
	return (true);
}

bool	lexer_next_smeta(t_lexer *lexer)
{
	if (lexer->ch == '|')
		lexer->tk = token_new(TOK_PIPE);
	else if (lexer->ch == '&')
		lexer->tk = token_new(TOK_AMP);
	else if (lexer->ch == '>')
		lexer->tk = token_new(TOK_REDIR_OUT);
	else if (lexer->ch == '<')
		lexer->tk = token_new(TOK_REDIR_IN);
	else if (lexer->ch == ';')
		lexer->tk = token_new(TOK_SEMI);
	else if (lexer->ch == '(')
		lexer->tk = token_new(TOK_LPAREN);
	else if (lexer->ch == ')')
		lexer->tk = token_new(TOK_RPAREN);
	else
		return (false);
	return (true);
}
